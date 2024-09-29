#include <MSPDBX/WinFileDownloader.h>
#include <Windows.h>
#include <wininet.h>

// #pragma comment(lib, "wininet.lib")

std::optional<std::vector<std::uint8_t>> MSPDBX::DownloadFileToBuffer(const std::string& url) {
	HINTERNET hInternet = InternetOpenA("Downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!hInternet) return std::nullopt;

	HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD, 0);
	if (!hUrl) {
		InternetCloseHandle(hInternet);
		return std::nullopt;
	}

	std::vector<std::uint8_t> buffer;
	std::uint8_t tempBuffer[4096];
	DWORD bytesRead = 0;

	while (InternetReadFile(hUrl, tempBuffer, sizeof(tempBuffer), &bytesRead) && bytesRead != 0) {
		buffer.insert(buffer.end(), tempBuffer, tempBuffer + bytesRead);
	}

	InternetCloseHandle(hUrl);
	InternetCloseHandle(hInternet);

	if (buffer.empty()) return std::nullopt;
	return buffer;
}