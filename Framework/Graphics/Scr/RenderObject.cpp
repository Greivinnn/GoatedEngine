#include "Precompiled.h"
#include "RenderObject.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;

void RenderObject::Terminate()
{
	meshBuffer.Terminate();

	TextureManager* tm = TextureManager::Get();
	tm->ReleaseTexture(diffuseMapId);
	tm->ReleaseTexture(specMapId);
	tm->ReleaseTexture(normMapId);
	tm->ReleaseTexture(bumpMapId);
}

void RenderGroup::Initialize(const std::filesystem::path& modelFilePath)
{
	modelId = ModelManager::Get()->LoadModel(modelFilePath);
	const Model* model = ModelManager::Get()->GetModel(modelId);
	ASSERT(model != nullptr, "RenderGroup: model was not found for %s", modelFilePath.u8string().c_str());
	Initialize(*model);
}
void RenderGroup::Initialize(const Model& model)
{
	auto TryLoadTexture = [](const auto& textureName)->TextureId
		{
			if (textureName.empty())
			{
				return 0;
			}
			return TextureManager::Get()->LoadTexture(textureName);
		};

	for (const Model::MeshData& meshData : model.meshData)
	{
		RenderObject& renderObject = renderObjects.emplace_back();
		renderObject.meshBuffer.Initialize(meshData.mesh);
		if (meshData.materialIndex < model.materialData.size())
		{
			const Model::MaterialData& materialData = model.meshData[meshData.materialIndex];
			renderObject.material = materialData.material;

			renderObject.diffuseMapId = TryLoadTexture(materialData.diffuseName);
			renderObject.specMapId = TryLoadTexture(materialData.specName);
			renderObject.normMapId = TryLoadTexture(materialData.normName);
			renderObject.bumpMapId = TryLoadTexture(materialData.bumpName);
		}
	}
}
void RenderGroup::Terminate()
{
	for (RenderObject& renderObject : renderObjects)
	{
		renderObject.Terminate();
	}
	renderObjects.clear();
}