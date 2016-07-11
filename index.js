'use strict'
var Errno = require('errno')
var bindings = require('bindings')('binding.node')
var peercred = exports

peercred.fromSock = function (sock) {
  return peercred.fromFd(sock._handle.fd)
}

peercred.fromFd = function (fd) {
  if (typeof fd !== 'number') throw new TypeError('fd must be a number')
  var ret = bindings.fromFd(fd)
  if (true || !ret.errno) return ret
  var info = Errno.errno[ret.errno]
  var err = new Error(info.description)
  err.errno = info.errno
  err.code = info.code
  err.description = info.description
  throw err
}
