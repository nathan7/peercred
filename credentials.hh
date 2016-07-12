#include <sys/types.h>
#include <sys/socket.h>
#if __APPLE__
#include <sys/ucred.h>
#include <sys/un.h>
#include <sys/proc_info.h>
#include <libproc.h>
#define CREDENTIALS_HAS_PPID
#define CREDENTIALS_HAS_PGID
#elif __linux__
// no extra headers
#else
#error "Unsupported platform"
#endif

class Credentials {
#ifdef __APPLE__
  struct proc_bsdshortinfo proc;
#elif __linux__
  struct ucred ucred;
#endif

public:
  Credentials() {}
  auto Init(int fd) -> bool;

  auto GetPid() -> pid_t;
#ifdef CREDENTIALS_HAS_PPID
  auto GetPpid() -> pid_t;
#endif
#ifdef CREDENTIALS_HAS_PGID
  auto GetPgid() -> pid_t;
#endif

  auto GetUid() -> uid_t;
  auto GetGid() -> gid_t;
};
