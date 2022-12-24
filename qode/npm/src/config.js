const os = require('os');
const path = require('path');
const envPaths = require('env-paths');
const package = require('../package');

const platform = os.platform();
const libVersion = package.version;
const arch = os.arch();
const extractDir = path.resolve(__dirname, '..', 'binaries');

const appPaths = envPaths('qode');

const executableNames = {
  darwin: 'qode',
  linux: 'qode',
  win32: 'qode.exe',
};

const gitTagVersion = `v${libVersion}-qode`;

const qodePath = path.resolve(extractDir, executableNames[platform]);
const downloadArchiveName = `${gitTagVersion}-${platform}-${arch}.tar.gz`;
const downloadLink =
  process.env.QODE_MIRROR ||
  `https://github.com/nodegui/qodejs/releases/download/${gitTagVersion}/${downloadArchiveName}`;

module.exports = {
  downloadLink,
  libVersion,
  extractDir,
  cacheDir: appPaths.cache,
  qodePath,
};
