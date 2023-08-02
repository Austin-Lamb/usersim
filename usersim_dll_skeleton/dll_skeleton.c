// Copyright (c) Microsoft Corporation
// SPDX-License-Identifier: MIT

#include <sal.h>
#include <winsock2.h>
#include <windows.h>
#include "usersim/rtl.h"
#include "usersim/wdf.h"

#define NT_SUCCESS(status) (((NTSTATUS)(status)) >= 0)

WDFDRIVER
WdfGetDriver();

DRIVER_INITIALIZE DriverEntry;

__declspec(dllimport)
PWDF_DRIVER_GLOBALS UsersimWdfDriverGlobals;

__declspec(dllimport) const WDFFUNC* UsersimWdfFunctions;

PWDF_DRIVER_GLOBALS WdfDriverGlobals = NULL;
const WDFFUNC* WdfFunctions_01015 = NULL;

    struct _driver_object
    {
        int dummy;
    };

    NTSTATUS
    UsersimStartDriver()
    {
        WdfDriverGlobals = UsersimWdfDriverGlobals;
        WdfFunctions_01015 = UsersimWdfFunctions;

        DRIVER_OBJECT driver_object = {0};
        UNICODE_STRING registry_path = {0};
        return DriverEntry(&driver_object, &registry_path);
    }

    void
    UsersimStopDriver()
    {
        WDFDRIVER driver = WdfDriverGlobals->Driver;
        if (driver.config.EvtDriverUnload != NULL) {
            driver.config.EvtDriverUnload(driver);
        }
    }

    BOOL APIENTRY
    DllMain(HMODULE hModule, unsigned long ul_reason_for_call, void* lpReserved)
    {
        UNREFERENCED_PARAMETER(hModule);
        UNREFERENCED_PARAMETER(lpReserved);
        switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            return NT_SUCCESS(UsersimStartDriver());
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            UsersimStopDriver();
            break;
        }
        return TRUE;
    }