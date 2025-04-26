#include "Line.hpp"

//Rectangle-Line collision system from "2D Game Collision Detection" by Thomas Schwarzl

bool Line::LineRectangleCollision(Line l, Rectangle r)
{
	Tmpl8::vec2 n = (l.direciton.y, -l.direciton.x); // roteted 90 degrees clockwise

	float dp1, dp2, dp3, dp4;

	Tmpl8::vec2 c1 = r.origin;
	Tmpl8::vec2 c2 = c1 + r.size;
	Tmpl8::vec2 c3 = { c2.x, c1.y };	
	Tmpl8::vec2 c4 = { c1.x, c2.y };

	c1 = (c1 - l.base);
	c2 = (c2 - l.base);
	c3 = (c3 - l.base);
	c4 = (c4 - l.base);

	dp1 = (n.x * c1.x) + (n.y * c1.y);
	dp2 = (n.x * c2.x) + (n.y * c2.y);
	dp3 = (n.x * c3.x) + (n.y * c3.y);
	dp4 = (n.x * c4.x) + (n.y * c4.y);

	return
		(dp1 * dp2 <= 0)
		||
		(dp2 * dp3 <= 0)
		||
		(dp3 * dp4 <= 0);
}
