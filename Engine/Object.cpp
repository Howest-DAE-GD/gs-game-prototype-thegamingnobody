#include "Object.h"
#include "utils.h"

dae::Object::Object() 
	: m_Shape(Rectf(50, 50, 40, 40))
	, m_MoveSpeed(250)
	, m_CanMove(true)
{
}

dae::Object::Object(const Rectf& shape, float const moveSpeed, bool const canMove)
	: m_Shape(shape)
	, m_MoveSpeed(moveSpeed)
	, m_CanMove(canMove)
{
}

void dae::Object::Update(float elapsedSec)
{
	m_DirectionThisFrame = m_DirectionThisFrame.Normalized();

	m_Shape.left	+= m_DirectionThisFrame.x * m_MoveSpeed * elapsedSec;
	m_Shape.bottom	+= m_DirectionThisFrame.y * m_MoveSpeed * elapsedSec;

	m_DirectionThisFrame = Vector2f(0.0f, 0.0f);
}

void dae::Object::Draw() const
{
	utils::FillRect(m_Shape);
}

void dae::Object::AddPositionOffset(const Vector2f& direction)
{
	AddPositionOffset(direction.x, direction.y);
}

void dae::Object::AddPositionOffset(float const directionX, float const directionY)
{
	if (m_CanMove)
	{
		m_DirectionThisFrame.x += directionX;
		m_DirectionThisFrame.y += directionY;
	}
}
