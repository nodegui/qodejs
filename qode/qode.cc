// Copyright 2017 Atul R. All rights reserved.
#include <string.h>

#include "qode.h"
#include "qode/qode_shared.h"
#include "node_options.h"
#include "env-inl.h"
#include "qode/integration/node_integration.h"
#include "qode/helpers/qode_helper.h"
#include "qode_shared.h"
#include "node_version.h"

std::string qodeVersion = NODE_EXE_VERSION;

namespace qode {

// The global instance of NodeIntegration.
std::unique_ptr<NodeIntegration> qodeNodeIntegration;

std::string startFile = "";

// Untility function to create a V8 string.
inline v8::Local<v8::String> ToV8String(node::Environment* env, const char* str) {
  return v8::String::NewFromUtf8(
      env->isolate(), str, v8::NewStringType::kNormal).ToLocalChecked();
}

// Force running uv loop.
void ActivateUvLoop(const v8::FunctionCallbackInfo<v8::Value> &args) {
  qodeNodeIntegration->CallNextTick();
}


bool InitWrapper(node::Environment *env, v8::Local<v8::Object> &process_object) {
   // Set MicrotasksPolicy to Auto otherwise microtasks won't run.
  env->isolate()->SetMicrotasksPolicy(v8::MicrotasksPolicy::kAuto);
  v8::HandleScope handle_scope(env->isolate());

  v8::Local<v8::Value> versions = process_object->Get(
      env->context(), ToV8String(env, "versions")).ToLocalChecked();
  versions.As<v8::Object>()->Set(
      env->context(), ToV8String(env, "qode"), ToV8String(env, qodeVersion.c_str())).ToChecked();
  process_object->DefineOwnProperty(
      env->context(), ToV8String(env, "versions"), versions, v8::ReadOnly).Check();

  node::SetMethod(env->context(), process_object, "activateUvLoop", &ActivateUvLoop);

  std::shared_ptr<node::EnvironmentOptions> options = env->options();

  if(startFile.length()!=0 && qodeHelper::checkIfFileExists(startFile)){
      options->preload_cjs_modules.push_back(startFile);
  }
  return true;
}

bool QodeRunLoop(){
  qodeNodeIntegration->UvRunOnce();
  if(qode::custom_run_loop) {
    qode::custom_run_loop();
  }
  return true;
}

int Start(int argc, char *argv[]) {
  qode::qode_argc = argc;
  qode::qode_argv = argv;

  const char* run_as_node = getenv("QODE_RUN_AS_NODE");
  if (!run_as_node || strcmp(run_as_node, "1")) {
    // Prepare node integration.
    qodeNodeIntegration.reset(NodeIntegration::Create());
    qodeNodeIntegration->Init();

    JSON::json qodeConfig = qodeHelper::readConfig();

    std::string relativeDistPath = qodeConfig.value("distPath","");

    if(relativeDistPath.length() != 0){
      startFile = qodeHelper::mergePaths(qodeHelper::getExecutableDir(), relativeDistPath);
    }
    // Set run loop and init function on node.
    qode::InjectQodeInit(&InitWrapper);
    qode::InjectQodeRunLoop(&QodeRunLoop);
  }

  // Always enable GC this app is almost always running on desktop.
  std::string qodeFlags = "--expose_gc";
  v8::V8::SetFlagsFromString(qodeFlags.c_str(), qodeFlags.length());

  // Start node js
  int code = node::Start(qode::qode_argc, qode::qode_argv);

  // Clean up node integration and quit.
  qodeNodeIntegration.reset();
  return code;
}

}  // namespace qode
