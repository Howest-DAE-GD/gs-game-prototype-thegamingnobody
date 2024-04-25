#include "pch.h"
#include "Camera.h"
#include <iostream>
#include <SVGParser.h>

Camera::Camera(float width, float height) :
	m_Width				{ width   },
	m_Height			{ height  },
	m_LevelBoundaries	{},
	m_FixedLocation		{},
	m_FixedInPlace		{ false }
{
	//SVGParser::GetVerticesFromSvgFile("Level.svg", m_LevelBoundaries);
}

void Camera::Transform(const Rectf& target, const bool fixInPlace) const
{
	Point2f trackingPoint{ };
	if (fixInPlace)
	{
		trackingPoint.x = m_FixedLocation.x;
		trackingPoint.y = m_FixedLocation.y;
	}
	else
	{
		trackingPoint = Track(target) ;
		//Clamp(trackingPoint);
	}

	glTranslatef(-trackingPoint.x, -trackingPoint.y, 0.0f);
}

Point2f Camera::GetCameraPos(const Rectf& target) const
{
	Point2f trackingPoint{ };
	if (m_FixedInPlace)
	{
		trackingPoint.x = m_FixedLocation.x;
		trackingPoint.y = m_FixedLocation.y;
	}
	else
	{
		trackingPoint = Track(target);
		//Clamp(trackingPoint);
	}
	return trackingPoint;
}

Point2f Camera::Track(const Rectf& target) const
{
	Point2f tempTrackingPoint{};

	tempTrackingPoint.x = target.left   - ((m_Width)  / 2);
	tempTrackingPoint.y = target.bottom - ((m_Height) / 2);
	
	return tempTrackingPoint;
}

//void Camera::Clamp( Point2f& bottomLeftPos) const
//{
//	utils::HitInfo hit{  };
//	Point2f topLeft			{ Point2f(bottomLeftPos.x,				bottomLeftPos.y + m_Height) };
//	Point2f bottomRight		{ Point2f(bottomLeftPos.x + m_Width,	bottomLeftPos.y) };
//	Point2f topRight		{ Point2f(bottomLeftPos.x + m_Width,	bottomLeftPos.y + m_Height) };
//
//
//	float leftCap	{ 24.0f };
//	float rightCap	{ 1300.0f };
//	float downCap	{ 60.0f };
//	float UpCap		{ 92.0f };
//
//	if (bottomLeftPos.y > 300.0f)
//	{
//		rightCap = 1190.0f;
//		downCap = 400.0f;
//		UpCap = 413.0f;
//	}
//	if (bottomLeftPos.y > 700.0f)
//	{
//		rightCap = 1335.0f;
//		downCap = 750.0f;
//		UpCap = 760.0f;
//	}
//	if (bottomLeftPos.x > 1450.0f && bottomLeftPos.y > 300.0f)
//	{
//		leftCap = 1607.0f;
//		rightCap = 1623.0f;
//		UpCap = 590.0f;
//	}
//
//	if (bottomLeftPos.y <= downCap or bottomLeftPos.y >= UpCap)
//	{
//		//bottomLeftPos.x = hit.intersectPoint.x;
//
//		if (bottomLeftPos.y < downCap)
//		{
//			bottomLeftPos.y = downCap;
//		}
//		if (bottomLeftPos.y > UpCap)
//		{
//			bottomLeftPos.y = UpCap;
//		}
//	}
//	if (bottomLeftPos.x >= leftCap && bottomLeftPos.x <= rightCap)
//	{
//		if (utils::Raycast(m_LevelBoundaries[0], topLeft, topRight, hit) && utils::Raycast(m_LevelBoundaries[0], bottomLeftPos, bottomRight, hit))
//		{
//			Point2f middle{ Point2f(bottomLeftPos.x + (m_Width / 2), bottomLeftPos.y)};
//			if (hit.intersectPoint.x > middle.x)
//			{
//				bottomLeftPos.x = hit.intersectPoint.x - m_Width;
//			}
//			else
//			{
//				bottomLeftPos.x = hit.intersectPoint.x;
//			}
//		}
//	}
//	else
//	{
//		//bottomLeftPos.x = hit.intersectPoint.x;
//
//		if (bottomLeftPos.x < leftCap)
//		{
//			bottomLeftPos.x = leftCap;
//		}
//		if (bottomLeftPos.x > rightCap)
//		{
//			bottomLeftPos.x = rightCap;
//		}
//	}
//}

float Camera::GetWidth() const
{
	return m_Width;
}
float Camera::GetHeight() const
{
	return m_Height;
}

void Camera::SetFixedLocation(const Point2f& fixedLocation)
{
	m_FixedLocation = fixedLocation;
	m_FixedInPlace = true;
}

void Camera::SetIsFixed(const bool isFixed)
{
	m_FixedInPlace = isFixed;
}