#include "Line.hpp"
//using https://paulbourke.net/geometry/pointlineplane/

bool Line::LineLineIntersection(Tmpl8::vec2 a1, Tmpl8::vec2 a2, Tmpl8::vec2 b1, Tmpl8::vec2 b2)
{
	float denominator = (b2.y - b1.y) * (a2.x - a1.x) - (b2.x - b1.x) * (a2.y - a1.y);
	if (denominator == 0)
	{
		return false; // lines are parallel
	}

	float ua = ((b2.x - b1.x) * (a1.y - b1.y) - (b2.y - b1.y) * (a1.x - b1.x)) / denominator;
	float ub = ((a2.x - a1.x) * (a1.y - b1.y) - (a2.y - a1.y) * (a1.x - b1.x)) / denominator;

	return (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1);
}
