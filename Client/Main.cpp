#include "PrecompiledHeader.h"

#include "JSONStorage.h"
#include "OSInfo.h"
#include "DXGIInfo.h"
#include "D3D12Caps.h"
#include "PageOutput.h"

enum class ReturnCodes : int
{
    OK = 0,
    CouldntQueryWindowsVersion = 1,
    CouldntCreateDXGIFactory = 2,
    NoCompatibleGPUs = 3,
    FileSystemError = 4
};

const int64_t CurrentReportVersion = 0;

int main()
{
    int64_t windowsVersion = GetWindowsVersion();
    if (!windowsVersion)
    {
        assert(0);
        return (int)ReturnCodes::CouldntQueryWindowsVersion;
    }

    ComPtr<IDXGIFactory> factory;
    HRESULT hr = ::CreateDXGIFactory1(IID_PPV_ARGS(&factory));
    if (FAILED(hr))
    {
        return (int)ReturnCodes::CouldntCreateDXGIFactory;
    }

    std::vector<JSONStorage> submissions;

    UINT adapterIndex = 0;
    while (true)
    {
        ComPtr<IDXGIAdapter> adapter;
        hr = factory->EnumAdapters(adapterIndex++, &adapter);
        if (FAILED(hr))
        {
            break;
        }

        JSONStorage submission{};

        submission.AddEntry(L"ReportVersion", CurrentReportVersion);
        submission.AddEntry(L"OSVersion", windowsVersion);

        if (!QueryDXGIInfo(adapter, submission))
        {
            break;
        }
        QueryD3D12Caps(adapter, submission);

        submissions.push_back(std::move(submission));
    }

    if (submissions.empty())
    {
        assert(0);
        return (int)ReturnCodes::NoCompatibleGPUs;
    }

    if (!GenerateAndOpenOutputPage(submissions))
    {
        assert(0);
        return (int)ReturnCodes::FileSystemError;
    }
    return (int)ReturnCodes::OK;
}