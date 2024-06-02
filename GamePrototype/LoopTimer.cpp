#include "pch.h"
#include "LoopTimer.h"


LoopTimer::LoopTimer() 
	: m_NumberTexturesBorder()
	, m_NumberTexturesInner()
	, m_TimerTextureBorder()
	, m_TimerTextureInner()
	, m_ScoreTextureBorder()
	, m_ScoreTextureInner()
	, m_FontBorder()
	, m_FontInner()
	, m_FontWidth()
	, m_FontHeight()
	, m_CountDown()
	, m_StartPos(10.0f, 10.0f)
	, m_TimerPaused(false)
{
	std::string fontName{ "m04.TTF" };
	m_FontBorder = TTF_OpenFont(fontName.c_str(), 20);
	std::string fontName2{ "m04b.TTF" };
	m_FontInner = TTF_OpenFont(fontName2.c_str(), 20);

	for (int i = 0; i < 10; i++)
	{
		m_NumberTexturesBorder.emplace_back(Texture(std::to_string(i), m_FontBorder, Color4f(0.0f, 0.0f, 0.0f, 1.0f)));
		m_NumberTexturesInner.emplace_back(Texture(std::to_string(i), m_FontInner, Color4f(1.0f, 1.0f, 1.0f, 1.0f)));
	}
	m_TimerTextureBorder = std::make_unique<Texture>("TIME: ", m_FontBorder, Color4f(0.0f, 0.0f, 0.0f, 1.0f));
	m_TimerTextureInner =  std::make_unique<Texture>("TIME: ", m_FontInner, Color4f(1.0f, 1.0f, 1.0f, 1.0f));
	m_ScoreTextureBorder = std::make_unique<Texture>("SCORE: ", m_FontBorder, Color4f(0.0f, 0.0f, 0.0f, 1.0f));
	m_ScoreTextureInner =  std::make_unique<Texture>("SCORE: ", m_FontInner, Color4f(1.0f, 1.0f, 1.0f, 1.0f));

	std::string fontName3{ "m04.TTF" };
	m_FontBorder = TTF_OpenFont(fontName3.c_str(), 15);
	std::string fontName4{ "m04b.TTF" };
	m_FontInner = TTF_OpenFont(fontName4.c_str(), 15);
	m_NumberTexturesBorder.emplace_back(Texture(".", m_FontBorder, Color4f(0.0f, 0.0f, 0.0f, 1.0f)));
	m_NumberTexturesInner.emplace_back(Texture(".", m_FontInner, Color4f(1.0f, 1.0f, 1.0f, 1.0f)));

	m_FontWidth = m_NumberTexturesBorder[0].GetWidth();
	m_FontHeight = m_NumberTexturesBorder[0].GetHeight();

}

LoopTimer::~LoopTimer()
{
	TTF_CloseFont(m_FontBorder);
	TTF_CloseFont(m_FontInner);
}

void LoopTimer::Update(float elapsedSec)
{
	float elapsedMiliSec{ elapsedSec * 1000 };

	if (not m_PlayerDied)
	{
		m_CountDown -= elapsedMiliSec;
	}

}

void LoopTimer::Draw() const
{
	auto countDownInSec{ (m_CountDown / 1000) };

	int numberMinute{ static_cast<int>(countDownInSec / 60) % 10 };
	countDownInSec -= (numberMinute * 60);
	int numberDeciSec{ static_cast<int>(countDownInSec / 10) % 10 };
	countDownInSec -= (numberDeciSec * 10);
	int numberSec{ static_cast<int>(countDownInSec) % 10 };

	if (numberSec < 0)
	{
		numberSec = 0;
	}

	 m_TimerTextureInner->Draw(Rectf(m_StartPos.x + (0 * m_FontWidth), m_StartPos.y, m_TimerTextureInner->GetWidth(), m_FontHeight));
	m_TimerTextureBorder->Draw(Rectf(m_StartPos.x + (0 * m_FontWidth), m_StartPos.y, m_TimerTextureBorder->GetWidth(), m_FontHeight));

	auto tempStartPos{ m_StartPos };
	tempStartPos.x += m_TimerTextureBorder->GetWidth();

	  m_NumberTexturesInner[numberMinute].Draw(Rectf(tempStartPos.x + (0 * m_FontWidth), tempStartPos.y, m_FontWidth, m_FontHeight));
	 m_NumberTexturesBorder[numberMinute].Draw(Rectf(tempStartPos.x + (0 * m_FontWidth), tempStartPos.y, m_FontWidth, m_FontHeight));

	  m_NumberTexturesInner[10].Draw(Rectf(tempStartPos.x + (1 * m_FontWidth), tempStartPos.y - 2, m_FontWidth, m_FontHeight));
	 m_NumberTexturesBorder[10].Draw(Rectf(tempStartPos.x + (1 * m_FontWidth), tempStartPos.y - 2, m_FontWidth, m_FontHeight));
	  m_NumberTexturesInner[10].Draw(Rectf(tempStartPos.x + (1 * m_FontWidth), tempStartPos.y + m_FontHeight - 10 - 2, m_FontWidth, m_FontHeight));
	 m_NumberTexturesBorder[10].Draw(Rectf(tempStartPos.x + (1 * m_FontWidth), tempStartPos.y + m_FontHeight - 10 - 2, m_FontWidth, m_FontHeight));

	  m_NumberTexturesInner[numberDeciSec].Draw(Rectf(tempStartPos.x + (2 * m_FontWidth), tempStartPos.y, m_FontWidth, m_FontHeight));
	 m_NumberTexturesBorder[numberDeciSec].Draw(Rectf(tempStartPos.x + (2 * m_FontWidth), tempStartPos.y, m_FontWidth, m_FontHeight));

	  m_NumberTexturesInner[numberSec].Draw(Rectf(tempStartPos.x + (3 * m_FontWidth), tempStartPos.y, m_FontWidth, m_FontHeight));
	 m_NumberTexturesBorder[numberSec].Draw(Rectf(tempStartPos.x + (3 * m_FontWidth), tempStartPos.y, m_FontWidth, m_FontHeight));

	 tempStartPos.y += m_TimerTextureBorder->GetHeight() + 2;

	  m_ScoreTextureInner->Draw(Rectf(m_StartPos.x + (0 * m_FontWidth), tempStartPos.y, m_ScoreTextureInner->GetWidth(), m_FontHeight));
	 m_ScoreTextureBorder->Draw(Rectf(m_StartPos.x + (0 * m_FontWidth), tempStartPos.y, m_ScoreTextureBorder->GetWidth(), m_FontHeight));

	 auto tempScore{ m_PlayerScore };
	 int scoreHundreds{ static_cast<int>(tempScore / 100) % 10 };
	 tempScore -= (scoreHundreds * 100);
	 int scoreTens{ static_cast<int>(tempScore / 10) % 10 };
	 tempScore -= (scoreTens * 10);
	 int scoreOnes{ static_cast<int>(tempScore) % 10 };
	 tempScore -= (scoreOnes);

	  m_NumberTexturesInner[scoreHundreds].Draw(Rectf(tempStartPos.x + (1 * m_FontWidth), tempStartPos.y, m_FontWidth, m_FontHeight));
	 m_NumberTexturesBorder[scoreHundreds].Draw(Rectf(tempStartPos.x + (1 * m_FontWidth), tempStartPos.y, m_FontWidth, m_FontHeight));
	  m_NumberTexturesInner[scoreTens].Draw(Rectf(tempStartPos.x + (2 * m_FontWidth), tempStartPos.y, m_FontWidth, m_FontHeight));
	 m_NumberTexturesBorder[scoreTens].Draw(Rectf(tempStartPos.x + (2 * m_FontWidth), tempStartPos.y, m_FontWidth, m_FontHeight));
	  m_NumberTexturesInner[scoreOnes].Draw(Rectf(tempStartPos.x + (3 * m_FontWidth), tempStartPos.y, m_FontWidth, m_FontHeight));
	 m_NumberTexturesBorder[scoreOnes].Draw(Rectf(tempStartPos.x + (3 * m_FontWidth), tempStartPos.y, m_FontWidth, m_FontHeight));
}

void LoopTimer::SetTimer(float minutes, float seconds)
{
	m_CountDown = 0;
	m_CountDown += (minutes * 60) * 1000;
	m_CountDown += seconds * 1000;
}

void LoopTimer::TogglePause()
{
	m_TimerPaused = (not m_TimerPaused);
}

bool LoopTimer::IsPaused() const
{
	return m_TimerPaused;
}

bool LoopTimer::HasTimerEnded() const
{
	return ((m_CountDown / 1000) <= 0);
}

void LoopTimer::AddScore(int const scoreToAdd)
{
	m_PlayerScore += scoreToAdd;
}

void LoopTimer::Reset()
{
	m_PlayerScore = 0;
}
