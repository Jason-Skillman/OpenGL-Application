#pragma once
#include "Vertex.h"

class [[deprecated]] Quad {
private:
	float x, y;
	Vertex vertices[4];

public:
	Quad(float x, float y);
};
