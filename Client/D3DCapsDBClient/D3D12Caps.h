#pragma once
#include "JSONStorage.h"

bool QueryD3D12Caps(const ComPtr<IDXGIAdapter>& adapter, JSONStorage& submission);
