#include <MSPDBX/PDBUtility.h>
#include <MSPDBX/WinFileDownloader.h>
#include <MSPDBX/PDBContainer.h>
#include <MSPDBX/PDBDownload.h>
#include <guiddef.h>
#include <Windows.h>

using namespace MSPDBX;

#pragma pack(push, 1)
// CodeView structure (RSDS)
struct CV_INFO_PDB70 {
	DWORD  Signature;
	GUID   Guid;        // Debug GUID
	DWORD  Age;
	char   PdbFileName[1];  // Null-terminated string
};
#pragma pack(pop)

std::string GUIDToString(const GUID& guid) {
	char buffer[64] = { 0 };
	snprintf(buffer, sizeof(buffer),
		"%08X%04X%04X%04X%012llX",
		guid.Data1, guid.Data2, guid.Data3,
		(guid.Data4[0] << 8) + guid.Data4[1],
		((uint64_t)guid.Data4[2] << 40) |
		((uint64_t)guid.Data4[3] << 32) |
		((uint64_t)guid.Data4[4] << 24) |
		((uint64_t)guid.Data4[5] << 16) |
		((uint64_t)guid.Data4[6] << 8) |
		((uint64_t)guid.Data4[7]));

	return std::string(buffer);
}

std::optional<PDBDownloadInfo> MSPDBX::FindPDBDownloadInfo(HMODULE hModule) {
	if (!hModule)
		return std::nullopt;

	// Get the DOS header
	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)hModule;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return std::nullopt;

	// Get the NT headers
	IMAGE_NT_HEADERS* pNtHeaders = (IMAGE_NT_HEADERS*)((BYTE*)hModule + pDosHeader->e_lfanew);
	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)
		return std::nullopt;

	// Get the Debug Directory entry
	IMAGE_DATA_DIRECTORY debugDataDir = pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG];
	if (debugDataDir.VirtualAddress == 0)
		return std::nullopt;

	// Get the Debug Directory
	IMAGE_DEBUG_DIRECTORY* pDebugDir = (IMAGE_DEBUG_DIRECTORY*)((BYTE*)hModule + debugDataDir.VirtualAddress);
	DWORD debugDirCount = debugDataDir.Size / sizeof(IMAGE_DEBUG_DIRECTORY);

	// Loop through all debug directory entries
	for (DWORD i = 0; i < debugDirCount; i++) {
		// Check for CodeView signature
		if (pDebugDir[i].Type != IMAGE_DEBUG_TYPE_CODEVIEW)
			continue;

		// Get the CodeView information
		CV_INFO_PDB70* pCvInfo = (CV_INFO_PDB70*)((BYTE*)hModule + pDebugDir[i].AddressOfRawData);

		// Ensure it is RSDS (PDB v7.0) format
		if (pCvInfo->Signature != 'SDSR')
			continue;

		return PDBDownloadInfo{
			GUIDToString(pCvInfo->Guid),
			std::string(pCvInfo->PdbFileName),
			std::to_string(pCvInfo->Age)
		};
	}

	return std::nullopt;
}

std::optional<std::vector<std::uint8_t>> MSPDBX::MSDownloadPDBFromInfo(const PDBDownloadInfo& downloadInfo)
{
	std::string fullDownloadUrl =
		"http://msdl.microsoft.com/download/symbols/" +
		downloadInfo.mPDBFile +
		"/" +
		downloadInfo.mGUID +
		downloadInfo.mPDBAge +
		"/" +
		downloadInfo.mPDBFile;

	return DownloadFileToBuffer(fullDownloadUrl);
}

std::optional<std::vector<std::uint8_t>> MSPDBX::MSDownloadPDB(HMODULE hMod)
{
	auto ntdllPdbDownloadInfo = FindPDBDownloadInfo(hMod);
	if (!ntdllPdbDownloadInfo)
		return std::nullopt;
	return MSDownloadPDBFromInfo(*ntdllPdbDownloadInfo);
}

std::optional<PDBContainer> MSPDBX::MSDownloadPDB(const std::string& loadedDllName)
{
	auto dll = GetModuleHandleA(loadedDllName.c_str());
	if (!dll) return std::nullopt;
	auto pdb = MSDownloadPDB(dll); if (!pdb) return std::nullopt;
	return PDBContainer((*pdb));
}