# MSPDBX
This library provides a robust framework for working with Program Database (PDB) files, enabling efficient parsing, symbol resolution, and data management. It includes structures for handling PDB headers and symbols, functionality to download PDB files from remote sources, and tools for resolving symbols to their respective relative virtual addresses (RVAs) in a PE (Portable Executable) format. Designed for ease of use, it offers a clean interface for iterating through symbols, accessing data streams, and managing PDB file operations, making it ideal for developers engaged in debugging, reverse engineering, or static analysis of binaries.
## Samples
### From Raw PDB Buffer
```cpp
#include <MSPDBX/PDBContainer.h>
#include <MSPDBX/PDBSymRVAResolver.h>
...
std::vector<std::uint8_t> rawPdb {...};
MSPDBX::PDBContainer pdbContainer(rawPdb);
MSPDBX::PDBSymRVAResolver symResolver(yourPeBaseAddr, pdbContainer.mPDB);
...
```
### From Raw PDB Buffer Pointer ###
```cpp
#include <MSPDBX/PDB.h>
#include <MSPDBX/PDBSymRVAResolver.h>
...
MSPDBX::PDB pdb(yourPdbBuffPtr);
MSPDBX::PDBSymRVAResolver symResolver(yourPeBaseAddr, pdb);
...
```
### Download From Microsoft PDB Store
```cpp
#include <MSPDBX/PDBDownload.h>
...
auto pdbContainer = MSPDBX::MSDownloadPDB("KERNEL32.dll");
if(!pdbContainer) /*Error Handle*/ return 1;
MSPDBX::PDBSymRVAResolver symResolver(GetModuleHandleA("KERNEL32.dll"), (*pdbContainer).mPDB);
...
```
## Credits
[Original skeleton](https://gist.github.com/namazso/4bfafdb0233f72f5d13bfee825c203f7) by @namazso
