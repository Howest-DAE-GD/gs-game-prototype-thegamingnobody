#pragma once
#include "BaseObject.h"

namespace dae
{
	enum class LevelObjectTypes
	{
		Wall,
		Goal,
		FadeEffect,
		GoalSpawnRadius
	};


	class LevelObject : public BaseObject
	{
	public:
		LevelObject();
		LevelObject(const Rectf& actualShape, const LevelObjectTypes& objectType = LevelObjectTypes::Wall);
		LevelObject(const Rectf& shapeInTiles, float const TileSide, const LevelObjectTypes& objectType = LevelObjectTypes::Wall);

		LevelObject(const LevelObject& other) = default;
		LevelObject& operator=(const LevelObject& other) = default;
		LevelObject(LevelObject&& other) = default;
		LevelObject& operator=(LevelObject&& other) = default;

		void SetAlpha(float const alphaOffset);

		LevelObjectTypes GetObjecttype() const { return m_ObjectType; }

		Rectf MakeGlobalRect(const Rectf& rect, float const tileSide = 50.0f);

	private:
		StateOfMatter GetStateByObjectType(const LevelObjectTypes& type);
		Color4f GetColorByObjectType(const LevelObjectTypes& type);

		LevelObjectTypes m_ObjectType;
	};
}