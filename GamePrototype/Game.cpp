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

void Game::Initialize( )
{
	m_Camera = std::make_unique<Camera>(Camera(GetViewPort().width, GetViewPort().height));
	auto playerShape{ Rectf( 75.0f, 75.0f, 25.0f, 25.0f )};
	m_Player = std::make_unique<dae::Player>(playerShape, dae::StateOfMatter::solid, Color4f(0.6f, 0.6f, 0.6f, 1.0f), 300.0f);
	m_GoalRadius = std::make_unique<dae::LevelObject>(Rectf(0, 0, 1, 1), dae::LevelObjectTypes::GoalSpawnRadius);
	InitializeLevel();
	std::cout	<< "Find the exit of this floor!\n"
				<< "Score: 0\n";

}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	bool goalNotReached{ true }; //if reached this frame

	HandleInput(elapsedSec);

	//make predicted player shape
	auto const playerPredictedPos{ m_Player->PredictPosition(elapsedSec) };

	auto playerShape{ m_Player->GetShape() };
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
		threads.emplace_back(&Game::IsRectValidPromise, this, std::move(promiseBool), playerShape, true, i * nrPerThread, nrPerThread, false);
	}

	threads.clear();

	if (utils::IsOverlapping(playerShape, m_GoalObject->GetShape()))
	{
		if (m_FadingStage == ScreenfadeStage::NoFade)
		{
			m_FadingStage = ScreenfadeStage::FadingIn;
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

}

void Game::Draw( ) const
{
	ClearBackground( );

	glPushMatrix();

	m_Camera->Transform(m_Player->GetShape(), false);

	m_GoalObject->Draw();
	
	utils::SetColor(Color4f(0.4f, 0.4f, 0.4f, 1.0f));
	m_Player->Draw();

	for (auto& object : m_LevelWalls)
	{
		utils::SetColor(object.GetColor());
		object.Draw();
	}


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
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	m_Position.x -= 1;
	//	break;
	//case SDLK_RIGHT:
	//	m_Position.x += 1;
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
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

		int const outerBorder{ 2 };
		GoalShape.width -= 2 * outerBorder;
		GoalShape.height -= 2 * outerBorder;
		GoalShape.left += outerBorder;
		GoalShape.bottom += outerBorder;

		//check validity
		std::promise<bool> promiseBool;
		std::future<bool> futureBool = promiseBool.get_future();

		std::thread thread(&Game::IsRectValidPromise, this, std::move(promiseBool), GoalShape, true, 0, static_cast<int>(m_LevelWalls.size()), true);

		thread.join();

		if (futureBool.get())
		{
			isNewGoalValid = true;
		}
	}

	m_GoalObject = std::make_unique<dae::LevelObject>(GoalShape, dae::LevelObjectTypes::Goal);
	m_GoalRadius = std::make_unique<dae::LevelObject>(Rectf(GoalShape.left	- 152, GoalShape.bottom - 152, 250, 250), dae::LevelObjectTypes::GoalSpawnRadius);
}

Rectf Game::MakeGlobalRect(const Rectf& rect, float const tileSide)
{
	Rectf result{ Rectf(rect.left * tileSide, rect.bottom * tileSide, rect.width * tileSide, rect.height * tileSide) };
	return result;
}

void Game::IsRectValidPromise(std::promise<bool> promise, const Rectf& rect, bool const isRectAlreadyGlobal, int const startIndex, int const nrToCheck, bool isGenerating)
{
	thread_local Rectf globalRect{ rect };

	int const wallSize{ 50 };

	if (not isRectAlreadyGlobal)
	{
		globalRect = Rectf(rect.left * wallSize, rect.bottom * wallSize, rect.width * wallSize, rect.height * wallSize);
	}

	for (int i = startIndex; i < nrToCheck; i++)
	{
		
		if (m_LevelWalls[i].GetObjecttype() == dae::LevelObjectTypes::Wall)
		{
			auto wallShape{ m_LevelWalls[i].GetShape()};

			if (utils::IsOverlapping(globalRect, wallShape))
			{
				promise.set_value(false);
				return;
			}
		}
	}
	if (isGenerating)
	{
		if (utils::IsOverlapping(globalRect, m_GoalRadius.get()->GetShape()))
		{
			promise.set_value(false);
			return;
		}
	}

	promise.set_value(true);
}

void Game::MakeNextFloor()
{
	GenerateNewGoal(wallThickness);

	Rectf PlayerShape{ MakeNewPlayerShape() };

	m_Player->ResetPosition(PlayerShape);
	system("cls");
	std::cout	<< "Find the exit of this floor!\n" 
				<< "Score: " << ++m_PlayerScore << "\n";

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

		std::thread thread(&Game::IsRectValidPromise, this, std::move(promiseBool), PlayerShape, true, 0, static_cast<int>(m_LevelWalls.size()), true);

		thread.join();

		if (futureBool.get())
		{
			isNewPlayerPosValid = true;
		}
	}

	return PlayerShape;
}
