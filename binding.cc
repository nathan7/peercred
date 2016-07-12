#include <sys/types.h>
#include <sys/socket.h>
#if __APPLE__ || __FreeBSD__
#include <sys/ucred.h>
#include <sys/un.h>
#include <sys/proc_info.h>  // for struct proc_bsdshortinfo
#include <libproc.h>        // for proc_pidinfo()
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
  socklen_t creds_len = sizeof creds;
  int fail = getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &creds, &creds_len);
#elif defined(LOCAL_PEERCRED) && defined(LOCAL_PEERPID)
#define USE_PROC
  struct proc_bsdshortinfo proc;

  struct {
    pid_t pid;
    uid_t uid;
  } creds;

  struct xucred xcreds;
  socklen_t xcreds_len = sizeof xcreds;
  int fail = getsockopt(fd, SOL_LOCAL, LOCAL_PEERCRED, &xcreds, &xcreds_len);
  if (!fail) {
    assert(xcreds.cr_version == XUCRED_VERSION);
    creds.uid = xcreds.cr_uid;

    socklen_t pid_len = sizeof creds.pid;
    fail = getsockopt(fd, SOL_LOCAL, LOCAL_PEERPID, &creds.pid, &pid_len);
  }
  if (!fail) {
    fail = proc_pidinfo(creds.pid, PROC_PIDT_SHORTBSDINFO, 1, &proc, PROC_PIDT_SHORTBSDINFO_SIZE) == 0;
  }
#else
#error "unsupported platform"
#endif

  if (fail) {
    SET_RETVAL(errno, errno);
  } else {
    SET_RETVAL(pid, creds.pid);
    SET_RETVAL(uid, creds.uid);
#ifdef USE_PROC
    SET_RETVAL(ppid, proc.pbsi_ppid);
    SET_RETVAL(pgid, proc.pbsi_pgid);
    SET_RETVAL(gid, proc.pbsi_gid);
#else
    SET_RETVAL(gid, creds.gid);
#endif
  }
}

NAN_MODULE_INIT(Init) {
  Nan::Set(target, Nan::New("fromFd").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(FromFd)).ToLocalChecked());
}

NODE_MODULE(NativeExtension, Init)
