#pragma once

#include "MeshBuffer.h"
#include "Transform.h"
#include "Material.h"
#include "TextureManager.h"

namespace GoatedEngine::Graphics
{
	class RenderObject
	{
	public: 
		void Terminate();

		MeshBuffer meshBuffer;		  // shape, vertices, indices...
		Transform transform;		 // location/orientation
		Material material;			// light data
		TextureId diffuseMapId;		// diffuse texture of the object (base coloring)
		TextureId specMapId;		// specular texture of the object (lighting information)
		TextureId normMapId;		// normal texture of the object (extra details added to object without adding vertices)
		TextureId bumpMapId;		// bump texture adds geometry offsets to objects 
	};
}