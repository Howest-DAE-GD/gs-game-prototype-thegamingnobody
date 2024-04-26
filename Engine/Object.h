#pragma once
#include "structs.h"
#include "Vector2f.h"

namespace dae
{
	enum class StateOfMatter
	{
		solid, //detects collision, player can't move onto object
		nonSolid, //detects collision, player is able to move onto object
		noCollision //no collision detection
	};

	class Object
	{
	public:
		Object();
		Object(const Rectf& shape, const StateOfMatter& stateOfMatter = StateOfMatter::solid, float const moveSpeed = 0);

		virtual void Update( float elapsedSec );
		virtual void Draw() const;

		void AddDirectionCurrentFrame(const Vector2f& direction);
		void AddDirectionCurrentFrame(float const directionX, float const directionY);

		Point2f PredictPosition(float elapsedSec);

		float GetMoveSpeed() const { return m_MoveSpeed; }

		Rectf GetShape() { return m_Shape; }

		StateOfMatter GetStateOfMatter() const { return m_StateOfMatter; }

		void ResetDirectionThisFrame() { m_DirectionThisFrame = Vector2f(0, 0); }


	private:
		Rectf m_Shape{};
		float const m_MoveSpeed{};
		bool m_CanMove{};

		Vector2f m_DirectionThisFrame{};
		StateOfMatter m_StateOfMatter{};
	};
}
