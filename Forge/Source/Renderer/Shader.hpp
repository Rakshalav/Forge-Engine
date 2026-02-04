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

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetBool(const std::string& name, bool value) = 0;

		virtual void SetVec2(const std::string& name, const Vec2f& value) = 0;
		virtual void SetVec3(const std::string& name, const Vec3f& value) = 0;
		virtual void SetVec4(const std::string& name, const Vec4f& value) = 0;

		virtual void SetMat2(const std::string& name, const glm::mat2& value) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		static Ref<Shader> Create(const std::string& vertexpath, const std::string& fragmentpath);
	};
}