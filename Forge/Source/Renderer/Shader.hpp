#pragma once

#include <string>
#include "../Core/Base.hpp"
#include "../Source/Maths/Math.hpp"

namespace fg
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void SetInt(std::string name, int value) = 0;
		virtual void SetFloat(std::string name, float value) = 0;
		virtual void SetBool(std::string name, bool value) = 0;

		virtual void SetVec2(std::string name, Vec2f value) = 0;
		virtual void SetVec3(std::string name, Vec3f value) = 0;
		virtual void SetVec4(std::string name, Vec4f value) = 0;

		virtual void SetMat2(std::string name, glm::mat2 value) = 0;
		virtual void SetMat3(std::string name, glm::mat3 value) = 0;
		virtual void SetMat4(std::string name, glm::mat4 value) = 0;

		static Ref<Shader> Create(const std::string& vertexpath, const std::string& fragmentpath);
	};
}