#include <vector>
#include <utils.h>
//#include "Level.h"

#pragma once
class Camera
{
private:
	float m_Width;
	float m_Height;
	std::vector<std::vector<Point2f>> m_LevelBoundaries;

	Point2f m_FixedLocation;
	bool	m_FixedInPlace;

	Point2f Track(const Rectf& target) const;
	//void Clamp( Point2f& bottomLeftPos) const;

public:
	Camera(float width, float height);

	void Transform(const Rectf& target, const bool fixInPlace) const;
	Point2f GetCameraPos(const Rectf& target) const;
	float GetWidth() const;
	float GetHeight() const;

	void SetFixedLocation(const Point2f& fixedLocation);
	void SetIsFixed(const bool isFixed);
};

