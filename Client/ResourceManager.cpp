#include "PrecompiledHeader.h"
#include "ResourceManager.h"

#include "resource.h"

std::pair<void*, size_t> GetHTMLPageCode()
{
    HRSRC hrsrc = ::FindResource(NULL, MAKEINTRESOURCE(IDR_FILE1), RT_RCDATA);
    if (hrsrc == NULL)
    {
        assert(0);
        return {};
    }
    DWORD size = ::SizeofResource(0, hrsrc);
    if (size == 0)
    {
        assert(0);
        return {};
    }
    HGLOBAL resource = ::LoadResource(0, hrsrc);
    if (resource == NULL)
    {
        assert(0);
        return {};
    }
    PVOID buff = ::LockResource(resource);
    if (buff == nullptr)
    {
        assert(0);
        return {};
    }
    return { buff, size };
}
