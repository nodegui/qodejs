# Qode

<img alt="logo" src="https://github.com/nodegui/nodegui/raw/master/extras/logo/nodegui.png" height="200" />

Qode is a lightly modified fork of Node.js that allows Node's event loop to be merged with Qt's or any other Gui event loop.
It is designed to be used together with `@nodegui/nodegui`. Qode achieves this by allowing message loop injection via a NodeJS addon.

Qode allows you to run both Qt and Node's event loops in the same thread. This is crucial to achieving low CPU footprint.
Qode can be used as a _drop in_ replacement of NodeJS and hence you could consider it as NodeJS with Qt powers 💪.

This npm module downloads the prebuilt qode for the current Operating System.

_Build Instructions for qode can be found here:_ https://github.com/nodegui/qode

# Installation

```
yarn add @nodegui/qode
```

or

```
npm install @nodegui/qode
```

### Dowload mirrors

By default qode is downloaded using the github releases at

```js
`https://github.com/nodegui/qodejs/releases/download/v${libVersion}-qode/${gitTagVersion}-${platform}-${arch}.tar.gz`
```

if you need to specify an alternative download path you can set the environment variable `QODE_MIRROR` like so and run npm install

```
QODE_MIRROR=https://github.com/nodegui/qodejs/releases/download/v1.2-qode/1.2-linux-x64.tar.gz npm install @nodegui/qodejs
```

If it doesnt download make sure to clear the cached qode at https://github.com/nodegui/qodejs/blob/89ccb29fe3c9d66426311a95369c47820bca4de5/qode/npm/src/config.js#L29

# Version Table:

| Qode        | Node    | Qt                 |
| ----------- | ------- | ------------------ |
| 2.0.0       | 12.11.0 | depends on Nodegui |
| ----------- | ------- | ------------------ |
| 3.0.0       | 16.4.0  | depends on Nodegui |
