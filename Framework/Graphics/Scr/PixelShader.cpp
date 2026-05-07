#include "Precompiled.h"
#include "PixelShader.h"
#include "GraphicSystem.h"

using namespace GoatedEngine;
using namespace GoatedEngine::Graphics;

void PixelShader::Initialize(const std::filesystem::path& shaderPath)
{
	auto device = GraphicSystem::Get()->GetDevice();

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	// pixel shader
	HRESULT hr = D3DCompileFromFile(
		shaderPath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlags, 0,
		&shaderBlob,
		&errorBlob);
	if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}
	ASSERT(SUCCEEDED(hr), "PixelShader: failed to compile pixel shader");

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader);
	ASSERT(SUCCEEDED(hr), "PixelShader: failed to create pixel shader");
	SafeRealease(shaderBlob);
	SafeRealease(errorBlob);
}

void PixelShader::Terminate()
{
	SafeRealease(mPixelShader);
}

void PixelShader::Bind()
{
	auto context = GraphicSystem::Get()->GetContext();
	context->PSSetShader(mPixelShader, nullptr, 0);
}