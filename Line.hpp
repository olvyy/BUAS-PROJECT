#pragma once
#include "template.h"
#include "Rectangle.hpp"

struct Line
{
	Tmpl8::vec2 base;
	Tmpl8::vec2 direciton;

	Line() : base(0, 0), direciton(0, 0) {}

	Line(const Tmpl8::vec2& c_base, const Tmpl8::vec2& c_direction)
		: base(c_base), direciton(c_direction) {}

	bool LineLineIntersection(Tmpl8::vec2 a1, Tmpl8::vec2 a2, Tmpl8::vec2 b1, Tmpl8::vec2 b2);
};