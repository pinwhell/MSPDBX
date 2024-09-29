#pragma once

#include <optional>
#include <vector>
#include <string>

// Make sure to link with `wininet.lib`

namespace MSPDBX {
	std::optional<std::vector<std::uint8_t>> DownloadFileToBuffer(const std::string& url);
}


