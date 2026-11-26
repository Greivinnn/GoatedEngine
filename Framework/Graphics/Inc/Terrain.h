#pragma once

#include "MeshType.h"

namespace GoatedEngine::Graphics
{
	class Terrain final
	{
	public:
		void Initialize(const std::filesystem::path& filePath, float heightScale);
		float GetHeight(const Math::Vector3& position) const;

		Mesh mesh;
		uint32_t rows = 0;
		uint32_t columns = 0;
	};
}