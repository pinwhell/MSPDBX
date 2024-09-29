#include <MSPDBX/PDBContainer.h>

using namespace MSPDBX;

PDBContainer::PDBContainer(const std::vector<std::uint8_t>& pdb)
	: mStg(pdb)
	, mPDB(pdb.data())
{}

PDB& PDBContainer::operator*()
{
	return mPDB;
}
