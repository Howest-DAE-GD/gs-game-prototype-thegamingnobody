#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <iostream>
#include <chrono>
#include <thread> 
#include <execution>
#include <algorithm>

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize(bool addScore)
{
	m_Camera = std::make_unique<Camera>(Camera(GetViewPort().width, GetViewPort().height));
	auto playerShape{ Rectf( 75.0f, 75.0f, 25.0f, 25.0f )};
	m_Player = std::make_unique<dae::Player>(playerShape, dae::StateOfMatter::solid, Color4f(0.43f, 0.96f, 0.42f, 1.0f), 300.0f);
	m_GoalRadius = std::make_unique<dae::LevelObject>(Rectf(0, 0, 1, 1), dae::LevelObjectTypes::GoalSpawnRadius);
	InitializeLevel();
	MakeNextFloor(addScore);
}

void Game::Cleanup( )
{
	
}

void Game::Update( float elapsedSec )
{
	bool goalNotReached{ true }; //if reached this frame

	HandleInput(elapsedSec);

	m_Timer.Update(elapsedSec);

	//make predicted player shape
	auto const playerPredictedPos{ m_Player->PredictPosition(elapsedSec) };

	auto playerShape{ m_Player->GetShape(true) };
	playerShape.left = playerPredictedPos.x;
	playerShape.bottom = playerPredictedPos.y;

	//set up threading for player wall collision
	std::vector<std::future<bool>> futureBools;
	std::vector<std::jthread> threads;

	unsigned int const nrOfThreads{ 1 };
	unsigned int const nrPerThread{ static_cast<int>(m_LevelWalls.size()) / nrOfThreads };

	for (unsigned int i = 0; i < nrOfThreads; i++)
	{
		std::promise<bool> promiseBool;
		futureBools.emplace_back(promiseBool.get_future());
		threads.emplace_back(&Game::IsRectValidPromise, this, std::move(promiseBool), playerShape, true, i * nrPerThread, nrPerThread, false, CheckingMode::walls);
	}

	threads.clear();

	if (utils::IsOverlapping(playerShape, m_GoalObject->GetShape(true)))
	{
		if (m_FadingStage == ScreenfadeStage::NoFade)
		{
			m_FadingStage = ScreenfadeStage::FadingIn;

		}
	}
	
	for (auto& object : m_Coin)
	{
		Circlef coinshape{ Circlef(object.GetCoinShape().center, object.GetCoinShape().radiusX) };

		if (utils::IsOverlapping(playerShape, coinshape))
		{
			if (object.IsEnabled())
			{
				m_Timer.AddScore(1);
				object.SetEnabled(false);

			}
		}
	}

	for (auto& future : futureBools)
	{
		if (not future.get())
		{
			m_Player->ResetDirectionThisFrame();
			break;
		}
	}

	futureBools.clear();

	for (unsigned int i = 0; i < nrOfThreads; i++)
	{
		std::promise<bool> promiseBool;
		futureBools.emplace_back(promiseBool.get_future());
		threads.emplace_back(&Game::IsRectValidPromise, this, std::move(promiseBool), playerShape, true, i * nrPerThread, nrPerThread, false, CheckingMode::dangerTiles);
	}

	threads.clear();

	for (auto& future : futureBools)
	{
		if (not future.get())
		{
			m_Timer.PlayerDied();
			m_Player->MarkDie();
			break;
		}
	}

	if (m_Timer.HasTimerEnded())
	{
		m_Timer.PlayerDied();
		m_Player->MarkDie();
		
	}

	float const timeToFade{ 1.0f };
	float const maxAlpha{ 1.0f };
	float const alphaOffset{ (maxAlpha / timeToFade) * elapsedSec * 2 };

	switch (m_FadingStage)
	{
	case Game::ScreenfadeStage::FadingOut:
		m_LevelWalls.back().SetAlpha(-alphaOffset);
		if (m_LevelWalls.back().GetColor().a <= 0.0f)
		{
			m_FadingStage = ScreenfadeStage::NoFade;
		}
		break;
	case Game::ScreenfadeStage::FadingIn:
		m_LevelWalls.back().SetAlpha(alphaOffset);
		if (m_LevelWalls.back().GetColor().a >= 1.5f)
		{
			m_FadingStage = ScreenfadeStage::FadingOut;
			goalNotReached = false;
			if (not m_Timer.IsPaused())
			{
				m_Timer.TogglePause();
			}
		}
		break;
	case Game::ScreenfadeStage::NoFade:
		break;
	}

	if (goalNotReached)
	{
		m_Player->Update(elapsedSec);
	}
	else
	{
		MakeNextFloor();
	}

	for (auto& object : m_DangerTiles)
	{
		object.Update(elapsedSec);
	}


}

void Game::Draw( ) const
{
	ClearBackground( );

	glPushMatrix();

	m_Camera->Transform(m_Player->GetShape(false), false);

	m_GoalObject->Draw();

	for (auto& object : m_DangerTiles)
	{
		object.Draw();
	}

	for (auto& object : m_Coin)
	{
		if (object.IsEnabled())
		{
			object.DrawCoin();
		}
	}

	utils::SetColor(Color4f(0.4f, 0.4f, 0.4f, 1.0f));
	m_Player->Draw();

	for (auto& object : m_LevelWalls)
	{
		if (object.IsEnabled())
		{
			object.Draw();
		}
	}

	glPopMatrix();

	if (not m_Timer.IsPaused())
	{
		m_Timer.Draw();
	}

	glPushMatrix();

	m_Camera->Transform(m_Player->GetShape(false), false);
	
	(m_LevelWalls.end() - 1)->Draw();

	glPopMatrix();
}


void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;

	//switch ( e.keysym.sym )
	//{
	//case SDLK_UP:
	//	
	//	break;
	//case SDLK_DOWN:
	//	m_Position.y += m_MoveSpeed;
	//	break;
	//case SDLK_LEFT:
	//	m_Position.x -= m_MoveSpeed;
	//	break;
	//case SDLK_RIGHT:
	//	m_Position.x += m_MoveSpeed;
	//	break;
	//}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	switch ( e.keysym.sym )
	{
	case SDLK_r:
		ResetGame();
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.9f, 0.9f, 0.9f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::InitializeLevel()
{
	//nr of columns: 22 (maze can only use 1 to 20)
	//nr of rows: 22 (maze can only use 1 to 20)

	m_LevelWalls.emplace_back(dae::LevelObject(Rectf( 0,  0,  1, 22), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(21,  0,  1, 22), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf( 0,  0, 22,  1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf( 0, 21, 22,  1), wallThickness, dae::LevelObjectTypes::Wall));
	
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf( 2,  4,  3,  1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf( 2,  5,  1,  2), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf( 2,  8,  5,  1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf( 6,  9,  1,  4), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf( 6,  6,  3,  1), wallThickness, dae::LevelObjectTypes::Wall));

	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(6, 2, 1, 4), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(7, 2, 4, 1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(10, 4, 3, 3), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(12, 7, 1, 5), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(13,  9,  2,  1), wallThickness, dae::LevelObjectTypes::Wall));

	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(14, 4, 3, 1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(16, 1, 1, 3), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(16, 6, 3, 1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(16, 7, 1, 5), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(16, 11,  5,  1), wallThickness, dae::LevelObjectTypes::Wall));

	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(8, 11, 4, 1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(8, 12, 1, 6), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(4, 17, 4, 1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(4, 11, 1, 6), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf( 1, 11,  3,  1), wallThickness, dae::LevelObjectTypes::Wall));

	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(2, 13, 1, 3), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(4, 19, 1, 2), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(10, 17, 1, 4), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(12, 17, 1, 3), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(13, 17,  6,  1), wallThickness, dae::LevelObjectTypes::Wall));

	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(18, 18,  1,  2), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(12, 13,  1,  3), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(12, 13,  6,  1), wallThickness, dae::LevelObjectTypes::Wall));

	m_LevelWalls.emplace_back(dae::LevelObject(Rectf(-50, -50,  100,  100), wallThickness, dae::LevelObjectTypes::FadeEffect));

	GenerateNewGoal(wallThickness);
}

void Game::HandleInput(float elapsedSec)
{
	// Check keyboard state
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (pStates[SDL_SCANCODE_UP] or pStates[SDL_SCANCODE_W])
	{
		m_Player->AddDirectionCurrentFrame(0, 1);
	}
	if (pStates[SDL_SCANCODE_DOWN] or pStates[SDL_SCANCODE_S])
	{
		m_Player->AddDirectionCurrentFrame(0, -1);
	}
	if (pStates[SDL_SCANCODE_RIGHT] or pStates[SDL_SCANCODE_D])
	{
		m_Player->AddDirectionCurrentFrame(1, 0);
	}
	if (pStates[SDL_SCANCODE_LEFT] or pStates[SDL_SCANCODE_A])
	{
		m_Player->AddDirectionCurrentFrame(-1, 0);
	}

}

std::pair<int, int> Game::GetRandomGridLocation(int const maxX, int const maxY)
{


	return std::pair<int, int>((rand() % (maxX - 1)) + 1, (rand() % (maxY - 1)) + 1);
}

void Game::GenerateNewGoal(float const wallSize)
{
	bool isNewGoalValid{ false };
	Rectf GoalShape{};

	while (not isNewGoalValid)
	{
		//generate new rect
		auto newGoalLocation{ GetRandomGridLocation(21, 21) };
		GoalShape = Rectf(static_cast<float>(newGoalLocation.first), static_cast<float>(newGoalLocation.second), 1, 1);

		//edit rect for collision reasons
		GoalShape = MakeGlobalRect(GoalShape, wallSize);

		GoalShape = ShrinkRect(GoalShape, 2);

		//check validity
		std::promise<bool> promiseBool;
		std::future<bool> futureBool = promiseBool.get_future();

		std::thread thread(&Game::IsRectValidPromise, this, std::move(promiseBool), GoalShape, true, 0, static_cast<int>(m_LevelWalls.size()), true, CheckingMode::both);

		thread.join();

		if (futureBool.get())
		{
			isNewGoalValid = true;
		}
	}

	m_GoalObject = std::make_unique<dae::LevelObject>(GoalShape, dae::LevelObjectTypes::Goal, true);
	m_GoalRadius = std::make_unique<dae::LevelObject>(Rectf(GoalShape.left	- 152, GoalShape.bottom - 152, 250, 250), dae::LevelObjectTypes::GoalSpawnRadius);
}

void Game::GenerateNewCoin(int const amountOfCoins)
{
	bool isNewCoinValid{ false };
	Ellipsef CoinShape{};
	
	for (int i = 0; i < amountOfCoins; i++)
	{
		while (not isNewCoinValid)
		{
			int const coinRadius{ 20 };

			//generate new rect
			auto newGoalLocation{ GetRandomGridLocation(18, 18) };
			CoinShape = Ellipsef(static_cast<float>(newGoalLocation.first) * (wallThickness), static_cast<float>(newGoalLocation.second) * (wallThickness), coinRadius, coinRadius);
			CoinShape.center.x += coinRadius;
			CoinShape.center.y += coinRadius;


			//check validity
			std::promise<bool> promiseBool;
			std::future<bool> futureBool = promiseBool.get_future();

			Rectf collisionShape{ static_cast<float>(newGoalLocation.first) * (wallThickness) - coinRadius, static_cast<float>(newGoalLocation.second) * (wallThickness) - coinRadius, 2 * coinRadius, 2 * coinRadius };

			std::thread thread(&Game::IsRectValidPromise, this, std::move(promiseBool), collisionShape, true, 0, static_cast<int>(m_LevelWalls.size()), true, CheckingMode::both);

			thread.join();

			if (futureBool.get())
			{
				isNewCoinValid = true;
			}
		}

		m_Coin.emplace_back(CoinShape, dae::LevelObjectTypes::Coin);
		isNewCoinValid = false;
	}

}

Rectf Game::MakeGlobalRect(const Rectf& rect, float const tileSide)
{
	Rectf result{ Rectf(rect.left * tileSide, rect.bottom * tileSide, rect.width * tileSide, rect.height * tileSide) };
	return result;
}

void Game::ResetWalls()
{
	for (auto& wall : m_LevelWalls)
	{
		wall.SetEnabled(true);
	}

	m_Coin.clear();
}

void Game::DisableOneWall()
{
	int const nrOfOuterWalls{ 4 }; //these are the first 4 walls in the vector and should be able to disable

	int const rndmID{ (rand() % static_cast<int>(m_LevelWalls.size() - nrOfOuterWalls)) + nrOfOuterWalls };
	m_LevelWalls[rndmID].SetEnabled(false);
}

void Game::IsRectValidPromise(std::promise<bool> promise, const Rectf& rect, bool const isRectAlreadyGlobal, int const startIndex, int const nrToCheck, bool isGenerating, CheckingMode checkingMode)
{
	thread_local Rectf globalRect{ rect };

	int const wallSize{ 50 };

	if (not isRectAlreadyGlobal)
	{
		globalRect = Rectf(rect.left * wallSize, rect.bottom * wallSize, rect.width * wallSize, rect.height * wallSize);
	}

	if (checkingMode == CheckingMode::dangerTiles or checkingMode == CheckingMode::both)
	{

		for (int i = 0; i < m_DangerTiles.size(); i++)
		{
			auto wallShape{ m_DangerTiles[i].GetShape(true)};

			if (utils::IsOverlapping(globalRect, wallShape) and m_DangerTiles[i].IsOn())
			{
				promise.set_value(false);
				return;
			}
		}
	}
	
	if (checkingMode == CheckingMode::walls or checkingMode == CheckingMode::both)
	{
		for (int i = startIndex; i < nrToCheck; i++)
		{
		
			if (m_LevelWalls[i].GetObjecttype() == dae::LevelObjectTypes::Wall)
			{
				auto wallShape{ m_LevelWalls[i].GetShape(true)};

				if (utils::IsOverlapping(globalRect, wallShape))
				{
					promise.set_value(false);
					return;
				}
			}
		}
	}

	if (isGenerating)
	{
		if (utils::IsOverlapping(globalRect, m_GoalRadius.get()->GetShape(false)))
		{
			promise.set_value(false);
			return;
		}
	}

	promise.set_value(true);
}

void Game::MakeNextFloor(bool addScore)
{
	m_Timer.SetTimer(0, 31);
	//m_Timer.SetTimer(0, 5);

	if (m_Timer.IsPaused())
	{
		m_Timer.TogglePause();
	}

	m_FloorsBeaten++;

	ResetWalls();

	GenerateNewDangerTile(wallThickness);

	GenerateNewCoin((rand() % 4)+1);

	DisableOneWall();

	GenerateNewGoal(wallThickness);

	Rectf PlayerShape{ MakeNewPlayerShape() };

	m_Player->ResetPosition(PlayerShape);
	if (addScore)
	{
		m_Timer.AddScore(1);
	}

}

Rectf Game::MakeNewPlayerShape()
{
	Rectf PlayerShape{};
	bool isNewPlayerPosValid{ false };

	while (not isNewPlayerPosValid)
	{
		auto newPlayerLocation{ GetRandomGridLocation(21, 21) };

		PlayerShape = Rectf(static_cast<float>(newPlayerLocation.first), static_cast<float>(newPlayerLocation.second), 0.5f, 0.5f);
		PlayerShape = MakeGlobalRect(PlayerShape);


		std::promise<bool> promiseBool;
		std::future<bool> futureBool = promiseBool.get_future();

		std::thread thread(&Game::IsRectValidPromise, this, std::move(promiseBool), PlayerShape, true, 0, static_cast<int>(m_LevelWalls.size()), true, CheckingMode::both);

		thread.join();

		if (futureBool.get())
		{
			isNewPlayerPosValid = true;
		}
	}

	return PlayerShape;
}

Rectf Game::ShrinkRect(const Rectf& rect, int const shrinkAmount)
{
	Rectf result{ rect };

	result.width	-= 2 *	shrinkAmount;
	result.height	-= 2 *	shrinkAmount;
	result.left		+=		shrinkAmount;
	result.bottom	+=		shrinkAmount;

	return result;
}

void Game::GenerateNewDangerTile(float const wallSize)
{
	bool  isNewDangerTileValid{ false };
	Rectf dangerTileShape{};

	while (not isNewDangerTileValid)
	{
		//generate new rect
		auto newTileLocation{ GetRandomGridLocation(18, 18) };
		dangerTileShape = Rectf(static_cast<float>(newTileLocation.first), static_cast<float>(newTileLocation.second), 1, 1);

		//edit rect for collision reasons
		dangerTileShape = MakeGlobalRect(dangerTileShape, wallSize);

		dangerTileShape = ShrinkRect(dangerTileShape, 2);
		
		//check validity
		std::promise<bool> promiseBool;
		std::future<bool> futureBool = promiseBool.get_future();

		std::thread thread(&Game::IsRectValidPromise, this, std::move(promiseBool), dangerTileShape, true, 0, static_cast<int>(m_LevelWalls.size()), true, CheckingMode::both);

		thread.join();

		if (futureBool.get())
		{
			isNewDangerTileValid = true;
		}
	}

	m_DangerTiles.emplace_back(dangerTileShape, true);

}

void Game::ResetGame()
{
	m_Timer.Reset();

	ResetWalls();
	m_Player.reset();
	m_GoalObject.reset();
	m_GoalRadius.reset();
	m_DangerTiles.clear();
	m_Coin.clear();

	Initialize(false);
}