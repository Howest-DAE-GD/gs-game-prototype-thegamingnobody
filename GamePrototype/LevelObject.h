#pragma once
#include "BaseObject.h"

namespace dae
{
	enum class LevelObjectTypes
	{
		Wall,
		Goal,
		FadeEffect,
		GoalSpawnRadius,
		DangerTile,
		Coin
	};


	class LevelObject : public BaseObject
	{
	public:
		LevelObject();
		LevelObject(const Rectf& actualShape, const LevelObjectTypes& objectType = LevelObjectTypes::Wall, bool hasSmallHitbox = false);
		LevelObject(const Rectf& shapeInTiles, float const TileSide, const LevelObjectTypes& objectType = LevelObjectTypes::Wall, bool hasSmallHitbox = false);
		LevelObject(const Ellipsef& coinShape, const LevelObjectTypes& objectType = LevelObjectTypes::Coin);

		LevelObject(const LevelObject& other) = default;
		LevelObject& operator=(const LevelObject& other) = default;
		LevelObject(LevelObject&& other) = default;
		LevelObject& operator=(LevelObject&& other) = default;

		void DrawCoin() const;

		void SetAlpha(float const alphaOffset);

		LevelObjectTypes GetObjecttype() const { return m_ObjectType; }

		Rectf MakeGlobalRect(const Rectf& rect, float const tileSide = 50.0f);

		Ellipsef GetCoinShape() const { return m_CoinShape; }

	private:
		StateOfMatter GetStateByObjectType(const LevelObjectTypes& type);
		Color4f GetColorByObjectType(const LevelObjectTypes& type);

		LevelObjectTypes m_ObjectType;

		Ellipsef m_CoinShape;

		
	};
}