#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <vector>

namespace fg
{
	class Task
	{
	public:
		virtual ~Task() = default;
		virtual void Execute() = 0;

		void AddDependent(Task* dependent)
		{
			dependent->DependenciesRemaining.fetch_add(1, std::memory_order_relaxed);
			std::lock_guard<std::mutex> lock(m_DependentsMutex);
			Dependents.push_back(dependent);
		}

		void AddDependents(Task* dependent)
		{
			AddDependent(dependent);
		}

		std::vector<Task*> GetDependents() const
		{
			std::lock_guard<std::mutex> lock(m_DependentsMutex);
			return Dependents;
		}

		std::atomic<int> DependenciesRemaining = 0;

	private:
		mutable std::mutex m_DependentsMutex;
		std::vector<Task*> Dependents;
	};

	class FunctionTask final : public Task
	{
	public:
		explicit FunctionTask(std::function<void()> fn) : m_Function(std::move(fn)) {}

		void Execute() override
		{
			if (m_Function)
				m_Function();
		}

	private:
		std::function<void()> m_Function;
	};
}