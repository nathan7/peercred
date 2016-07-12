#include "credentials.hh"
#include <nan.h>

NAN_METHOD(FromFd) {
  auto fd = Nan::To<int>(info[0]).FromJust();
  auto ret = Nan::New<v8::Object>();
  info.GetReturnValue().Set(ret);
#define SET_RETVAL(key, value) Nan::Set(ret, Nan::New(key).ToLocalChecked(), Nan::New(value))

  Credentials creds;
  if (!creds.Init(fd)) {
    SET_RETVAL("errno", errno);
    return;
  }

  SET_RETVAL("pid", creds.GetPid());
  SET_RETVAL("uid", creds.GetUid());
  SET_RETVAL("gid", creds.GetGid());

#ifdef CREDENTIALS_HAS_PPID
  SET_RETVAL("ppid", creds.GetPpid());
#endif
#ifdef CREDENTIALS_HAS_PGID
  SET_RETVAL("pgid", creds.GetPgid());
#endif
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, Nan::New("fromFd").ToLocalChecked(),
    Nan::GetFunction(Nan::New<v8::FunctionTemplate>(FromFd)).ToLocalChecked());
}

NODE_MODULE(NativeExtension, Init)
