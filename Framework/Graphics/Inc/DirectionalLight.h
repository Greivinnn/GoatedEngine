#pragma once

#include "Color.h"

namespace GoatedEngine::Graphics
{
	struct DirectionalLight
	{
		Color ambient = Colors::White;					// when no light is present
		Color diffuse = Colors::White;					// main light color
		Color specular = Colors::White;					// when highlights are colored 
		Math::Vector3 direction = Math::Vector3::ZAxis;	// direction of the light
		float padding = 0.0f;
	};
}