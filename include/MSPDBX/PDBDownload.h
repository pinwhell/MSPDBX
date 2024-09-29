#pragma once

#include <MSPDBX/PDBContainer.h>
#include <optional>
#include <string>

namespace MSPDBX {
	std::optional<PDBContainer> MSDownloadPDB(const std::string& loadedDllName);
}