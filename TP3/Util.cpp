#include "Util.h"

float VolumeFromDistance(Point p1, Point p2, float maxDistance)
{
    float distance = Point::Distance(p1, p2);
    float level = (maxDistance - distance) / maxDistance;
    return level;
}
