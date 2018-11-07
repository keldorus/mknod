var bindings = require('node-gyp-build')(__dirname)
var noop = function () {}

module.exports = function (path, mode, dev, cb) {
  console.log('here');
  bindings.mknod(path, mode, dev, cb || noop)
}
