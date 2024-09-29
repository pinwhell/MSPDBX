#include <MSPDBX/PEUtility.h>

using namespace MSPDBX;

std::optional<std::unordered_map<size_t, DWORD>> MSPDBX::PEGetAllSegments(const IMAGE_NT_HEADERS* ntHeaders) {
	// Check if the passed IMAGE_NT_HEADERS pointer is valid
	if (!ntHeaders || ntHeaders->Signature != IMAGE_NT_SIGNATURE) {
		return std::nullopt; // Invalid state, return nullopt
	}

	// Create a map to hold segment RVA by index
	std::unordered_map<size_t, DWORD> segmentRVA;

	// Get the number of sections from the file header
	size_t sectionCount = ntHeaders->FileHeader.NumberOfSections;
	IMAGE_SECTION_HEADER* sectionHeaders = IMAGE_FIRST_SECTION(ntHeaders);

	// Loop through each section to retrieve the RVA
	for (size_t i = 0; i < sectionCount; ++i) {
		DWORD rva = sectionHeaders[i].VirtualAddress; // Get the RVA of the section
		segmentRVA[i] = rva; // Store it in the map with the index as the key
	}

	return segmentRVA; // Return the map containing segment RVA by index
}

std::optional<std::unordered_map<size_t, DWORD>> MSPDBX::PEGetAllSegments(HINSTANCE hInstance) {
	// Check if the HINSTANCE is valid
	if (!hInstance) {
		return std::nullopt; // Invalid HINSTANCE, return nullopt
	}

	// Get the DOS header
	PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(hInstance);

	// Check if the DOS header is valid
	if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		return std::nullopt; // Not a valid PE file, return nullopt
	}

	// Get the NT headers
	PIMAGE_NT_HEADERS ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(
		reinterpret_cast<BYTE*>(hInstance) + dosHeader->e_lfanew);

	// Validate NT headers
	if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) {
		return std::nullopt; // Not a valid NT headers, return nullopt
	}

	// Call the original function with the valid NT headers
	return PEGetAllSegments(ntHeaders);
}