#pragma once

namespace GoatedEngine::Graphics
{
	class Texture
	{
	public:
		// this removes reference of texture in shader
		static void UnbindPS(uint32_t slot);

		Texture() = default;
		virtual ~Texture();

		// delete copy constructor 
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		// allow move with noexcept 
		Texture(Texture&&) noexcept; 
		Texture& operator=(Texture&& rhs) noexcept; 

		virtual void Initialize(const std::filesystem::path& fileName);
		virtual void Terminate();

		void BindVS(uint32_t slot) const;
		void BindPS(uint32_t slot) const; 
	private:
		ID3D11ShaderResourceView* mShaderResourceView = nullptr;
	};
}