#!/usr/bin/env node

const path = require('path');
const os = require('os');
const fs = require('fs');
const cp = require('child_process');

const HOST_ARCH = os.arch();
const outDir = path.resolve(__dirname, '..', 'out', 'Release');
//==================================
//    HELPER UTILITIES
//==================================
// Wrapper of execSync that prints output.
const execSync = (command, options = {}) => {
  if (options.stdio === undefined) options.stdio = 'inherit';
  if (options.env) options.env = Object.assign(options.env, options.env);
  else options.env = Object.assign({}, process.env);
  return cp.execSync(command, options);
};

const checkEnvExists = (envVarName, defaultValue) => {
  const value = process.env[envVarName];
  if (!value) {
    console.warn(
      `Env variable: ${envVarName} not specified, using default: ${defaultValue}`
    );
    process.env[envVarName] = defaultValue;
    return defaultValue;
  }
  console.log(`Env variable: ${envVarName}, value: ${value}`);
  return value;
};

//==================================
//    BUILD PROCESS
//==================================
// Specify target_arch.
const target_arch = checkEnvExists('TARGET_ARCH', HOST_ARCH);
const host_arch = checkEnvExists('HOST_ARCH', HOST_ARCH);

const isCrossCompiling = target_arch !== host_arch;

if (isCrossCompiling) {
  process.env.GYP_CROSSCOMPILE = '1';
}

function compileLinux() {
  if (target_arch !== host_arch) {
    execSync(
      `python3 configure --with-intl=small-icu --cross-compiling --dest-cpu=${target_arch}`,
      { cwd: path.resolve(__dirname, '..') }
    );
  } else {
    execSync(
      `python3 configure --with-intl=small-icu  --dest-cpu=${target_arch}`,
      { cwd: path.resolve(__dirname, '..') }
    );
  }

  execSync(`make -j${os.cpus().length}`, {
    cwd: path.resolve(__dirname, '..'),
  });

  fs.copyFileSync(path.resolve(outDir, 'node'), path.resolve(outDir, 'qode'));

  execSync(`cp node qode`, {
    cwd: path.resolve(__dirname, '..', 'out', 'Release'),
  });
}

function compileMac() {
  if (isCrossCompiling) {
    if (target_arch === 'arm64') {
      Object.assign(process.env, {
        CC: `cc -arch ${target_arch}`,
        CXX: `c++ -arch ${target_arch}`,
        CC_target: `cc -arch ${target_arch}`,
        CXX_target: `c++ -arch ${target_arch}`,
        CC_host: 'cc -arch x86_64',
        CXX_host: 'c++ -arch x86_64',
      });
    }
    execSync(
      `python3 configure --with-intl=small-icu --cross-compiling --dest-cpu=${target_arch}`,
      { cwd: path.resolve(__dirname, '..') }
    );
  } else {
    execSync(
      `python3 configure --with-intl=small-icu  --dest-cpu=${target_arch}`,
      { cwd: path.resolve(__dirname, '..') }
    );
  }

  execSync(`make -j${os.cpus().length}`, {
    cwd: path.resolve(__dirname, '..'),
  });

  fs.copyFileSync(path.resolve(outDir, 'node'), path.resolve(outDir, 'qode'));
}

function compileWin() {
  execSync(`.\\vcbuild release small-icu ${target_arch}`, {
    cwd: path.resolve(__dirname, '..'),
  });

  fs.copyFileSync(
    path.resolve(outDir, 'node.exe'),
    path.resolve(outDir, 'qode.exe')
  );
}

switch (process.platform) {
  case 'linux': {
    compileLinux();
    break;
  }
  case 'win32': {
    compileWin();
    break;
  }
  case 'darwin': {
    compileMac();
    break;
  }
  default: {
    throw new Error(`Unsupported platform ${process.platform}`);
  }
}
