#pragma once

#include <Windows.h>
#include <optional>
#include <unordered_map>

namespace MSPDBX {
	std::optional<std::unordered_map<size_t, DWORD>> PEGetAllSegments(const IMAGE_NT_HEADERS* ntHeaders);
	std::optional<std::unordered_map<size_t, DWORD>> PEGetAllSegments(HINSTANCE hInstance);
}