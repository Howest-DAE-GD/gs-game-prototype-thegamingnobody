#include "pch.h"
#include "Player.h"
#include <iostream>

dae::Player::Player() 
	: BaseObject()
	, m_MoveSpeed(250)
	, m_PlayerStartingShape(Rectf(75.0f, 75.0f, 25.0f, 25.0f))
	, m_IsAlive(true)
{
}

dae::Player::Player(const Rectf& shape, const StateOfMatter& stateOfMatter, const Color4f& Color, const float moveSpeed)
	: BaseObject(shape, stateOfMatter, Color)
	, m_MoveSpeed(moveSpeed)
	, m_PlayerStartingShape(shape)
	, m_IsAlive(true)
{
}

void dae::Player::Update(float)
{
	if (m_IsAlive)
	{
		m_Shape.left += m_DirectionThisFrame.x;
		m_Shape.bottom += m_DirectionThisFrame.y;

		m_DirectionThisFrame = Vector2f(0.0f, 0.0f);
	}
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

void dae::Player::ResetPosition(const Rectf& shape)
{
	if (shape.width > 0 and shape.height > 0)
	{
		m_Shape = shape;
		
	}
	else
	{
		m_Shape = m_PlayerStartingShape;
	}
}

void dae::Player::MarkDie()
{
	if (m_IsAlive)
	{
		std::cout << "You Died!\n\n\n\n\n\n";
		m_IsAlive = false;
		m_SquareColor = Color4f(0.70f, 0.70f, 0.70f, 1.0f);
	}
}