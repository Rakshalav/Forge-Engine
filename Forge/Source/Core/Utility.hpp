#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#elif __linux__
#include <cstdio>
#endif

#include <cstring>
#include <iostream>
#include <string>

namespace fg::Utility
{
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
