#pragma once

#include "Color.h"

namespace GoatedEngine::Graphics
{
	struct Material
	{
		Color emissive = Colors::Black;		// color where no light is present
		Color ambient = Colors::White;		// color with minimal light
		Color diffuse = Colors::White;		// base light color
		Color specular = Colors::White;		// highlight color
		float shininess = 10.0f;			// intensity of the light
		float padding[3] = { 0.0f };
	};
}