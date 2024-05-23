#pragma once
#include "LevelObject.h"

namespace dae
{
	enum class DangerTileState
	{
		Off,
		TurningOn,
		On
	};

	class DangerTile : public LevelObject
	{
	public:
		DangerTile(const Rectf& actualShape, bool hasSmallHitbox = false);
		DangerTile(const Rectf& shapeInTiles, float const TileSide, bool hasSmallHitbox = false);

		void Update(float elapsedSec) override;
		void Draw() const override;

		bool IsOn() const;

	private:
		void RandomizeCooldown(float const min, float const max, float const interval);

		Rectf m_Border;
		DangerTileState m_DangerTileState;
		float m_Cooldown;
		float const m_CooldownMin;
		float const m_CooldownMax;
		float const m_CooldownInterval;

		float m_Timer;

		float const m_TurningOnTime;
		float const m_TimeOn;
	};
}