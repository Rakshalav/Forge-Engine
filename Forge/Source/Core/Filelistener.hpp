#pragma once

#include <efsw/efsw.hpp>
#include <filesystem>
#include "Core/Application.hpp"
#include "Event/Event.hpp"

namespace fg
{
	class UpdateListener : public efsw::FileWatchListener
	{
	public:
		void handleFileAction(efsw::WatchID watchid, const std::string& dir, 
			const std::string& filename, efsw::Action action,
			const std::string& oldFilename) override
		{
			Event event = Event::FileChangeCallback(
				(int)action,
				std::filesystem::path(dir).generic_string(),
				std::filesystem::path(filename).generic_string(),
				std::filesystem::path(oldFilename).generic_string());
			Application::Get().RaiseEvent(event);
		}
	};
}
