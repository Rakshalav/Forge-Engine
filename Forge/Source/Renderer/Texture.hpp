#pragma once

#include <Maths/Math.hpp>
#include <string>
#include "AssetManager/Asset.hpp"

namespace fg
{
	struct TextureSpecification
	{
		uint32_t Width = 1;
		uint32_t Height = 1;

		enum class InternalFormat : uint8_t
		{
			None = 0,
			R8,    
			RG8,
			RGB8,         
			RGBA8,        

			SRGB8,        
			SRGBA8,       

			R16F,         
			RG16F,        
			RGB16F,       
			RGBA16F,      

			R32F,    
			RG32F,
			RGB32F,       
			RGBA32F,  

			R32I,
			RG32I,
			RGB32I,
			RGBA32I,

			DEPTH24,
			DEPTH24STENSIL8
		};

		enum class Wrap : uint8_t { Repeat = 1, ClampToEdge, MirroredRepeat };
		enum class Filter : uint8_t { Linear = 1, Nearest, LinearMipmapLinear };

		InternalFormat Format = InternalFormat::None;
		Wrap WrapMode = Wrap::Repeat;
		Filter FilterMode = Filter::Linear;
		bool GenerateMipMap = true;
	};

	class Texture2D : public Asset
	{
	public:
		static Ref<Texture2D> Create(const TextureSpecification& specification);

		virtual void Bind() const = 0;
		virtual void Activate(uint32_t slot = 0) const = 0;
		virtual void SetData(void* data, uint32_t size) = 0;
		virtual Vec2u GetSize() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		AssetType GetType() const override { return AssetType::Texture2D; }
		virtual const TextureSpecification& GetSpecification() const = 0;
		virtual bool operator== (const Texture2D& other) const = 0;
	};
}