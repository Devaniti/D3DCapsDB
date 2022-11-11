#include "PrecompiledHeader.h"
#include "DXGIInfo.h"

// Returns 0 on error
int64_t GetUMDVersion(const ComPtr<IDXGIAdapter>& adapter)
{
    LARGE_INTEGER umdVersion;
    HRESULT hr = adapter->CheckInterfaceSupport(__uuidof(IDXGIDevice), &umdVersion);
    if (FAILED(hr))
    {
        return 0;
    }

    return umdVersion.QuadPart;
}

// Returns false on error
bool Query_DXGI_ADAPTER_DESC(const ComPtr<IDXGIAdapter>& adapter, JSONStorage& submission)
{
    // DXGI_ADAPTER_DESC
    DXGI_ADAPTER_DESC dxgiDesc{};
    HRESULT hr = adapter->GetDesc(&dxgiDesc);
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"DXGI_ADAPTER_DESC");
    submission.AddEntry(L"Description", dxgiDesc.Description);
    submission.AddEntry(L"VendorId", dxgiDesc.VendorId);
    submission.AddEntry(L"DeviceId", dxgiDesc.DeviceId);
    submission.AddEntry(L"SubSysId", dxgiDesc.SubSysId);
    submission.AddEntry(L"Revision", dxgiDesc.Revision);
    submission.AddEntry(L"DedicatedVideoMemory", dxgiDesc.DedicatedVideoMemory);
    submission.AddEntry(L"DedicatedSystemMemory", dxgiDesc.DedicatedSystemMemory);
    submission.AddEntry(L"SharedSystemMemory", dxgiDesc.SharedSystemMemory);
    // We purpously omit AdapterLuid, since it doesn't make sense to submit that
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_DXGI_ADAPTER_DESC1(const ComPtr<IDXGIAdapter>& adapter, JSONStorage& submission)
{
    ComPtr<IDXGIAdapter1> adapter1;
    HRESULT hr = adapter->QueryInterface(IID_PPV_ARGS(&adapter1));
    if (FAILED(hr))
    {
        return false;
    }

    DXGI_ADAPTER_DESC1 dxgiDesc1{};
    hr = adapter1->GetDesc1(&dxgiDesc1);
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"DXGI_ADAPTER_DESC1");
    submission.AddEntry(L"Flags", dxgiDesc1.Flags);
    submission.PopStruct();
    return true;
}

// Returns false on error
bool Query_DXGI_ADAPTER_DESC2(const ComPtr<IDXGIAdapter>& adapter, JSONStorage& submission)
{
    ComPtr<IDXGIAdapter2> adapter2;
    HRESULT hr = adapter->QueryInterface(IID_PPV_ARGS(&adapter2));
    if (FAILED(hr))
    {
        return false;
    }

    DXGI_ADAPTER_DESC2 dxgiDesc2{};
    hr = adapter2->GetDesc2(&dxgiDesc2);
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"DXGI_ADAPTER_DESC2");
    submission.AddEntry(L"GraphicsPreemptionGranularity", dxgiDesc2.GraphicsPreemptionGranularity);
    submission.AddEntry(L"ComputePreemptionGranularity", dxgiDesc2.ComputePreemptionGranularity);
    submission.PopStruct();
    return true;
}

// Returns false on error
bool Query_DXGI_ADAPTER_DESC3(const ComPtr<IDXGIAdapter>& adapter, JSONStorage& submission)
{
    ComPtr<IDXGIAdapter4> adapter4;
    HRESULT hr = adapter->QueryInterface(IID_PPV_ARGS(&adapter4));
    if (FAILED(hr))
    {
        return false;
    }

    DXGI_ADAPTER_DESC3 dxgiDesc3{};
    hr = adapter4->GetDesc3(&dxgiDesc3);
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"DXGI_ADAPTER_DESC3");
    submission.AddEntry(L"Flags", dxgiDesc3.Flags);
    submission.PopStruct();

    return true;
}

// Returns false if fail to query required info
bool QueryDXGIInfo(const ComPtr<IDXGIAdapter>& adapter, JSONStorage& submission)
{
    int64_t umdVersion = GetUMDVersion(adapter);
    if (!umdVersion)
    {
        return false;
    }

    submission.AddEntry(L"UMDVersion", umdVersion);

    if (!Query_DXGI_ADAPTER_DESC(adapter, submission))
    {
        return false;
    }

    Query_DXGI_ADAPTER_DESC1(adapter, submission);
    Query_DXGI_ADAPTER_DESC2(adapter, submission);
    Query_DXGI_ADAPTER_DESC3(adapter, submission);

    return true;
}
