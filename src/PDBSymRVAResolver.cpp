#include <MSPDBX/PDBSymRVAResolver.h>
#include <MSPDBX/PEUtility.h>
#include <stdexcept>

using namespace MSPDBX;

void PDBSymRVAResolver::Initialize()
{
	auto allntSegs = PEGetAllSegments((HINSTANCE)mInst);
	if (!allntSegs) throw std::invalid_argument("");
	mPDB.ForEachSymbol([&](const auto& sym) {
		auto it = (*allntSegs).find(sym.seg - 1);
		if (it == (*allntSegs).end())
			return true;
		mAllSymsRVA[sym.name] = it->second + sym.off;
		return true;
		});
}

std::optional<std::uint32_t> PDBSymRVAResolver::Resolve(const std::string& forSymbol)
{
	auto it = mAllSymsRVA.find(forSymbol);
	if (it == mAllSymsRVA.end())
		return std::nullopt;
	return it->second;
}

void PDBSymRVAResolver::ForEach(std::function<bool(const std::string&, std::uint32_t)> callback)
{
	for (const auto& kv : mAllSymsRVA)
		if (!callback(kv.first, kv.second))
			break;
}