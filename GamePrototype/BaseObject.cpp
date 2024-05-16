#include "pch.h"
#include "BaseObject.h"
#include "utils.h"
#include <iostream>

dae::BaseObject::BaseObject()
	: m_Shape(Rectf(50, 50, 40, 40))
	, m_StateOfMatter(StateOfMatter::solid)
	, m_SquareColor(Color4f(1.0f, 1.0f, 1.0f, 1.0f))
	, m_IsEnabled(true)
	, m_HasSmallHitBox(false)
{
}

dae::BaseObject::BaseObject(const Rectf& shape, const StateOfMatter& stateOfMatter, const Color4f& Color, bool const isEnabled, bool hasSmallHitbox)
	: m_Shape(shape)
	, m_StateOfMatter(stateOfMatter)
	, m_SquareColor(Color)
	, m_IsEnabled(isEnabled)
	, m_HasSmallHitBox(hasSmallHitbox)
{
}

void dae::BaseObject::Update(float)
{
}

void dae::BaseObject::Draw() const
{
	utils::SetColor(m_SquareColor);
	utils::FillRect(m_Shape);
}

void dae::BaseObject::SetColor(const Color4f& newColor)
{
	SetColor(newColor.r, newColor.g, newColor.b, newColor.a);
}

void dae::BaseObject::SetColor(float const newR, float const newG, float const newB, float const newA)
{
	m_SquareColor.r = newR;
	m_SquareColor.g = newG;
	m_SquareColor.b = newB;
	m_SquareColor.a = newA;
}

Rectf dae::BaseObject::GetShape(bool isCollisionRect) const
{
	if (m_IsEnabled)
	{
		if (m_HasSmallHitBox /*and isCollisionRect*/)
		{
			int const offset{ 15 };

			Rectf result{ m_Shape };

			result.left		+= offset;
			result.bottom	+= offset;
			result.width	-= 2 * offset;
			result.height	-= 2 * offset;
			return result;
		}
		return m_Shape;
	}
	return Rectf();
}
