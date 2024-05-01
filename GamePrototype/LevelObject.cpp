#include "pch.h"
#include "LevelObject.h"

dae::LevelObject::LevelObject()
	: BaseObject(MakeGlobalRect(Rectf(0, 0, 1, 1), 50), GetStateByObjectType(LevelObjectTypes::Wall), GetColorByObjectType(LevelObjectTypes::Wall))
	, m_RectInTiles(Rectf(0, 0, 1, 1))
	, m_ObjectType(LevelObjectTypes::Wall)
{
}

dae::LevelObject::LevelObject(const Rectf& shapeInTiles, float const tileSide, const LevelObjectTypes& objectType)
	: BaseObject(MakeGlobalRect(shapeInTiles, tileSide), GetStateByObjectType(objectType), GetColorByObjectType(objectType))
	, m_RectInTiles(shapeInTiles)
	, m_ObjectType(objectType)
{
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
	default:
		return StateOfMatter::solid;
	}
}

Color4f dae::LevelObject::GetColorByObjectType(const LevelObjectTypes& type)
{
	Color4f black{ Color4f(0.1f, 0.1f, 0.1f, 1.0f) };
	Color4f gold{ Color4f(0.855f, 0.647f, 0.008f, 1.0f) };

	switch (type)
	{
	case LevelObjectTypes::Wall:
		return black;
	case LevelObjectTypes::Goal:
		return gold;
	default:
		return black;
	}

}
