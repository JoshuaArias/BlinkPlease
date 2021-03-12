/**
    Definition of Eye
*/

#include "Eye.hpp"

#include <cmath>

/**
    Constructor
*/
Eye::Eye
    (
    int x1, int y1,
    int x2, int y2,
    int x3, int y3,
    int x4, int y4,
    int x5, int y5,
    int x6, int y6
    )
{
    mEyePoints.emplace_back( Point{ x1, y1 } );
    mEyePoints.emplace_back( Point{ x2, y2 } );
    mEyePoints.emplace_back( Point{ x3, y3 } );
    mEyePoints.emplace_back( Point{ x4, y4 } );
    mEyePoints.emplace_back( Point{ x5, y5 } );
    mEyePoints.emplace_back( Point{ x6, y6 } );
}

/**
    Calculates the Eye Aspect Ratio for this eye object

    Formula for Eye Aspect Ratio (EAR):
    EAR = ( dist(p_1, p_5) + dist(p_2, p_4) ) / 2*dist(p_0, p_3 )

    @note caller will need to find the EAR for both eyes and then
            average the two values before using EAR for detecting blinks

    @pre mEyePoints must be filled with all values for eyes

    @return Eye Aspect Ratio for this eye
*/
double Eye::AspectRatio()
{
    double height1 = EuclideanDistance( 1, 5 );
    double height2 = EuclideanDistance( 2, 4 );
    double width   = EuclideanDistance( 0, 3 );
    return ( height1 + height2 ) / ( 2.0 * width );
}

/**
    Calculates the euclidian distance between two sets of points

    The following formula is used:
    d(p,q) = sqrt( (q_1 - p_1)^2 + (q_2 - p_2)^2 )

    @return Euclidean distance between the two given points
*/
double Eye::EuclideanDistance( int p, int q )
{
    return std::sqrt( std::pow( ( mEyePoints[q].x - mEyePoints[p].x ), 2.0 ) +
                      std::pow( ( mEyePoints[q].y - mEyePoints[p].y ), 2.0 )
                    );
}