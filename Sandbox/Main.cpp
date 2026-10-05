#include "pch.h"

#include "SandboxApp.h"

#if defined(JUG_COMPILER_MSVC) && defined(JUG_DEBUG)
#    define JUG_MSVC_DEBUG
#endif

int main(
    const int _argc,
    char**    _argv)
{
#ifdef JUG_MSVC_DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    jug::AppConfig config = {};
    config.appName        = "SandboxApp";
    config.appVersion     = "1.0.0";
    config.appIdentifier  = "juggernaut.sandbox";
    config.bInitGamepad   = false;

    SandboxApp* pSandbox = new SandboxApp { config };
    const int   code     = pSandbox->Run(_argc, _argv);
    delete pSandbox;
    return code;
}
