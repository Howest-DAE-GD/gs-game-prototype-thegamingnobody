#pragma once
#include <vector>
#include <Texture.h>
#include <memory>

class LoopTimer
{
public:
	LoopTimer();
	~LoopTimer();

	void Update(float elapsedSec);
	void Draw() const;

	void SetTimer(float minutes, float seconds);
	void TogglePause();

	bool IsPaused() const;

	bool HasTimerEnded() const;

	void AddScore(int const scoreToAdd);

	void Reset();

private:
	std::vector<Texture> m_NumberTexturesBorder;
	std::vector<Texture> m_NumberTexturesInner;
	std::unique_ptr<Texture> m_TimerTextureBorder;
	std::unique_ptr<Texture> m_TimerTextureInner;
	std::unique_ptr<Texture> m_ScoreTextureBorder;
	std::unique_ptr<Texture> m_ScoreTextureInner;

	TTF_Font* m_FontBorder;
	TTF_Font* m_FontInner;

	int m_FontWidth;
	int m_FontHeight;

	float m_CountDown;

	Point2f m_StartPos;
	
	bool m_TimerPaused;

	int m_PlayerScore{ -1 };
};