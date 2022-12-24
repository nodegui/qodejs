#include "qode_shared.h"

int qode::qode_argc = 0;
char** qode::qode_argv = 0;
qode::QodeInjectedInitFunc qode::qode_init = nullptr;
qode::QodeMainRunLoopFunc qode::qode_run_gui_loop = nullptr;
qode::QodeCustomRunLoopFunc qode::custom_run_loop = nullptr;

void qode::InjectQodeInit(qode::QodeInjectedInitFunc init) {
    qode::qode_init = init;
}
void qode::InjectQodeRunLoop(qode::QodeMainRunLoopFunc runLoop) {
    qode::qode_run_gui_loop = runLoop;
} 
void qode::InjectCustomRunLoop(qode::QodeCustomRunLoopFunc customRunLoop) {
    qode::custom_run_loop = customRunLoop;
} 