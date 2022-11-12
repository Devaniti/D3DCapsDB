#pragma once
#include "JSONStorage.h"

bool QueryDXGIInfo(const ComPtr<IDXGIAdapter>& adapter, JSONStorage& submission);
