#include <CppUnitTest.h>

#include <crtdbg.h>
#include <cstdlib>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-field-initializers" // the macro omits MethodMetadata::sourceFile
#endif
// A failing assert() or debug CRT/STL check in a Debug run would otherwise open a modal Abort/Retry/Ignore box that
// blocks unattended test runs. Print it to stderr and terminate instead.
TEST_MODULE_INITIALIZE(reportAssertsToStderr)
{
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
