/**
    Declaration of Eye
*/

#pragma once

#include <vector>

/**
	Holds points relating to an eye and provides methods for calculating
	Eye Aspect Ratio used in determining if eye is blinking.
*/
class Eye
{
public:

	Eye
		(
		int x1, int y1,
		int x2, int y2,
		int x3, int y3,
		int x4, int y4,
		int x5, int y5,
		int x6, int y6
		);

	~Eye() = default;

	double AspectRatio();

private:

	/**
		Stores x,y coordinate for a point
	*/
	struct Point
	{
		int x;
		int y;
	};

	double EuclideanDistance( int p, int q );

	//! Stores set of points outlining the eye
	std::vector<Point> mEyePoints;
};