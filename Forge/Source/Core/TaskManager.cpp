#include "TaskManager.hpp"

namespace fg
{
	TaskManager::TaskManager(uint32_t threads)
	{
		if (threads == 0)
			threads = 1;

		for (size_t i = 0; i < threads; ++i)
		{
			m_Workers.emplace_back([this]
				{
					while (true)
					{
						Task* task = nullptr;
						{
							std::unique_lock<std::mutex> lock(m_QueueMutex);
							m_Condition.wait(lock, [this]
								{
									return m_Stop || HasRunnableTaskUnsafe();
								});

							if (m_Stop && !HasRunnableTaskUnsafe())
								return;

							for (auto it = m_TaskQueue.begin(); it != m_TaskQueue.end(); ++it)
							{
								if ((*it)->DependenciesRemaining.load(std::memory_order_acquire) == 0)
								{
									task = *it;
									m_TaskQueue.erase(it);
									break;
								}
							}
						}

						if (!task)
							continue;

						task->Execute();

						for (Task* dependent : task->GetDependents())
							dependent->DependenciesRemaining.fetch_sub(1, std::memory_order_release);

						delete task;

						if (m_PendingTasks.fetch_sub(1, std::memory_order_acq_rel) == 1)
						{
							std::lock_guard<std::mutex> idleLock(m_QueueMutex);
							m_IdleCondition.notify_all();
						}

						m_Condition.notify_all();
					}
				});
		}
	}

	TaskManager::~TaskManager()
	{
		WaitIdle();
		{
			std::lock_guard<std::mutex> lock(m_QueueMutex);
			m_Stop = true;
		}

		m_Condition.notify_all();
		for (std::thread& worker : m_Workers)
		{
			if (worker.joinable())
				worker.join();
		}

		while (!m_TaskQueue.empty())
		{
			delete m_TaskQueue.front();
			m_TaskQueue.pop_front();
		}
	}

	void TaskManager::Submit(Task* task)
	{
		if (!task)
			return;

		{
			std::lock_guard<std::mutex> lock(m_QueueMutex);
			m_PendingTasks.fetch_add(1, std::memory_order_relaxed);
			m_TaskQueue.push_back(task);
		}

		m_Condition.notify_all();
	}

	void TaskManager::WaitIdle()
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);
		m_IdleCondition.wait(lock, [this]
			{
				return m_PendingTasks.load(std::memory_order_acquire) == 0;
			});
	}

	bool TaskManager::HasRunnableTaskUnsafe() const
	{
		for (Task* task : m_TaskQueue)
		{
			if (task->DependenciesRemaining.load(std::memory_order_acquire) == 0)
				return true;
		}
		return false;
	}
}