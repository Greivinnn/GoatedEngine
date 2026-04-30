#include "ShapeStates.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;

void ShapeStates::Initialize()
{
	CreateShapes();	

	auto device = GraphicSystem::Get()->GetDevice();

	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * mVertices.size());
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = mVertices.data();

	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);

	ASSERT(SUCCEEDED(hr), "MeshBuffer: Failed to create vertex buffer.");

	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoSomething.fx";
}

void ShapeStates::Terminate()
{
}

void ShapeStates::Update(float deltaTime)
{
}

void ShapeStates::Render()
{
}

void ShapeStates::CreateShapes()
{
	mVertices.push_back({ { -0.5f, -0.5f, 0.0f } });
	mVertices.push_back({ { 0.0f, 0.5f, 0.0f } });
	mVertices.push_back({ { 0.5f, -0.5f, 0.0f } });


}
