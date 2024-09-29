#pragma once

#include <vector>
#include <MSPDBX/PDB.h>

namespace MSPDBX {
	struct PDBContainer {
		PDBContainer(const std::vector<std::uint8_t>& pdb);

		PDB& operator*();

		std::vector<std::uint8_t> mStg;
		PDB mPDB;
	};
}