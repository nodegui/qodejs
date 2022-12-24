#pragma once

#include "node.h"

#ifdef _WIN32
# define QODE_EXPORT __declspec(dllexport)
#else
# define QODE_EXPORT __attribute__((visibility("default")))
#endif

namespace qode {
    typedef bool (*QodeInjectedInitFunc)(node::Environment* env, v8::Local<v8::Object> &proces_object);
    extern QodeInjectedInitFunc qode_init;
    void InjectQodeInit(QodeInjectedInitFunc init);

    typedef bool (*QodeMainRunLoopFunc)();
    extern QodeMainRunLoopFunc qode_run_gui_loop;
    void InjectQodeRunLoop(QodeMainRunLoopFunc runLoop);

    QODE_EXPORT typedef int (*QodeCustomRunLoopFunc)();
    extern QodeCustomRunLoopFunc custom_run_loop;
} // namespace qode

// qode api headers for integration
namespace qode {
    QODE_EXPORT extern int qode_argc;
    QODE_EXPORT extern char **qode_argv;
    QODE_EXPORT extern void InjectCustomRunLoop(QodeCustomRunLoopFunc customRunLoop);
}  // namespace qode 