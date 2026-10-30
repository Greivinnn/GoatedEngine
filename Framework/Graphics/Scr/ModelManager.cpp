#include "Precompiled.h"
#include "ModelManager.h"
#include "ModelIO.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;

namespace
{
	std::unique_ptr<ModelManager> sModelManager;
}

void ModelManager::StaticInitialize(const std::filesystem::path& rootPath)
{
	ASSERT(sModelManager == nullptr, "ModelManager: is already initialized");
	sModelManager = std::make_unique<ModelManager>();
	sModelManager->SetRootDirectory(rootPath);
}
void ModelManager::StaticTerminate()
{
	sModelManager.reset();
}
ModelManager* ModelManager::Get()
{
	ASSERT(sModelManager != nullptr, "ModelManager: is not initialized");
	return sModelManager.get();
}

void ModelManager::SetRootDirectory(const std::filesystem::path& roothPath)
{
	mRootDirectory = roothPath;
}
ModelId ModelManager::GetModelId(const std::filesystem::path& filePath)
{
	return std::filesystem::hash_value(mRootDirectory / filePath);
}
ModelId ModelManager::LoadModel(const std::filesystem::path& filePath)
{
	const ModelId& modelId = GetModelId(filePath);
	auto [iter, sucess] = mInventory.insert({ modelId, nullptr });
	if (sucess)
	{
		std::filesystem::path fullPath = mRootDirectory / filePath;
		auto& modelPtr = iter->second;
		modelPtr = std::make_unique<Model>();
		ModelIO::LoadModel(fullPath, *modelPtr);
	}
	return modelId;
}

const Model* ModelManager::GetModel(ModelId id)
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
	{
		return iter->second.get();
	}
	return nullptr;

}