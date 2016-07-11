#include <sys/types.h>
#include <sys/socket.h>
#if __APPLE__ || __FreeBSD__
#include <sys/ucred.h>
#endif
#include <nan.h>

using v8::FunctionTemplate;

NAN_METHOD(FromFd) {
  auto fd = Nan::To<int>(info[0]).FromJust();


  auto ret = Nan::New<v8::Object>();
  info.GetReturnValue().Set(ret);
#define SET_RETVAL(key, value) Nan::Set(ret, Nan::New(#key).ToLocalChecked(), Nan::New(value))

#if defined(SO_PEERCRED)
  struct ucred creds; 
  memset(&creds, 0, sizeof creds);
  socklen_t creds_len = sizeof creds;
  int fail = getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &creds, &creds_len);
#elif defined(LOCAL_PEERCRED)
  struct xucred creds;
  memset(&creds, 0, sizeof creds);
  socklen_t creds_len = sizeof creds;
  int fail = getsockopt(fd, LOCAL_PEERCRED, 1, &creds, &creds_len);
  assert(fail != 0 || creds.cr_version == XUCRED_VERSION);
#else
#error "unsupported platform"
#endif

  if (fail) {
    SET_RETVAL(errno, errno);
  } else {
    SET_RETVAL(pid, creds.pid);
    SET_RETVAL(uid, creds.uid);
    SET_RETVAL(gid, creds.gid);
  }
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, Nan::New("fromFd").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(FromFd)).ToLocalChecked());
}

NODE_MODULE(NativeExtension, Init)
