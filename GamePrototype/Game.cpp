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
	m_Player = std::make_unique<dae::Player>(Rectf(75.0f, 75.0f, 25.0f, 25.0f), dae::StateOfMatter::solid, Color4f(0.6f, 0.6f, 0.6f, 1.0f), 250.0f);
	InitializeLevel();
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	// Check keyboard state
	const Uint8 *pStates = SDL_GetKeyboardState( nullptr );

	if (pStates[SDL_SCANCODE_UP])
	{
		m_Player->AddDirectionCurrentFrame(0, 1);
	}
	if (pStates[SDL_SCANCODE_DOWN])
	{
		m_Player->AddDirectionCurrentFrame(0, -1);
	}
	if ( pStates[SDL_SCANCODE_RIGHT] )
	{
		m_Player->AddDirectionCurrentFrame(1, 0);
	}
	if ( pStates[SDL_SCANCODE_LEFT] )
	{
		m_Player->AddDirectionCurrentFrame(-1, 0);
	}

	auto const playerPredictedPos{ m_Player->PredictPosition(elapsedSec) };

	auto playerShape{ m_Player->GetShape() };
	playerShape.left = playerPredictedPos.x;
	playerShape.bottom = playerPredictedPos.y;

	std::promise<bool> promiseBool;
	std::future<bool> futureBool = promiseBool.get_future();

	std::thread thread(&Game::IsRectValidPromise, this, std::move(promiseBool), playerShape, true);

	//if (utils::IsOverlapping(playerShape, )
	//{

	//}

	thread.join();

	if (not IsRectValid(playerShape, true))
	{
		std::cout << "collision wall\n";
		m_Player->ResetDirectionThisFrame();

	}
	
	//todo: goal collision

	m_Player->Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );

	glPushMatrix();

	m_Camera->Transform(m_Player->GetShape(), false);

	utils::SetColor(Color4f(0.0f, 0.0f, 0.0f, 1.0f));
	for (auto& object : m_LevelWalls)
	{
		object->Draw();
	}
	
	utils::SetColor(Color4f(0.4f, 0.4f, 0.4f, 1.0f));
	m_Player->Draw();

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
	//float const canvasWidth	{ 1100 };
	//float const canvasHeight	{ 1100 };
	float const wallThickness	{ 50 };

	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 0,  0,  1, 22), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf(21,  0,  1, 22), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 0,  0, 22,  1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 0, 21, 22,  1), wallThickness, dae::LevelObjectTypes::Wall));
	
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 2,  4,  3,  1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 2,  5,  1,  2), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 2,  8,  5,  1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 6,  9,  1,  4), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 6,  6,  3,  1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 6,  2,  1,  4), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 7,  2,  4,  1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf(10,  4,  3,  3), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf(12,  7,  1,  5), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf(13,  9,  2,  1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf(14,  4,  3,  1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf(16,  1,  1,  3), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf(16,  6,  3,  1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf(16,  7,  1,  5), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf(16, 11,  5,  1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 8, 11,  4,  1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 8, 12,  1,  6), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 4, 17,  4,  1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 4, 11,  1,  6), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 1, 11,  3,  1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 2, 13,  1,  3), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf( 4, 19,  1,  2), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf(10, 17,  1,  4), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf(12, 17,  1,  3), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf(13, 17,  6,  1), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf(18, 18,  1,  2), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf(12, 13,  1,  3), wallThickness, dae::LevelObjectTypes::Wall));
	m_LevelWalls.emplace_back(std::make_unique<dae::LevelObject>(Rectf(12, 13,  6,  1), wallThickness, dae::LevelObjectTypes::Wall));
	
}

std::pair<int, int> Game::GetRandomGridLocation(int const maxX, int const maxY)
{
	return std::pair<int, int>((rand() % (maxX - 1)) + 1, (rand() % (maxY - 1)) + 1);
}

void Game::GenerateNewGoal(float const wallSize)
{
	//if (wall->GetObjecttype() == dae::LevelObjectTypes::Goal)
	//{
	//	int const outerBorder{ 2 };

	//	wallShape.width -= 2 * outerBorder;
	//	wallShape.height -= 2 * outerBorder;
	//	wallShape.left += outerBorder;
	//	wallShape.bottom += outerBorder;
	//}


	m_GoalObject = std::make_unique<dae::LevelObject>(Rectf(4, 3, 1, 1), wallSize, dae::LevelObjectTypes::Goal);
}

bool Game::IsRectValid(const Rectf& rect, bool const isRectAlreadyGlobal)
{
	Rectf globalRect{ rect };

	if (not isRectAlreadyGlobal)
	{
		globalRect = dae::LevelObject::MakeGlobalRect(rect);
	}

	auto wallIt = std::find_if(std::execution::par, m_LevelWalls.begin(), m_LevelWalls.end(), [&](const std::unique_ptr<dae::LevelObject>& wall) 
	{
		auto wallShape{ wall->GetShape() };

		if (utils::IsOverlapping(globalRect, wallShape))
		{
			return true;
		}

		return false;
	});

	if (wallIt != m_LevelWalls.end())
	{
		return false;
	}

	return true;
}

void Game::IsRectValidPromise(std::promise<bool> promise, const Rectf& rect, bool const isRectAlreadyGlobal)
{
	promise.set_value(IsRectValid(rect, isRectAlreadyGlobal));
}