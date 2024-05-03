#include "pch.h"
#include "LevelObject.h"
#include <algorithm>

dae::LevelObject::LevelObject()
	: BaseObject(MakeGlobalRect(Rectf(0, 0, 1, 1), 50), GetStateByObjectType(LevelObjectTypes::Wall), GetColorByObjectType(LevelObjectTypes::Wall))
	, m_ObjectType(LevelObjectTypes::Wall)
{
}

dae::LevelObject::LevelObject(const Rectf& actualShape, const LevelObjectTypes& objectType)
	: BaseObject(actualShape, GetStateByObjectType(objectType), GetColorByObjectType(objectType))
	, m_ObjectType(objectType)
{
}

dae::LevelObject::LevelObject(const Rectf& shapeInTiles, float const tileSide, const LevelObjectTypes& objectType)
	: BaseObject(MakeGlobalRect(shapeInTiles, tileSide), GetStateByObjectType(objectType), GetColorByObjectType(objectType))
	, m_ObjectType(objectType)
{
}

void dae::LevelObject::SetAlpha(float const alphaOffset)
{
	SetColor(m_SquareColor.r, m_SquareColor.g, m_SquareColor.b, m_SquareColor.a + alphaOffset);
	//m_SquareColor.a = std::clamp(m_SquareColor.a, 0.0f, 1.0f);
}

Rectf dae::LevelObject::MakeGlobalRect(const Rectf& rect, float const tileSide)
{
	return Rectf(rect.left * tileSide, rect.bottom * tileSide, rect.width * tileSide, rect.height * tileSide);
}

dae::StateOfMatter dae::LevelObject::GetStateByObjectType(const LevelObjectTypes& type)
{
	switch (type)
	{
	case LevelObjectTypes::Wall:
		return StateOfMatter::solid;
	case LevelObjectTypes::Goal:
		return StateOfMatter::nonSolid;
	case LevelObjectTypes::FadeEffect:
	case LevelObjectTypes::GoalSpawnRadius:
		return StateOfMatter::noCollision;
	default:
		return StateOfMatter::solid;
	}
}

Color4f dae::LevelObject::GetColorByObjectType(const LevelObjectTypes& type)
{
	Color4f black{ Color4f(0.1f, 0.1f, 0.1f, 1.0f) };
	Color4f blackFade{ Color4f(0.0f, 0.0f, 0.0f, 0.0f) };
	Color4f gold{ Color4f(0.855f, 0.647f, 0.008f, 1.0f) };

	switch (type)
	{
	case LevelObjectTypes::Wall:
		return black;
	case LevelObjectTypes::Goal:
		return gold;
	case LevelObjectTypes::FadeEffect:
	case LevelObjectTypes::GoalSpawnRadius:
		return blackFade;
	default:
		return black;
	}

}
