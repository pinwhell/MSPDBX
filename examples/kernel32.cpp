#include <MSPDBX/PDBDownload.h>
#include <MSPDBX/PDBSymRVAResolver.h>
#include <Windows.h>
#include <iostream>

using namespace MSPDBX;

int main()
{
    auto ctainer = MSDownloadPDB("KERNEL32.dll");
    if (!ctainer) return 1;
    PDBSymRVAResolver symRvaResolver(
        GetModuleHandleA("KERNEL32.dll"),
        (*ctainer).mPDB);
    std::cout << std::hex << *symRvaResolver.Resolve("VirtualAllocExStub") << std::endl;
    symRvaResolver.ForEach([&](const std::string& name, uint32_t rva) {
        printf("%08X -> %s\n", rva, name.c_str());
        return true;
        });
    return 0;
}