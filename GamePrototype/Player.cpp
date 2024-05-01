#include "pch.h"
#include "Player.h"

dae::Player::Player() 
	: BaseObject()
	, m_MoveSpeed(250)
	, m_PlayerStartingShape(Rectf(75.0f, 75.0f, 25.0f, 25.0f))
{
}

dae::Player::Player(const Rectf& shape, const StateOfMatter& stateOfMatter, const Color4f& Color, const float moveSpeed)
	: BaseObject(shape, stateOfMatter, Color)
	, m_MoveSpeed(moveSpeed)
	, m_PlayerStartingShape(shape)
{
}

void dae::Player::Update(float)
{
	m_Shape.left += m_DirectionThisFrame.x;
	m_Shape.bottom += m_DirectionThisFrame.y;

	m_DirectionThisFrame = Vector2f(0.0f, 0.0f);
}


Point2f dae::Player::PredictPosition(float elapsedSec)
{
	m_DirectionThisFrame = m_DirectionThisFrame.Normalized();

	m_DirectionThisFrame.x = m_DirectionThisFrame.x * m_MoveSpeed * elapsedSec;
	m_DirectionThisFrame.y = m_DirectionThisFrame.y * m_MoveSpeed * elapsedSec;

	return Point2f(m_Shape.left + m_DirectionThisFrame.x, m_Shape.bottom + m_DirectionThisFrame.y);
}

void dae::Player::AddDirectionCurrentFrame(const Vector2f& direction)
{
	AddDirectionCurrentFrame(direction.x, direction.y);
}

void dae::Player::AddDirectionCurrentFrame(float const directionX, float const directionY)
{
	m_DirectionThisFrame.x += directionX;
	m_DirectionThisFrame.y += directionY;
}

void dae::Player::ResetPosition()
{
	m_Shape = m_PlayerStartingShape;
}