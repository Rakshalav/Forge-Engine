#pragma once

#include "Task.hpp"
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <deque>
#include <mutex>
#include <thread>
#include <vector>

namespace fg
{
	class TaskManager
	{
	public:
		explicit TaskManager(uint32_t threads);
		~TaskManager();

		void Submit(Task* task);
		void WaitIdle();

		static TaskManager& Get()
		{
			static TaskManager instance(std::thread::hardware_concurrency());
			return instance;
		}

		TaskManager(const TaskManager&) = delete;
		void operator=(const TaskManager&) = delete;

	private:
		bool HasRunnableTaskUnsafe() const;

	private:
		std::vector<std::thread> m_Workers;
		std::deque<Task*> m_TaskQueue;
		mutable std::mutex m_QueueMutex;
		std::condition_variable m_Condition;
		std::condition_variable m_IdleCondition;
		std::atomic<uint32_t> m_PendingTasks = 0;
		bool m_Stop = false;
	};
}
