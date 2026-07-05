#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <shlobj.h>
#include <shellapi.h>  
#elif __linux__
#include <cstdio>
#endif

#include <cstring>
#include <algorithm>
#include <iostream>
#include <string>
#include <filesystem>

namespace fg::Utility
{
	inline std::string ToForwardSlashPath(std::string path)
	{
		std::replace(path.begin(), path.end(), '\\', '/');
		return path;
	}

#ifdef _WIN32
	inline bool CopyToClipboard(const std::string& text)
	{
		if (!OpenClipboard(nullptr))
			return false;

		EmptyClipboard();

		size_t size = text.size() + 1;
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
		if (!hMem) {
			CloseClipboard();
			return false;
		}

		std::memcpy(GlobalLock(hMem), text.c_str(), size);
		GlobalUnlock(hMem);

		SetClipboardData(CF_TEXT, hMem);

		CloseClipboard();

		return true;
	}

	inline bool CopyToClipBoard(const std::string& text)
	{
		return CopyToClipboard(text);
	}

	inline void OpenInExplorer(const std::filesystem::path& fullPath)
	{
		ShellExecuteW(NULL, L"open", fullPath.wstring().c_str(), NULL, NULL, SW_SHOWNORMAL);
	}

	inline void ShowInExplorer(const std::filesystem::path& fullPath)
	{
		PIDLIST_ABSOLUTE pidl = ILCreateFromPathW(fullPath.wstring().c_str());
		if (pidl)
		{
			SHOpenFolderAndSelectItems(pidl, 0, NULL, 0);
			ILFree(pidl);
		}
	}

#elif __linux__
	inline bool CopyToClipboard(const std::string& text) 
	{
		FILE* pipe = popen("xclip -selection clipboard", "w");
		if (!pipe) {
			std::cerr << "Error: Could not open pipe to xclip. Is it installed?\n";
			return false;
		}

		std::fputs(text.c_str(), pipe);

		pclose(pipe);
		return true;
	}
#endif 
}
