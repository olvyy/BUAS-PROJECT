#include "Rectangle.hpp"
#include <utility> // to use min

bool overlapping(float minA, float maxA, float minB, float maxB)
{
	return minB <= maxA && minA <= maxB;
}

bool collide(Rectangle a, Rectangle b)
{
	float aLeft = a.origin.x;
	float aRight = aLeft + a.size.x;
	float bLeft = b.origin.x;
	float bRight = bLeft + b.size.x;

	float aBottom = a.origin.y;
	float aTop = aBottom + a.size.y;
	float bBottom = b.origin.y;
	float bTop = bBottom + b.size.y;

	return
		overlapping(aLeft, aRight, bLeft, bRight)
		&&
		overlapping(aBottom, aTop, bBottom, bTop);

}

void DrawRectangle(Rectangle& rect, Tmpl8::Surface* screen, Tmpl8::Pixel color)
{
	int x1 = static_cast<int>(rect.origin.x);
	int y1 = static_cast<int>(rect.origin.y) ;

	int x2 = x1 + static_cast<int>(rect.size.x) ;
	int y2 = y1 + static_cast<int>(rect.size.y);

	screen->Box(x1, y1, x2, y2, color);
}
