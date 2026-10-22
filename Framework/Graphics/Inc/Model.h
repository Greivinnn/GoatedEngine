#pragma once

#include "MeshType.h"
#include "Material.h"

namespace GoatedEngine::Graphics
{
	struct Model
	{
		struct MeshData
		{
			Mesh mesh;
			uint32_t materialIndex = 0;
		};
		struct MaterialData
		{
			Material material;
			std::string diffuseName;
			std::string specName;
			std::string normName;
			std::string bumpName;
		};

		std::vector<MeshData> meshData;
		std::vector<MaterialData> materialData;
	};
}
