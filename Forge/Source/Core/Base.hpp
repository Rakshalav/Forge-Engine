#pragma once

#include <memory>

namespace Forge
{
	template<class T>
	using Ref = std::shared_ptr<T>;
	template<class T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<class T>
	using Scope = std::shared_ptr<T>;
	template<class T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}