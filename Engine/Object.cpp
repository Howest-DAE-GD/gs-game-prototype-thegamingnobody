#include "Object.h"
#include "utils.h"

dae::Object::Object() 
	: m_Shape(Rectf(50, 50, 40, 40))
	, m_MoveSpeed(250)
	, m_CanMove(true)
	, m_DirectionThisFrame()
	, m_StateOfMatter(StateOfMatter::solid)
{
}

dae::Object::Object(const Rectf& shape, const StateOfMatter& stateOfMatter, float const moveSpeed)
	: m_Shape(shape)
	, m_MoveSpeed(moveSpeed)
	, m_CanMove(true)
	, m_DirectionThisFrame()
	, m_StateOfMatter(stateOfMatter)
{
}

void dae::Object::Update(float)
{
	m_Shape.left	+= m_DirectionThisFrame.x;
	m_Shape.bottom	+= m_DirectionThisFrame.y;

	m_DirectionThisFrame = Vector2f(0.0f, 0.0f);
}

Point2f dae::Object::PredictPosition(float elapsedSec)
{
	m_DirectionThisFrame = m_DirectionThisFrame.Normalized();

	m_DirectionThisFrame.x = m_DirectionThisFrame.x * m_MoveSpeed * elapsedSec;
	m_DirectionThisFrame.y = m_DirectionThisFrame.y * m_MoveSpeed * elapsedSec;


	return Point2f(m_Shape.left + m_DirectionThisFrame.x, m_Shape.bottom + m_DirectionThisFrame.y);
}

void dae::Object::Draw() const
{
	utils::FillRect(m_Shape);
}

void dae::Object::AddDirectionCurrentFrame(const Vector2f& direction)
{
	AddDirectionCurrentFrame(direction.x, direction.y);
}

void dae::Object::AddDirectionCurrentFrame(float const directionX, float const directionY)
{
	if (m_CanMove)
	{
		m_DirectionThisFrame.x += directionX;
		m_DirectionThisFrame.y += directionY;
	}
}

