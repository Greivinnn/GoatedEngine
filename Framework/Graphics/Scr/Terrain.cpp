#include "Precompiled.h"
#include "Terrain.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;

void Terrain::Initialize(const std::filesystem::path& fileName, float heightScale)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "rb");
	ASSERT(file != nullptr, "Terrain: file %s was not found", fileName.u8string().c_str());

	fseek(file, 0L, SEEK_END);
	const uint32_t fileSize = ftell(file);
	const uint32_t dimensions = (uint32_t)sqrt(static_cast<float>(fileSize));
	fseek(file, 0L, SEEK_SET);

	rows = dimensions;
	columns = dimensions;
	const float tileCount = 10.0f;
	mesh.vertices.resize(rows * columns);
	for (uint32_t z = 0; z < rows; ++z)
	{
		for (uint32_t x = 0; x < columns; ++x)
		{
			const auto c = fgetc(file);
			const float height = (c / 255.0f) * heightScale;
			const uint32_t index = x + (z * columns);

			Vertex& vertex = mesh.vertices[index];
			const float posX = static_cast<float>(x);
			const float posZ = static_cast<float>(z);
			vertex.position = { posX, height, posZ };
			vertex.normal = Math::Vector3::YAxis;
			vertex.uvCoord.x = (posX / columns) * tileCount;
			vertex.uvCoord.y = (posZ / rows) * tileCount;
		}
	}
	fclose(file);

	const uint32_t cells = (rows - 1) * (columns - 1);
	mesh.indices.reserve(cells * 6);
	for (uint32_t z = 0; z < rows - 1; ++z)
	{
		for (uint32_t x = 0; x < columns - 1; ++x)
		{
			const uint32_t bl = x + (z * columns);
			const uint32_t tl = x + ((z + 1) * columns);
			const uint32_t br = (x + 1) + (z * columns);
			const uint32_t tr = (x + 1) + ((z + 1) * columns);

			mesh.indices.push_back(bl);
			mesh.indices.push_back(tl);
			mesh.indices.push_back(tr);

			mesh.indices.push_back(bl);
			mesh.indices.push_back(tr);
			mesh.indices.push_back(br);
		}
	}

	// real normals from neighbor heights (central difference), otherwise lighting is flat
	for (uint32_t z = 0; z < rows; ++z)
	{
		for (uint32_t x = 0; x < columns; ++x)
		{
			const uint32_t xl = (x > 0) ? x - 1 : x;
			const uint32_t xr = (x + 1 < columns) ? x + 1 : x;
			const uint32_t zd = (z > 0) ? z - 1 : z;
			const uint32_t zu = (z + 1 < rows) ? z + 1 : z;
			const float hl = mesh.vertices[xl + (z * columns)].position.y;
			const float hr = mesh.vertices[xr + (z * columns)].position.y;
			const float hd = mesh.vertices[x + (zd * columns)].position.y;
			const float hu = mesh.vertices[x + (zu * columns)].position.y;
			Vertex& vertex = mesh.vertices[x + (z * columns)];
			const float dx = static_cast<float>(xr - xl);
			const float dz = static_cast<float>(zu - zd);
			vertex.normal = Math::Normalize(Math::Vector3((hl - hr) * dz, dx * dz, (hd - hu) * dx));
			vertex.tangent = Math::Vector3::XAxis;
		}
	}
}

float Terrain::GetHeight(const Math::Vector3& position) const
{
	const int x = static_cast<int>(position.x);
	const int z = static_cast<int>(position.z);
	if (x < 0 || z < 0 || x + 1 >= static_cast<int>(columns) || z + 1 >= static_cast<int>(rows))
	{
		return -1.0f;
	}

	const uint32_t bl = x + (z * columns);
	const uint32_t tl = x + ((z + 1) * columns);
	const uint32_t br = (x + 1) + (z * columns);
	const uint32_t tr = (x + 1) + ((z + 1) * columns);

	const float u = position.x - x;
	const float v = position.z - z;

	float height = 0.0f;
	if (u > v)
	{
		const Vertex& a = mesh.vertices[br];
		const Vertex& b = mesh.vertices[tr];
		const Vertex& c = mesh.vertices[bl];
		const float deltaAB = b.position.y - a.position.y;
		const float deltaAC = c.position.y - a.position.y;
		height = a.position.y + (deltaAB * v) + (deltaAC * (1 - u));
	}
	else
	{
		const Vertex& a = mesh.vertices[tl];
		const Vertex& b = mesh.vertices[tr];
		const Vertex& c = mesh.vertices[bl];
		const float deltaAB = b.position.y - a.position.y;
		const float deltaAC = c.position.y - a.position.y;
		height = a.position.y + (deltaAB * u) + (deltaAC * (1 - v));
	}
	return height;
}