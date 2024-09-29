#pragma once

#include <MSPDBX/PDB.h>
#include <optional>
#include <unordered_map>
#include <string>
#include <functional>

namespace MSPDBX {
	struct PDBSymRVAResolver {
		template<typename TPE>
		inline PDBSymRVAResolver(TPE pe, const PDB& pdb)
			: mInst((void*)pe)
			, mPDB(pdb)
		{
			Initialize();
		}

		std::optional<std::uint32_t> Resolve(const std::string& forSymbol);
		void ForEach(std::function<bool(const std::string&, std::uint32_t)> callback);
	private:
		void Initialize();

		void* mInst;
		const PDB& mPDB;
		std::unordered_map<std::string, std::uint32_t> mAllSymsRVA;
	};
}