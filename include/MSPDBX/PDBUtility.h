#pragma once

#include <Windows.h>
#include <optional>
#include <string>
#include <vector>

namespace MSPDBX {
	struct PDBDownloadInfo {
		std::string mGUID;
		std::string mPDBFile;
		std::string mPDBAge;
	};

	inline std::optional<PDBDownloadInfo> FindPDBDownloadInfo(HMODULE hModule);
	std::optional<std::vector<std::uint8_t>> MSDownloadPDBFromInfo(const PDBDownloadInfo& downloadInfo);
	std::optional<std::vector<std::uint8_t>> MSDownloadPDB(HMODULE hMod);
}
