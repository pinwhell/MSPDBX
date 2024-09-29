#include <stdexcept>
#include <MSPDBX/PDB.h>

using namespace MSPDBX;

bool PDB::SuperBlock::is_magic_valid() const
{
	return 0 == memcmp(FileMagic, kMagic, sizeof(kMagic));
}

PDB::PDB(const void* pPDB)
	: mvpPDB(pPDB)
{
	if (!mpSB->is_magic_valid())
		throw std::invalid_argument("");

	const auto size = mpSB->NumDirectoryBytes;
	const auto blkSz = mpSB->BlockSize;
	const auto blkCnt = (size + blkSz - 1) / blkSz;
	const auto blkIdArr = reinterpret_cast<uint32_t*>(mpbPDB + blkSz * mpSB->BlockMapAddr);
	std::vector<std::uint8_t> streamDir; streamDir.reserve(blkCnt * blkSz);

	for (auto i = 0u; i < blkCnt; ++i)
	{
		const auto blk = mpbPDB + blkSz * blkIdArr[i];
		streamDir.insert(streamDir.end(), blk, blk + blkSz);
	}

	streamDir.resize(size);

	auto ui32Iter = reinterpret_cast<uint32_t*>(streamDir.data());
	const auto streamNum = *ui32Iter++;
	const auto streamArr = ui32Iter;
	ui32Iter += streamNum;
	mStreams.reserve(streamNum);

	for (auto i = 0u; i < streamNum; ++i)
	{
		std::vector<std::uint8_t> currStream;
		const auto currStreamSz = streamArr[i];
		const auto currStreamBlkCount = (currStreamSz + blkSz - 1) / blkSz;
		currStream.reserve(currStreamBlkCount * blkSz);

		for (auto j = 0u; j < currStreamBlkCount; ++j)
		{
			const auto blkId = *ui32Iter++;
			const auto blk = mpbPDB + blkSz * blkId;
			currStream.insert(currStream.end(), blk, blk + blkSz);
		}

		currStream.resize(currStreamSz);
		mStreams.push_back(std::move(currStream));
	}
}

const PDB::DBIHeader& PDB::GetDBI() const
{
	return GetStream<DBIHeader>(3);
}

void PDB::ForEachSymbol(std::function<bool(const PUBSYM32&)> callback) const
{
	auto& symbols = mStreams[GetDBI().SymRecordStream];
	auto it = symbols.data();
	const auto end = it + symbols.size();
	while (it != end)
	{
		const auto curr = reinterpret_cast<const PUBSYM32*>(it);
		if (curr->rectyp == PDB_Symbol_PUB32 && !callback(*curr))
			break;
		it += curr->reclen + 2;
	}
}