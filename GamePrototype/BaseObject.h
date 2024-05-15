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

	class BaseObject
	{
	public:
		BaseObject();
		BaseObject(const Rectf& shape, const StateOfMatter& stateOfMatter = StateOfMatter::solid, const Color4f& Color = Color4f(1.0f, 1.0f, 1.0f, 1.0f), bool const isEnabled = true);

		virtual void Update(float elapsedSec);
		virtual void Draw() const;

		void SetColor(const Color4f& newColor);
		void SetColor(float const newR, float const newG, float const newB, float const newA);

		Rectf GetShape() const;

		StateOfMatter GetStateOfMatter() const { return m_StateOfMatter; }

		Color4f GetColor() const { return m_SquareColor; }

		bool IsEnabled() const { return m_IsEnabled; }
		void SetEnabled(bool const enabled) { m_IsEnabled = enabled; }

	protected:
		Rectf m_Shape{};

		StateOfMatter m_StateOfMatter{};

		Color4f m_SquareColor{};

		bool m_IsEnabled{};
	};
}