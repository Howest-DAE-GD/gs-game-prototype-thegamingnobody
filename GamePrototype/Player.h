#pragma once
#include "BaseObject.h"

namespace dae
{
	class Player final : public BaseObject
	{
	public:
		Player();
		Player(const Rectf& shape, const StateOfMatter& stateOfMatter = StateOfMatter::solid, const Color4f& Color = Color4f(1.0f, 1.0f, 1.0f, 1.0f), const float moveSpeed = 250);

		void Update(float elapsedSec) override;

		Point2f PredictPosition(float elapsedSec);

		void AddDirectionCurrentFrame(const Vector2f& direction);
		void AddDirectionCurrentFrame(float const directionX, float const directionY);

		float GetMoveSpeed() const { return m_MoveSpeed; }

		void ResetDirectionThisFrame() { m_DirectionThisFrame = Vector2f(0, 0); }

	private:
		float const m_MoveSpeed{};

		Vector2f m_DirectionThisFrame{};
	};
}