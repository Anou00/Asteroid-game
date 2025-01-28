#include "Constants.h"

bool checkCollision(Circle& c1, Circle& c2)
{
    //Calculate total radius squared
    int totalRadiusSquared = c1.r + c2.r;
    totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    return (distanceSquared(c1.origin, c2.origin)) < (totalRadiusSquared);
}

double distanceSquared(Point p1, Point p2)
{
    int deltaX = p1.y - p1.x;
    int deltaY = p2.y - p2.x;
    return deltaX * deltaX + deltaY * deltaY;
}