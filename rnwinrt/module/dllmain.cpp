#include "pch.h"

#include <jsi\jsi.cpp>

#pragma warning (push)
#pragma warning (disable: 4267) // 'argument': conversion from 'size_t' to 'unsigned int', possible loss of data
#include <TurboModule\Core\TurboModule.cpp>
#pragma warning (pop)

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