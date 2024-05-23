#include "pch.h"
#include "DangerTile.h"
#include <utils.h>

#define COOLDOWN_MIN 3.0f
#define COOLDOWN_MAX 4.0f
#define COOLDOWN_INTERVAL 0.1f
#define TURNING_ON_TIME 1.0f
#define TIME_ON 3.0f

dae::DangerTile::DangerTile(const Rectf& actualShape, bool hasSmallHitbox)
	: LevelObject(actualShape, LevelObjectTypes::DangerTile, hasSmallHitbox)
	, m_Border(actualShape)
	, m_DangerTileState(DangerTileState::Off)
	, m_Cooldown(0.0f)
	, m_CooldownMin(COOLDOWN_MIN)
	, m_CooldownMax(COOLDOWN_MAX)
	, m_CooldownInterval(COOLDOWN_INTERVAL)
	, m_Timer()
	, m_TurningOnTime(TURNING_ON_TIME)
	, m_TimeOn(TIME_ON)
{
	RandomizeCooldown(m_CooldownMin, m_CooldownMax, m_CooldownInterval);
}
dae::DangerTile::DangerTile(const Rectf& shapeInTiles, float const TileSide, bool hasSmallHitbox)
	: LevelObject(shapeInTiles, TileSide, LevelObjectTypes::DangerTile, hasSmallHitbox)
	, m_Border(MakeGlobalRect(shapeInTiles, TileSide))
	, m_DangerTileState(DangerTileState::Off)
	, m_Cooldown(0.0f)
	, m_CooldownMin(COOLDOWN_MIN)
	, m_CooldownMax(COOLDOWN_MAX)
	, m_CooldownInterval(COOLDOWN_INTERVAL)
	, m_Timer()
	, m_TurningOnTime(TURNING_ON_TIME)
	, m_TimeOn(TIME_ON)
{
	RandomizeCooldown(m_CooldownMin, m_CooldownMax, m_CooldownInterval);
}

void dae::DangerTile::Update(float elapsedSec)
{
	m_Timer -= elapsedSec;
	if (m_Timer <= 0.0f)
	{
		switch (m_DangerTileState)
		{
		case dae::DangerTileState::Off:
			m_DangerTileState = DangerTileState::TurningOn;
			SetColor(Color4f(1.0f, 0.89f, 0.53f, 1.0f));
			m_Timer = m_TurningOnTime;
			break;
		case dae::DangerTileState::TurningOn:
			m_DangerTileState = DangerTileState::On;
			SetColor(Color4f(0.96f, 0.40f, 0.32f, 1.0f));
			m_Timer = m_TimeOn;
			break;
		case dae::DangerTileState::On:
			m_DangerTileState = DangerTileState::Off;
			SetColor(Color4f(0.43f, 0.96f, 0.42f, 1.0f));
			m_Timer = m_Cooldown;
			break;
		}
	}

}

void dae::DangerTile::Draw() const
{
	utils::SetColor(m_SquareColor);
	if (IsOn())
	{
		utils::FillRect(m_Shape);
	}
	utils::DrawRect(m_Border, 5.0f);
}

bool dae::DangerTile::IsOn() const
{
	return (m_DangerTileState == DangerTileState::On);
}

void dae::DangerTile::RandomizeCooldown(float const min, float const max, float const interval)
{
	SetColor(Color4f(0.43f, 0.96f, 0.42f, 1.0f));
	int randMin{ static_cast<int>(min / interval) };
	int randMax{ static_cast<int>(max / interval) };

	m_Cooldown = static_cast<float>(((rand() % randMax) + randMin) * interval);
	m_Timer = m_Cooldown;	
}