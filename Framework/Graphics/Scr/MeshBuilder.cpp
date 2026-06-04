#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;

namespace
{
	Color GetNextColor(int& index)
	{
		constexpr Color colorTable[] = {
			Colors::Red,
			Colors::Green,
			Colors::Blue,
			Colors::Yellow,
			Colors::Cyan,
			Colors::Magenta,
			Colors::White,
			Colors::Pink,
			Colors::Peru,
			Colors::Indigo,
			Colors::DarkSalmon
		};

		index = (index + 1) % std::size(colorTable);
		return colorTable[index];
	}

	void CreateCubeIndices(std::vector<uint32_t>& indices)
	{
		indices = {
			// front
			0, 1, 2,
			0, 2, 3,
			// back
			7, 5, 4,
			7, 6, 5,
			// right
			3, 2, 6,
			3, 6, 7,
			// left
			4, 5, 1,
			4, 1, 0,
			// top
			1, 5, 6,
			1, 6, 2,
			// bottom
			0, 3, 7,
			0, 7, 4
		};
	}

	void CreatePlaneIndices(std::vector<uint32_t>& indices, int numRows, int numColumns)
	{
		for (int r = 0; r < numRows; ++r)
		{
			for (int c = 0; c < numColumns; ++c)
			{
				int i = (r * (numColumns + 1)) + c;
				// triangle 0
				indices.push_back(i);
				indices.push_back(i + numColumns + 1);
				indices.push_back(i + numColumns + 2);

				// triangle 1
				indices.push_back(i);
				indices.push_back(i + numColumns + 2);
				indices.push_back(i + 1);
			}
		}
	}
}

MeshPC MeshBuilder::CreateVertexCubePC(float size, const Color& color)
{
	MeshPC mesh;
	const float hs = size * 0.5f;

	mesh.vertices.push_back({ {-hs, -hs, -hs }, { color } });
	mesh.vertices.push_back({ { hs,  hs, -hs }, { color } });
	mesh.vertices.push_back({ { hs, -hs, -hs }, { color } });

	mesh.vertices.push_back({ {-hs, -hs, -hs }, { color } });
	mesh.vertices.push_back({ {-hs,  hs, -hs }, { color } });
	mesh.vertices.push_back({ { hs,  hs, -hs }, { color } });

	return mesh;
}

MeshPC MeshBuilder::CreateCubePC(float size)
{
	MeshPC mesh;
	const float hs = size * 0.5f;
	int index = rand() % 100;

	// front
	mesh.vertices.push_back({ {-hs, -hs, -hs }, GetNextColor(index) });
	mesh.vertices.push_back({ {-hs,  hs, -hs }, GetNextColor(index) });
	mesh.vertices.push_back({ { hs,  hs, -hs }, GetNextColor(index) });
	mesh.vertices.push_back({ { hs, -hs, -hs }, GetNextColor(index) });

	// back
	mesh.vertices.push_back({ {-hs, -hs,  hs }, GetNextColor(index) });
	mesh.vertices.push_back({ {-hs,  hs,  hs }, GetNextColor(index) });
	mesh.vertices.push_back({ { hs,  hs,  hs }, GetNextColor(index) });
	mesh.vertices.push_back({ { hs, -hs,  hs }, GetNextColor(index) });

	// add indices
	CreateCubeIndices(mesh.indices);

	return mesh;
}

MeshPX MeshBuilder::CreateCubePX(float size)
{
	MeshPX mesh;

	const float hs = size * 0.5f;
	const float ot = 1.0f / 3.0f;
	const float tt = 2.0f / 3.0f;

	// front
	mesh.vertices.push_back({ { -hs, -hs, -hs}, {0.25f, tt} });
	mesh.vertices.push_back({ { -hs,  hs, -hs}, {0.25f, ot} });
	mesh.vertices.push_back({ {  hs,  hs, -hs}, {0.5f, ot} });

	mesh.vertices.push_back({ { -hs, -hs, -hs}, {0.25f, tt} });
	mesh.vertices.push_back({ {  hs,  hs, -hs}, {0.5f, ot} });
	mesh.vertices.push_back({ {  hs, -hs, -hs}, {0.5f, tt} });

	// right
	mesh.vertices.push_back({ {  hs, -hs, -hs}, {0.5f, tt} });
	mesh.vertices.push_back({ {  hs,  hs, -hs}, {0.5f, ot} });
	mesh.vertices.push_back({ {  hs,  hs,  hs}, {0.75f, ot} });

	mesh.vertices.push_back({ {  hs, -hs, -hs}, {0.5f, tt} });
	mesh.vertices.push_back({ {  hs,  hs,  hs}, {0.75f, ot} });
	mesh.vertices.push_back({ {  hs, -hs,  hs}, {0.75f, tt} });

	// back
	mesh.vertices.push_back({ {  hs, -hs,  hs}, {0.75f, tt} });
	mesh.vertices.push_back({ {  hs,  hs,  hs}, {0.75f, ot} });
	mesh.vertices.push_back({ { -hs,  hs,  hs}, {1.0f, ot} });

	mesh.vertices.push_back({ {  hs, -hs,  hs}, {0.75f, tt} });
	mesh.vertices.push_back({ { -hs,  hs,  hs}, {1.0f, ot} });
	mesh.vertices.push_back({ { -hs, -hs,  hs}, {1.0f, tt} });

	// left
	mesh.vertices.push_back({ { -hs, -hs, -hs}, {0.25f, tt} });
	mesh.vertices.push_back({ { -hs,  hs,  hs}, {0.0f, ot} });
	mesh.vertices.push_back({ { -hs,  hs, -hs}, {0.25f, ot} });

	mesh.vertices.push_back({ { -hs, -hs, -hs}, {0.25f, tt} });
	mesh.vertices.push_back({ { -hs, -hs,  hs}, {0.0f, tt} });
	mesh.vertices.push_back({ { -hs,  hs,  hs}, {0.0f, ot} });

	// top
	mesh.vertices.push_back({ { -hs,  hs, -hs}, {0.25f, ot} });
	mesh.vertices.push_back({ { -hs,  hs,  hs}, {0.25f,0.0f} });
	mesh.vertices.push_back({ {  hs,  hs,  hs}, {0.5f,0.0f} });

	mesh.vertices.push_back({ { -hs,  hs, -hs}, {0.25f, ot} });
	mesh.vertices.push_back({ {  hs,  hs,  hs}, {0.5f,0.0f} });
	mesh.vertices.push_back({ {  hs,  hs, -hs}, {0.5f, ot} });

	// bottom
	mesh.vertices.push_back({ { -hs, -hs, -hs}, {0.25f, tt} });
	mesh.vertices.push_back({ {  hs, -hs,  hs}, {0.5f,1.0f} });
	mesh.vertices.push_back({ { -hs, -hs,  hs}, {0.25f,1.0f} });

	mesh.vertices.push_back({ { -hs, -hs, -hs}, {0.25f, tt} });
	mesh.vertices.push_back({ {  hs, -hs, -hs}, {0.5f, tt} });
	mesh.vertices.push_back({ {  hs, -hs,  hs}, {0.5f,1.0f} });

	// dont need indices as the vertices make up the shape

	return mesh;
}

MeshPC MeshBuilder::CreateBoxPC(float width, float height, float depth)
{
	MeshPC mesh;
	const float hw = width * 0.5f;
	const float hh = height * 0.5f;
	const float hd = depth * 0.5f;
	int index = rand() % 100;

	// front
	mesh.vertices.push_back({ {-hw, -hh, -hd }, GetNextColor(index) });
	mesh.vertices.push_back({ {-hw,  hh, -hd }, GetNextColor(index) });
	mesh.vertices.push_back({ { hw,  hh, -hd }, GetNextColor(index) });
	mesh.vertices.push_back({ { hw, -hh, -hd }, GetNextColor(index) });

	// back
	mesh.vertices.push_back({ {-hw, -hh,  hd }, GetNextColor(index) });
	mesh.vertices.push_back({ {-hw,  hh,  hd }, GetNextColor(index) });
	mesh.vertices.push_back({ { hw,  hh,  hd }, GetNextColor(index) });
	mesh.vertices.push_back({ { hw, -hh,  hd }, GetNextColor(index) });

	CreateCubeIndices(mesh.indices);

	return mesh;
}

MeshPC MeshBuilder::CreatePyramidPC(float size)
{
	MeshPC mesh;
	const float hs = size * 0.5f;
	int index = rand() % 100;

	// top
	mesh.vertices.push_back({ { 0.0f,  hs, 0.0f }, GetNextColor(index) }); // 0
	// bottom
	mesh.vertices.push_back({ { -hs, -hs, -hs }, GetNextColor(index) }); // 1
	mesh.vertices.push_back({ { -hs, -hs,  hs }, GetNextColor(index) }); // 2
	mesh.vertices.push_back({ {  hs, -hs,  hs }, GetNextColor(index) }); // 3
	mesh.vertices.push_back({ {  hs, -hs, -hs }, GetNextColor(index) }); // 4

	mesh.indices = {
		// top pointy part
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 1,

		// bottom
		1, 3, 2,
		1, 4, 3
	};

	return mesh;
}

MeshPC MeshBuilder::CreatePlanePC(int numRows, int numColumns, float spacing, bool horizontal)
{
	MeshPC mesh;
	int index = rand() % 100;
	const float hpw = static_cast<float>(numColumns) * spacing * 0.5f;
	const float hph = static_cast<float>(numRows) * spacing * 0.5;

	float w = -hpw;
	float h = -hph;
	for (int r = 0; r <= numRows; ++r)
	{
		for (int c = 0; c <= numColumns; ++c)
		{
			// horizontal is x/z and not horizontal (vertical) is x/y
			Math::Vector3 position = (horizontal) ? Math::Vector3{ w, 0.0f, h } : Math::Vector3{ w, h, 0.0f };
			mesh.vertices.push_back({ position, GetNextColor(index) });
			w += spacing;
		}
		w = -hpw;
		h += spacing;
	}

	CreatePlaneIndices(mesh.indices, numRows, numColumns);

	return mesh;
}

MeshPX MeshBuilder::CreatePlanePX(int numRows, int numColumns, float spacing, bool horizontal)
{
	MeshPX mesh;
	int index = rand() % 100;
	const float hpw = static_cast<float>(numColumns) * spacing * 0.5f;
	const float hph = static_cast<float>(numRows) * spacing * 0.5;
	const float uInc = 1.0f / static_cast<float>(numColumns);
	const float vInc = -1.0f / static_cast<float>(numRows);

	float w = -hpw;
	float h = -hph;
	float u = 0.0f;
	float v = 1.0f;

	for (int r = 0; r <= numRows; ++r)
	{
		for (int c = 0; c <= numColumns; ++c)
		{
			// horizontal is x/z and not horizontal (vertical) is x/y
			Math::Vector3 position = (horizontal) ? Math::Vector3{ w, 0.0f, h } : Math::Vector3{ w, h, 0.0f };
			mesh.vertices.push_back({ position, { u, v } });
			w += spacing;
			u += uInc;
		}
		w = -hpw;
		h += spacing;
		v += vInc;
	}

	CreatePlaneIndices(mesh.indices, numRows, numColumns);

	return mesh;
}

MeshPC MeshBuilder::CreateCylinderPC(int slices, int rings)
{
	MeshPC mesh;
	int index = rand() % 100;

	const float hh = static_cast<float>(rings) * 0.5f;
	const float fSlices = static_cast<float>(slices);
	for (int r = 0; r <= rings; ++r)
	{
		float rF = static_cast<float>(r);
		for (int s = 0; s <= slices; ++s)
		{
			float sF = static_cast<float>(s);
			float rotation = (sF / fSlices) * Math::Constants::TwoPi;

			mesh.vertices.push_back({ {sin(rotation), rF - hh, -cos(rotation) }, GetNextColor(index) });
		}
	}

	CreatePlaneIndices(mesh.indices, rings, slices);

	// add top and bottom
	uint32_t topCenterIndex = mesh.vertices.size();
	mesh.vertices.push_back({ { 0.0f, hh, 0.0f }, GetNextColor(index) });

	for (int s = 0; s < slices; ++s)
	{
		uint32_t topRingIndex = rings * (slices + 1);
		mesh.indices.push_back(topCenterIndex);
		mesh.indices.push_back(topRingIndex + s + 1);
		mesh.indices.push_back(topRingIndex + s);
	}

	uint32_t bottomCenterIndex = mesh.vertices.size();
	mesh.vertices.push_back({ { 0.0f, -hh, 0.0f }, GetNextColor(index) });

	for (int s = 0; s < slices; ++s)
	{
		mesh.indices.push_back(bottomCenterIndex);
		mesh.indices.push_back(s);
		mesh.indices.push_back(s + 1);
	}

	return mesh;
}

MeshPC MeshBuilder::CreateSpherePC(int slices, int rings, float radius)
{
	MeshPC mesh;
	int index = rand() % 100;

	float vertRotation = Math::Constants::Pi / static_cast<float>(rings);
	float horizRotation = Math::Constants::TwoPi / static_cast<float>(slices);

	for (int r = 0; r <= rings; ++r)
	{
		float rF = static_cast<float>(r);
		float phi = rF * vertRotation;
		for (int s = 0; s <= slices; ++s)
		{
			float sF = static_cast<float>(s);
			float rotation = sF * horizRotation;

			mesh.vertices.push_back({ {
					radius * sin(rotation) * sin(phi),	 // x
					radius * cos(phi),					 // y
					radius * cos(rotation) * sin(phi) }, // z
					GetNextColor(index) });
		}
	}

	CreatePlaneIndices(mesh.indices, rings, slices);

	return mesh;
}

MeshPX MeshBuilder::CreateSpherePX(int slices, int rings, float radius)
{
	MeshPX mesh;
	int index = rand() % 100;

	float vertRotation = Math::Constants::Pi / static_cast<float>(rings);
	float horizRotation = Math::Constants::TwoPi / static_cast<float>(slices);

	float uStep = 1.0f / static_cast<float>(slices);
	float vStep = 1.0f / static_cast<float>(rings);

	for (int r = 0; r <= rings; ++r)
	{
		float rF = static_cast<float>(r);
		float phi = rF * vertRotation;
		for (int s = 0; s <= slices; ++s)
		{
			float sF = static_cast<float>(s);
			float rotation = sF * horizRotation;

			float u = uStep * sF;
			float v = vStep * rF;
			mesh.vertices.push_back({ {
					radius * sin(rotation) * sin(phi),	 // x
					radius * cos(phi),					 // y
					radius * cos(rotation) * sin(phi) }, // z
					{ u, v } });
		}
	}

	CreatePlaneIndices(mesh.indices, rings, slices);

	return mesh;
}

MeshPX MeshBuilder::CreateSkySpherePX(int slices, int rings, float radius)
{
	MeshPX mesh;
	int index = rand() % 100;

	float vertRotation = Math::Constants::Pi / static_cast<float>(rings);
	float horizRotation = Math::Constants::TwoPi / static_cast<float>(slices);

	float uStep = 1.0f / static_cast<float>(slices);
	float vStep = 1.0f / static_cast<float>(rings);

	for (int r = 0; r <= rings; ++r)
	{
		float rF = static_cast<float>(r);
		float phi = rF * vertRotation;
		for (int s = 0; s <= slices; ++s)
		{
			float sF = static_cast<float>(s);
			float rotation = sF * horizRotation;

			float u = uStep * sF;
			float v = vStep * rF;
			mesh.vertices.push_back({ {
					radius * cos(rotation) * sin(phi),	 // x
					radius * cos(phi),					 // y
					radius * sin(rotation) * sin(phi) }, // z
					{ u, v } });
		}
	}

	CreatePlaneIndices(mesh.indices, rings, slices);

	return mesh;
}