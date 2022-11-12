#include "PrecompiledHeader.h"
#include "OSInfo.h"

#pragma comment(lib, "version.lib")

// Returns 0 on error
int64_t GetWindowsVersion()
{
    const wchar_t* systemFile = L"kernelbase.dll";
    DWORD dummy = 0;

    const DWORD versionInfoSize = ::GetFileVersionInfoSizeExW(FILE_VER_GET_NEUTRAL, systemFile, &dummy);

    std::vector<char> buffer(versionInfoSize);
    BOOL res = ::GetFileVersionInfoExW(FILE_VER_GET_NEUTRAL, systemFile, dummy, (DWORD)buffer.size(), buffer.data());
    if (!res)
    {
        return 0;
    }

    void* p = nullptr;
    UINT size = 0;
    res = ::VerQueryValueW(buffer.data(), L"\\", &p, &size);
    if (!res)
    {
        return 0;
    }

    const VS_FIXEDFILEINFO* fixedFileInfo = static_cast<const VS_FIXEDFILEINFO*>(p);

    return (int64_t(fixedFileInfo->dwFileVersionMS) << 32) | fixedFileInfo->dwFileVersionLS;
}
