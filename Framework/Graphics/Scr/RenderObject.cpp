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