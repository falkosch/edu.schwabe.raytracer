#include <CppUnitTest.h>

#include <omp.h>

#include <crtdbg.h>
#include <cstdlib>

#include <windows.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-field-initializers" // the macro omits MethodMetadata::sourceFile
#endif
TEST_MODULE_INITIALIZE(initializeTestModule)
{
    // The CppUnitTest executor FreeLibrary()s the test DLL after the run, which also unloads the OpenMP runtime
    // (vcomp140.dll) while its idle worker threads are still parked inside it. They then fault executing unmapped code
    // (0xC0000005) and take the test host down. Pin the module that implements omp_get_max_threads for the lifetime of
    // the process: with MSVC's dllimport declaration that is the OpenMP runtime itself; with clang-cl's omp.h (no
    // dllimport) it is the import thunk in this DLL, which keeps this DLL and thereby its OpenMP import loaded.
    HMODULE runtime{};
    GetModuleHandleExW(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_PIN,
        reinterpret_cast<LPCWSTR>(&omp_get_max_threads), &runtime
    );

    // A failing assert() or debug CRT/STL check in a Debug run would otherwise open a modal Abort/Retry/Ignore box that
    // blocks unattended test runs. Print it to stderr and terminate instead.
    _set_error_mode(_OUT_TO_STDERR);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif
