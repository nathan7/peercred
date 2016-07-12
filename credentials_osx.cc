#include "credentials.hh"
#include <cassert>

auto Credentials::Init(int fd) -> bool {
  auto pid = (pid_t) -1;
  socklen_t pid_len = sizeof pid;
  if (getsockopt(fd, SOL_LOCAL, LOCAL_PEERPID, &pid, &pid_len)) {
    return false;
  }
  assert(pid_len == sizeof pid);
  if (proc_pidinfo(pid, PROC_PIDT_SHORTBSDINFO, 1, &proc, PROC_PIDT_SHORTBSDINFO_SIZE) == 0) {
    return false;
  }
  return true;
}

auto Credentials::GetPid() -> pid_t {
  return (pid_t) proc.pbsi_pid;
}

auto Credentials::GetPpid() -> pid_t {
  return (pid_t) proc.pbsi_ppid;
}

auto Credentials::GetPgid() -> pid_t {
  return (pid_t) proc.pbsi_pgid;
}

auto Credentials::GetUid() -> uid_t {
  return proc.pbsi_uid;
}

auto Credentials::GetGid() -> gid_t {
  return proc.pbsi_gid;
}
