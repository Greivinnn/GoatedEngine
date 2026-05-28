#pragma once

#include "MeshType.h"

namespace GoatedEngine::Graphics
{
	class MeshBuilder
	{
	public:
		// cube
		static MeshPC CreateVertexCubePC(float size, const Color& color);
		static MeshPC CreateCubePC(float size);

		static MeshPX CreateCubePX(float size);

		// rectangle
		static MeshPC CreateBoxPC(float width, float height, float depth);

		// pyramid
		static MeshPC CreatePyramidPC(float size);

		// plane
		static MeshPC CreatePlanePC(int numRows, int numColumns, int spacing, bool horizontal);
		static MeshPX CreatePlanePX(int numRows, int numColumns, int spacing, bool horizontal);

		// cylinder
		static MeshPC CreateCylinderPC(int slices, int rings);

		// sphere
		static MeshPC CreateSpherePC(int slices, int rings, float radius);
	};
}