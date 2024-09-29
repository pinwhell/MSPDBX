#pragma once

#include <cstdint>
#include <vector>
#include <functional>

namespace MSPDBX {
#pragma pack(push, 1)
	struct PDB {
		struct DBIHeader
		{
			std::int32_t	VersionSignature;
			std::uint32_t	VersionHeader;
			std::uint32_t	Age;
			std::uint16_t	GlobalStreamIndex;
			std::uint16_t	BuildNumber;
			std::uint16_t	PublicStreamIndex;
			std::uint16_t	PdbDllVersion;
			std::uint16_t	SymRecordStream;
			std::uint16_t	PdbDllRbld;
			std::int32_t	ModInfoSize;
			std::int32_t	SectionContributionSize;
			std::int32_t	SectionMapSize;
			std::int32_t	SourceInfoSize;
			std::int32_t	TypeServerSize;
			std::uint32_t	MFCTypeServerIndex;
			std::int32_t	OptionalDbgHeaderSize;
			std::int32_t	ECSubstreamSize;
			std::uint16_t	Flags;
			std::uint16_t	Machine;
			std::uint32_t	Padding;
		};

		struct SuperBlock
		{
			constexpr static char kMagic[] =
			{
				0x4D, 0x69, 0x63, 0x72, 0x6F, 0x73, 0x6F, 0x66, 0x74, 0x20, 0x43, 0x2F,
				0x43, 0x2B, 0x2B, 0x20, 0x4D, 0x53, 0x46, 0x20, 0x37, 0x2E, 0x30, 0x30,
				0x0D, 0x0A, 0x1A, 0x44, 0x53, 0x00, 0x00, 0x00
			};

			char          FileMagic[sizeof(kMagic)];
			std::uint32_t BlockSize;
			std::uint32_t FreeBlockMapBlock;
			std::uint32_t NumBlocks;
			std::uint32_t NumDirectoryBytes;
			std::uint32_t Unknown;
			std::uint32_t BlockMapAddr;

			bool is_magic_valid() const;
		};

		struct PUBSYM32
		{
			std::uint16_t reclen;     // Record length
			std::uint16_t rectyp;     // S_PUB32
			std::uint32_t pubsymflags;
			std::uint32_t off;
			std::uint16_t seg;
			char name[1];    // Length-prefixed name
		};

		enum { PDB_Symbol_PUB32 = 0x110e };

		PDB(const void* pPDB);
		const DBIHeader& GetDBI() const;
		void ForEachSymbol(std::function<bool(const PUBSYM32&)> callback) const;

		template<typename T>
		inline const T& GetStream(size_t index) const
		{
			return *reinterpret_cast<const T*>(mStreams[index].data());
		}

	private:

		union {
			SuperBlock* mpSB;
			const void* mvpPDB;
			uint8_t* mpbPDB;
		};
		std::vector<std::vector<std::uint8_t>> mStreams;
	};
#pragma pack(pop)
}