'use strict'
var fs = require('fs')
var net = require('net')
var peercred = require('./')

try {
  fs.unlinkSync('test.sock')
} catch (e) {
  if (e.code !== 'ENOENT') throw e
}

net.createServer(function (sock) {
  var cred = peercred.fromSock(sock)
  sock.end(JSON.stringify(cred, null, 2))
})
