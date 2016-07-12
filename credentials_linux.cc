#include "credentials.hh"
#include <cassert>

auto Credentials::Init(int fd) -> bool {
  socklen_t ucred_len = sizeof ucred;
  if (getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &ucred, &ucred_len)) {
    return false;
  }
  assert(ucred_len == sizeof ucred);
  return true;
}

auto Credentials::GetPid() -> pid_t {
  return ucred.pid;
}

auto Credentials::GetUid() -> uid_t {
  return ucred.uid;
}

auto Credentials::GetGid() -> gid_t {
  return ucred.uid;
}
