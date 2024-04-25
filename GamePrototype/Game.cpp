#include "pch.h"
#include "Game.h"
#include "utils.h"

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
	m_Player = std::make_unique<dae::Object>(dae::Object(Rectf(50, 50, 40, 40), 250, true));
	//m_LevelTexture = std::make_unique<Texture>("level.png");
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
		m_Player->AddPositionOffset(0, 1);
	}
	if (pStates[SDL_SCANCODE_DOWN])
	{
		m_Player->AddPositionOffset(0, -1);
	}
	if ( pStates[SDL_SCANCODE_RIGHT] )
	{
		m_Player->AddPositionOffset(1, 0);
	}
	if ( pStates[SDL_SCANCODE_LEFT] )
	{
		m_Player->AddPositionOffset(-1, 0);
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
	float const canvasWidth		{ 1000 };
	float const canvasHeight	{ 1000 };
	float const wallThickness	{ 50 };

	m_LevelObjects.emplace_back(std::make_unique<dae::Object>(Rectf(						  0,							0, wallThickness,	canvasHeight)));
	m_LevelObjects.emplace_back(std::make_unique<dae::Object>(Rectf(canvasWidth - wallThickness,							0, wallThickness,	canvasHeight)));
	m_LevelObjects.emplace_back(std::make_unique<dae::Object>(Rectf(						  0,							0, canvasWidth,		wallThickness)));
	m_LevelObjects.emplace_back(std::make_unique<dae::Object>(Rectf(						  0, canvasHeight - wallThickness, canvasWidth,		wallThickness)));
}