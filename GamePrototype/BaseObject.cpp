#include "pch.h"
#include "BaseObject.h"
#include "utils.h"

dae::BaseObject::BaseObject()
	: m_Shape(Rectf(50, 50, 40, 40))
	, m_StateOfMatter(StateOfMatter::solid)
	, m_SquareColor(Color4f(1.0f, 1.0f, 1.0f, 1.0f))
{
}

dae::BaseObject::BaseObject(const Rectf& shape, const StateOfMatter& stateOfMatter, const Color4f& Color)
	: m_Shape(shape)
	, m_StateOfMatter(stateOfMatter)
	, m_SquareColor(Color)
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