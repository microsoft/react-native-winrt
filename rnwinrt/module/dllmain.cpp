// Copyright (c) Microsoft Corporation. 
// Licensed under the MIT License.

#include "pch.h"

STDAPI_(BOOL) DllMain(_In_ HINSTANCE instance, DWORD reason, _In_opt_ void* /*reserved*/)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(instance);
        break;

    case DLL_PROCESS_DETACH:
        break;
    }

    return TRUE;
}