#include "PrecompiledHeader.h"

#include "D3D12Caps.h"

// Returns false on error
bool Query_D3D12_FEATURE_DATA_D3D12_OPTIONS(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_D3D12_OPTIONS featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_D3D12_OPTIONS");
    submission.AddEntry(L"DoublePrecisionFloatShaderOps", featureData.DoublePrecisionFloatShaderOps);
    submission.AddEntry(L"OutputMergerLogicOp", featureData.OutputMergerLogicOp);
    submission.AddEntry(L"MinPrecisionSupport", featureData.MinPrecisionSupport);
    submission.AddEntry(L"TiledResourcesTier", featureData.TiledResourcesTier);
    submission.AddEntry(L"ResourceBindingTier", featureData.ResourceBindingTier);
    submission.AddEntry(L"PSSpecifiedStencilRefSupported", featureData.PSSpecifiedStencilRefSupported);
    submission.AddEntry(L"TypedUAVLoadAdditionalFormats", featureData.TypedUAVLoadAdditionalFormats);
    submission.AddEntry(L"ROVsSupported", featureData.ROVsSupported);
    submission.AddEntry(L"ConservativeRasterizationTier", featureData.ConservativeRasterizationTier);
    submission.AddEntry(L"MaxGPUVirtualAddressBitsPerResource", featureData.MaxGPUVirtualAddressBitsPerResource);
    submission.AddEntry(L"StandardSwizzle64KBSupported", featureData.StandardSwizzle64KBSupported);
    submission.AddEntry(L"CrossNodeSharingTier", featureData.CrossNodeSharingTier);
    submission.AddEntry(L"CrossAdapterRowMajorTextureSupported", featureData.CrossAdapterRowMajorTextureSupported);
    submission.AddEntry(L"VPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation", featureData.VPAndRTArrayIndexFromAnyShaderFeedingRasterizerSupportedWithoutGSEmulation);
    submission.AddEntry(L"ResourceHeapTier", featureData.ResourceHeapTier);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_ARCHITECTURE(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_ARCHITECTURE featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_ARCHITECTURE");
    submission.AddEntry(L"TileBasedRenderer", featureData.TileBasedRenderer);
    submission.AddEntry(L"UMA", featureData.UMA);
    submission.AddEntry(L"CacheCoherentUMA", featureData.CacheCoherentUMA);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_FEATURE_LEVELS(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    const D3D_FEATURE_LEVEL knownFeatureLevels[] =
    {
        D3D_FEATURE_LEVEL_9_1 ,
        D3D_FEATURE_LEVEL_9_2 ,
        D3D_FEATURE_LEVEL_9_3 ,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_2
    };

    D3D12_FEATURE_DATA_FEATURE_LEVELS featureData{};

    featureData.pFeatureLevelsRequested = knownFeatureLevels;
    featureData.NumFeatureLevels = ARRAYSIZE(knownFeatureLevels);

    HRESULT hr;
    while (featureData.NumFeatureLevels)
    {
        hr = device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureData, sizeof(featureData));
        if (SUCCEEDED(hr))
        {
            break;
        }

        // In case older windows versions error out on seeing too new feature level
        // We just crop the list and remove newest one, one by one
        --featureData.NumFeatureLevels;
    }
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_FEATURE_LEVELS");
    submission.AddEntry(L"MaxSupportedFeatureLevel", featureData.MaxSupportedFeatureLevel);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_GPU_VIRTUAL_ADDRESS_SUPPORT, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT");
    submission.AddEntry(L"MaxGPUVirtualAddressBitsPerResource", featureData.MaxGPUVirtualAddressBitsPerResource);
    submission.AddEntry(L"MaxGPUVirtualAddressBitsPerProcess", featureData.MaxGPUVirtualAddressBitsPerProcess);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_SHADER_MODEL(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    const D3D_SHADER_MODEL knownShaderModels[] =
    {
        D3D_SHADER_MODEL_6_7,
        D3D_SHADER_MODEL_6_6,
        D3D_SHADER_MODEL_6_5,
        D3D_SHADER_MODEL_6_4,
        D3D_SHADER_MODEL_6_3,
        D3D_SHADER_MODEL_6_2,
        D3D_SHADER_MODEL_6_1,
        D3D_SHADER_MODEL_6_0,
        D3D_SHADER_MODEL_5_1,
    };

    HRESULT hr;
    D3D12_FEATURE_DATA_SHADER_MODEL featureData{};
    for (const D3D_SHADER_MODEL currentShaderModel : knownShaderModels)
    {
        featureData.HighestShaderModel = currentShaderModel;
        hr = device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &featureData, sizeof(featureData));
        if (SUCCEEDED(hr))
        {
            break;
        }
    }

    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_SHADER_MODEL");
    submission.AddEntry(L"HighestShaderModel", featureData.HighestShaderModel);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_D3D12_OPTIONS1(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_D3D12_OPTIONS1 featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS1, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_D3D12_OPTIONS1");
    submission.AddEntry(L"WaveOps", featureData.WaveOps);
    submission.AddEntry(L"WaveLaneCountMin", featureData.WaveLaneCountMin);
    submission.AddEntry(L"WaveLaneCountMax", featureData.WaveLaneCountMax);
    submission.AddEntry(L"TotalLaneCount", featureData.TotalLaneCount);
    submission.AddEntry(L"ExpandedComputeResourceStates", featureData.ExpandedComputeResourceStates);
    submission.AddEntry(L"Int64ShaderOps", featureData.Int64ShaderOps);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_PROTECTED_RESOURCE_SESSION_SUPPORT(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_PROTECTED_RESOURCE_SESSION_SUPPORT featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_PROTECTED_RESOURCE_SESSION_SUPPORT, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_PROTECTED_RESOURCE_SESSION_SUPPORT");
    submission.AddEntry(L"Support", featureData.Support);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_ROOT_SIGNATURE(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    const D3D_ROOT_SIGNATURE_VERSION knownRootSigVersions[] =
    {
        D3D_ROOT_SIGNATURE_VERSION_1_1,
        D3D_ROOT_SIGNATURE_VERSION_1_0,
    };

    HRESULT hr;
    D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData{};
    for (const D3D_ROOT_SIGNATURE_VERSION currentRootSigVersion : knownRootSigVersions)
    {
        featureData.HighestVersion = currentRootSigVersion;
        hr = device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData));
        if (SUCCEEDED(hr))
        {
            break;
        }
    }

    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_ROOT_SIGNATURE");
    submission.AddEntry(L"HighestVersion", featureData.HighestVersion);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_ARCHITECTURE1(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_ARCHITECTURE1 featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE1, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_ARCHITECTURE1");
    submission.AddEntry(L"IsolatedMMU", featureData.IsolatedMMU);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_D3D12_OPTIONS2(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_D3D12_OPTIONS2 featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS2, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_D3D12_OPTIONS2");
    submission.AddEntry(L"DepthBoundsTestSupported", featureData.DepthBoundsTestSupported);
    submission.AddEntry(L"ProgrammableSamplePositionsTier", featureData.ProgrammableSamplePositionsTier);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_SHADER_CACHE(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_SHADER_CACHE featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_SHADER_CACHE, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_SHADER_CACHE");
    submission.AddEntry(L"SupportFlags", featureData.SupportFlags);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_COMMAND_QUEUE_PRIORITY(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    const std::pair<D3D12_COMMAND_LIST_TYPE, const wchar_t*> queriedCommandQueueTypes[] =
    {
        {D3D12_COMMAND_LIST_TYPE_DIRECT, L"TYPE_DIRECT"},
        {D3D12_COMMAND_LIST_TYPE_COMPUTE, L"TYPE_COMPUTE"},
        {D3D12_COMMAND_LIST_TYPE_COPY, L"TYPE_COPY"},
    };

    const std::pair<D3D12_COMMAND_QUEUE_PRIORITY, const wchar_t*> queriedCommandQueuePriorities[] =
    {
        {D3D12_COMMAND_QUEUE_PRIORITY_NORMAL, L"PRIORITY_NORMAL"},
        {D3D12_COMMAND_QUEUE_PRIORITY_HIGH, L"PRIORITY_HIGH"},
        {D3D12_COMMAND_QUEUE_PRIORITY_GLOBAL_REALTIME, L"PRIORITY_GLOBAL_REALTIME"},
    };

    submission.PushStruct(L"D3D12_FEATURE_DATA_COMMAND_QUEUE_PRIORITY");
    for (const auto& [cmdListType, cmdListTypeName] : queriedCommandQueueTypes)
    {
        submission.PushStruct(cmdListTypeName);
        for (const auto& [cmdListPriority, cmdListPriorityName] : queriedCommandQueuePriorities)
        {
            submission.PushStruct(cmdListPriorityName);

            D3D12_FEATURE_DATA_COMMAND_QUEUE_PRIORITY featureData{};
            featureData.CommandListType = cmdListType;
            featureData.Priority = cmdListPriority;
            HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_COMMAND_QUEUE_PRIORITY, &featureData, sizeof(featureData));
            if (SUCCEEDED(hr))
            {
                submission.AddEntry(L"PriorityForTypeIsSupported", featureData.PriorityForTypeIsSupported);
            }

            submission.PopStruct();
        }
        submission.PopStruct();
    }
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_D3D12_OPTIONS3(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_D3D12_OPTIONS3 featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS3, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_D3D12_OPTIONS3");
    submission.AddEntry(L"CopyQueueTimestampQueriesSupported", featureData.CopyQueueTimestampQueriesSupported);
    submission.AddEntry(L"CastingFullyTypedFormatSupported", featureData.CastingFullyTypedFormatSupported);
    submission.AddEntry(L"WriteBufferImmediateSupportFlags", featureData.WriteBufferImmediateSupportFlags);
    submission.AddEntry(L"ViewInstancingTier", featureData.ViewInstancingTier);
    submission.AddEntry(L"BarycentricsSupported", featureData.BarycentricsSupported);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_EXISTING_HEAPS(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_EXISTING_HEAPS featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_EXISTING_HEAPS, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_EXISTING_HEAPS");
    submission.AddEntry(L"Supported", featureData.Supported);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_D3D12_OPTIONS4(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_D3D12_OPTIONS4 featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS4, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_D3D12_OPTIONS4");
    submission.AddEntry(L"MSAA64KBAlignedTextureSupported", featureData.MSAA64KBAlignedTextureSupported);
    submission.AddEntry(L"SharedResourceCompatibilityTier", featureData.SharedResourceCompatibilityTier);
    submission.AddEntry(L"Native16BitShaderOpsSupported", featureData.Native16BitShaderOpsSupported);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_SERIALIZATION(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_SERIALIZATION featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_SERIALIZATION, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_SERIALIZATION");
    submission.AddEntry(L"HeapSerializationTier", featureData.HeapSerializationTier);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_CROSS_NODE(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_CROSS_NODE featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_CROSS_NODE, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_CROSS_NODE");
    submission.AddEntry(L"SharingTier", featureData.SharingTier);
    submission.AddEntry(L"AtomicShaderInstructions", featureData.AtomicShaderInstructions);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_D3D12_OPTIONS5(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_D3D12_OPTIONS5 featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_D3D12_OPTIONS5");
    submission.AddEntry(L"SRVOnlyTiledResourceTier3", featureData.SRVOnlyTiledResourceTier3);
    submission.AddEntry(L"RenderPassesTier", featureData.RenderPassesTier);
    submission.AddEntry(L"RaytracingTier", featureData.RaytracingTier);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_DISPLAYABLE(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_DISPLAYABLE featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_DISPLAYABLE, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_DISPLAYABLE");
    submission.AddEntry(L"DisplayableTexture", featureData.DisplayableTexture);
    submission.AddEntry(L"SharedResourceCompatibilityTier", featureData.SharedResourceCompatibilityTier);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_D3D12_OPTIONS6(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_D3D12_OPTIONS6 featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS6, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_D3D12_OPTIONS6");
    submission.AddEntry(L"AdditionalShadingRatesSupported", featureData.AdditionalShadingRatesSupported);
    submission.AddEntry(L"PerPrimitiveShadingRateSupportedWithViewportIndexing", featureData.PerPrimitiveShadingRateSupportedWithViewportIndexing);
    submission.AddEntry(L"VariableShadingRateTier", featureData.VariableShadingRateTier);
    submission.AddEntry(L"ShadingRateImageTileSize", featureData.ShadingRateImageTileSize);
    submission.AddEntry(L"BackgroundProcessingSupported", featureData.BackgroundProcessingSupported);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_D3D12_OPTIONS7(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_D3D12_OPTIONS7 featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_D3D12_OPTIONS7");
    submission.AddEntry(L"MeshShaderTier", featureData.MeshShaderTier);
    submission.AddEntry(L"SamplerFeedbackTier", featureData.SamplerFeedbackTier);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_D3D12_OPTIONS8(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_D3D12_OPTIONS8 featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS8, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_D3D12_OPTIONS8");
    submission.AddEntry(L"UnalignedBlockTexturesSupported", featureData.UnalignedBlockTexturesSupported);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_D3D12_OPTIONS9(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_D3D12_OPTIONS9 featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS9, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_D3D12_OPTIONS9");
    submission.AddEntry(L"MeshShaderPipelineStatsSupported", featureData.MeshShaderPipelineStatsSupported);
    submission.AddEntry(L"MeshShaderSupportsFullRangeRenderTargetArrayIndex", featureData.MeshShaderSupportsFullRangeRenderTargetArrayIndex);
    submission.AddEntry(L"AtomicInt64OnTypedResourceSupported", featureData.AtomicInt64OnTypedResourceSupported);
    submission.AddEntry(L"AtomicInt64OnGroupSharedSupported", featureData.AtomicInt64OnGroupSharedSupported);
    submission.AddEntry(L"DerivativesInMeshAndAmplificationShadersSupported", featureData.DerivativesInMeshAndAmplificationShadersSupported);
    submission.AddEntry(L"WaveMMATier", featureData.WaveMMATier);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_D3D12_OPTIONS10(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_D3D12_OPTIONS10 featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS10, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_D3D12_OPTIONS10");
    submission.AddEntry(L"VariableRateShadingSumCombinerSupported", featureData.VariableRateShadingSumCombinerSupported);
    submission.AddEntry(L"MeshShaderPerPrimitiveShadingRateSupported", featureData.MeshShaderPerPrimitiveShadingRateSupported);
    submission.PopStruct();

    return true;
}

// Returns false on error
bool Query_D3D12_FEATURE_DATA_D3D12_OPTIONS11(const ComPtr<ID3D12Device>& device, JSONStorage& submission)
{
    D3D12_FEATURE_DATA_D3D12_OPTIONS11 featureData{};
    HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS11, &featureData, sizeof(featureData));
    if (FAILED(hr))
    {
        return false;
    }

    submission.PushStruct(L"D3D12_FEATURE_DATA_D3D12_OPTIONS11");
    submission.AddEntry(L"AtomicInt64OnDescriptorHeapResourceSupported", featureData.AtomicInt64OnDescriptorHeapResourceSupported);
    submission.PopStruct();

    return true;
}

// Returns false if device creation fails
bool QueryD3D12Caps(const ComPtr<IDXGIAdapter>& adapter, JSONStorage& submission)
{
    ComPtr<ID3D12Device> device;
    HRESULT hr = ::D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
    if (FAILED(hr))
    {
        submission.AddEntry(L"D3D12Supported", false);
        return false;
    }

    submission.AddEntry(L"D3D12Supported", true);
    Query_D3D12_FEATURE_DATA_D3D12_OPTIONS(device, submission);
    Query_D3D12_FEATURE_DATA_ARCHITECTURE(device, submission);
    Query_D3D12_FEATURE_DATA_FEATURE_LEVELS(device, submission);
    Query_D3D12_FEATURE_DATA_GPU_VIRTUAL_ADDRESS_SUPPORT(device, submission);
    Query_D3D12_FEATURE_DATA_SHADER_MODEL(device, submission);
    Query_D3D12_FEATURE_DATA_D3D12_OPTIONS1(device, submission);
    Query_D3D12_FEATURE_DATA_PROTECTED_RESOURCE_SESSION_SUPPORT(device, submission);
    Query_D3D12_FEATURE_DATA_ROOT_SIGNATURE(device, submission);
    Query_D3D12_FEATURE_DATA_ARCHITECTURE1(device, submission);
    Query_D3D12_FEATURE_DATA_D3D12_OPTIONS2(device, submission);
    Query_D3D12_FEATURE_DATA_SHADER_CACHE(device, submission);
    Query_D3D12_FEATURE_DATA_COMMAND_QUEUE_PRIORITY(device, submission);
    Query_D3D12_FEATURE_DATA_D3D12_OPTIONS3(device, submission);
    Query_D3D12_FEATURE_DATA_EXISTING_HEAPS(device, submission);
    Query_D3D12_FEATURE_DATA_D3D12_OPTIONS4(device, submission);
    Query_D3D12_FEATURE_DATA_SERIALIZATION(device, submission);
    Query_D3D12_FEATURE_DATA_CROSS_NODE(device, submission);
    Query_D3D12_FEATURE_DATA_D3D12_OPTIONS5(device, submission);
    Query_D3D12_FEATURE_DATA_DISPLAYABLE(device, submission);
    Query_D3D12_FEATURE_DATA_D3D12_OPTIONS6(device, submission);
    Query_D3D12_FEATURE_DATA_D3D12_OPTIONS7(device, submission);
    Query_D3D12_FEATURE_DATA_D3D12_OPTIONS8(device, submission);
    Query_D3D12_FEATURE_DATA_D3D12_OPTIONS9(device, submission);
    Query_D3D12_FEATURE_DATA_D3D12_OPTIONS10(device, submission);
    Query_D3D12_FEATURE_DATA_D3D12_OPTIONS11(device, submission);

    return true;
}
