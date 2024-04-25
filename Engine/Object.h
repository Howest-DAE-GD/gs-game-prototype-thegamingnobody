#pragma once
#include "structs.h"
#include "Vector2f.h"

namespace dae
{
	class Object
	{
	public:
		Object();
		Object(const Rectf& shape, float const moveSpeed = 0, bool const canMove = false);

		virtual void Update( float elapsedSec );
		virtual void Draw() const;

		void AddPositionOffset(const Vector2f& direction);
		void AddPositionOffset(float const directionX, float const directionY);

		float GetMoveSpeed() const { return m_MoveSpeed; }

		Rectf GetShape() { return m_Shape; }

	private:
		Rectf m_Shape{};
		float const m_MoveSpeed{};
		bool m_CanMove{};

		Vector2f m_DirectionThisFrame{};
	};
}
