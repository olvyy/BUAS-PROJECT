#pragma once
#include "template.h"
#include "surface.h"

//using a rectangle-rectangle collision system from "2D Game Collision Detection" by Thomas Schwarzl

struct Rectangle
{
	Tmpl8::vec2 origin;
	Tmpl8::vec2 size;

	// Default constructor
	Rectangle() : origin(0, 0), size(0, 0) {}

	// Constructor
	Rectangle(const Tmpl8::vec2& c_origin, const Tmpl8::vec2& c_size)
		: origin(c_origin), size(c_size) {
	}
};


bool overlapping(float minA, float maxA, float minB, float maxB);

bool collide(Rectangle a, Rectangle b);

void DrawRectangle(Rectangle& rect, Tmpl8::Surface* screen, Tmpl8::Pixel color);
