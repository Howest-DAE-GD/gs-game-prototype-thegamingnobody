#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <iostream>
#include <chrono>

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
	m_Player = std::make_unique<dae::Player>(Rectf(75, 75, 25, 25), dae::StateOfMatter::solid, Color4f(0.6f, 0.6f, 0.6f, 1.0f), 250);
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

	for (auto& wall : m_LevelObjects)
	{
		auto const wallShape{ wall->GetShape() };

		if (utils::IsOverlapping(playerShape, wallShape))
		{
			switch (wall->GetStateOfMatter())
			{
			case dae::StateOfMatter::solid:
				std::cout << "collision solid\n";
				m_Player->ResetDirectionThisFrame();
				break;
			case dae::StateOfMatter::nonSolid:
				std::cout << "collision non solid\n";
				break;
			case dae::StateOfMatter::noCollision:
				break;
			}
		}
	}

	m_Player->Update(elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );

	glPushMatrix();



	m_Camera->Transform(m_Player->GetShape(), false);

	//m_LevelTexture->Draw();

	utils::SetColor(Color4f(0.0f, 0.0f, 0.0f, 1.0f));
	for (auto& object : m_LevelObjects)
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
	float const canvasWidth		{ 1100 };
	float const canvasHeight	{ 1100 };
	float const wallThickness	{ 50 };

	Color4f black{ Color4f(0.1f, 0.1f, 0.1f, 1.0f) };
	Color4f gold{ Color4f(0.855f, 0.647f, 0.008f, 1.0f) };

	m_LevelObjects.emplace_back(std::make_unique<dae::BaseObject>(Rectf(						  0,							0, wallThickness,	 canvasHeight),    dae::StateOfMatter::solid, black));
	m_LevelObjects.emplace_back(std::make_unique<dae::BaseObject>(Rectf(canvasWidth - wallThickness,							0, wallThickness,	 canvasHeight),    dae::StateOfMatter::solid, black));
	m_LevelObjects.emplace_back(std::make_unique<dae::BaseObject>(Rectf(						  0,							0,	 canvasWidth,	wallThickness),    dae::StateOfMatter::solid, black));
	m_LevelObjects.emplace_back(std::make_unique<dae::BaseObject>(Rectf(						  0, canvasHeight - wallThickness,	 canvasWidth,	wallThickness),    dae::StateOfMatter::solid, black));
	
	m_LevelObjects.emplace_back(std::make_unique<dae::BaseObject>(Rectf(wallThickness * 2, wallThickness * 4, wallThickness * 3, wallThickness * 1), dae::StateOfMatter::solid, black));
	m_LevelObjects.emplace_back(std::make_unique<dae::BaseObject>(Rectf(wallThickness * 2, wallThickness * 5, wallThickness * 1, wallThickness * 2), dae::StateOfMatter::solid, black));
	m_LevelObjects.emplace_back(std::make_unique<dae::BaseObject>(Rectf(wallThickness * 2, wallThickness * 8, wallThickness * 5, wallThickness * 1), dae::StateOfMatter::solid, black));
	m_LevelObjects.emplace_back(std::make_unique<dae::BaseObject>(Rectf(wallThickness * 6, wallThickness * 9, wallThickness * 1, wallThickness * 4), dae::StateOfMatter::solid, black));
	m_LevelObjects.emplace_back(std::make_unique<dae::BaseObject>(Rectf(wallThickness * 6, wallThickness * 6, wallThickness * 3, wallThickness * 1), dae::StateOfMatter::solid, black));
	m_LevelObjects.emplace_back(std::make_unique<dae::BaseObject>(Rectf(wallThickness * 6, wallThickness * 2, wallThickness * 1, wallThickness * 4), dae::StateOfMatter::solid, black));
	
	//m_LevelObjects.emplace_back(std::make_unique<dae::BaseObject>(Rectf(						200,						  200, wallThickness,	wallThickness), dae::StateOfMatter::nonSolid, gold));
	
	
}

Rectf Game::CalculateOverlap(const Rectf& r1, const Rectf& r2)
{
	float const left	{ std::max(r1.left, r2.left) };
	float const bottom	{ std::max(r1.bottom, r2.bottom) };
	float const right	{ std::min(r1.left + r1.width, r2.left + r2.width) };
	float const top		{ std::min(r1.bottom + r1.height, r2.bottom + r2.height) };

	return Rectf(left, bottom, right - left, top - bottom);
}
