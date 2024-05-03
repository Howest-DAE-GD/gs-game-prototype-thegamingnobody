#pragma once
#include "BaseGame.h"
#include "Vector2f.h"
#include "Camera.h"
#include "BaseObject.h"
#include <memory>
#include <Texture.h>
#include "Player.h"
#include "LevelObject.h"
#include <future>
#include <mutex>

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

	void HandleInput(float elapsedSec);

	std::pair<int, int> GetRandomGridLocation(int const maxX, int const maxY);
	void GenerateNewGoal(float const wallSize);
	void IsRectValidPromise(std::promise<bool> promise, const Rectf& rect, bool const isRectAlreadyGlobal, int const startIndex, int const nrToCheck, bool isGenerating);

	Rectf MakeGlobalRect(const Rectf& rect, float const tileSide = 50.0f);

	void MakeNextFloor();

	Rectf MakeNewPlayerShape();

	std::unique_ptr<Camera> m_Camera;
	std::unique_ptr<dae::Player> m_Player;
	std::vector<dae::LevelObject> m_LevelWalls;
	std::unique_ptr<dae::LevelObject> m_GoalObject;
	std::unique_ptr<dae::LevelObject> m_GoalRadius;

	float const wallThickness{ 50 };
	int m_PlayerScore{ 0 };

	std::mutex m_WallsMutex;

	enum class ScreenfadeStage
	{
		NoFade,
		FadingOut,
		FadingIn
	};

	ScreenfadeStage m_FadingStage{ ScreenfadeStage::NoFade };
	float m_FadeAlpha{};
};