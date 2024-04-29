#pragma once
#include "BaseGame.h"
#include "Vector2f.h"
#include "Camera.h"
#include "BaseObject.h"
#include <memory>
#include <Texture.h>
#include "Player.h"

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void InitializeLevel();



	Rectf CalculateOverlap(const Rectf& r1, const Rectf& r2);

	std::unique_ptr<Camera> m_Camera;
	std::unique_ptr<dae::Player> m_Player;
	std::vector<std::unique_ptr<dae::BaseObject>> m_LevelObjects;
};