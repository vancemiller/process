#include "process.hpp"

#include <cstring>
#include <fstream>
#include <system_error>
#include <spawn.h>
#include <unistd.h>

namespace wrapper {

extern "C" char** environ;

static std::unique_ptr<char*[]> build_args(const std::list<std::string>& args) {
  std::unique_ptr<char*[]> argv = std::make_unique<char*[]>(args.size() + 1);
  size_t i = 0;
  for (auto it : args) {
    argv[i] = new char[it.length() + 1];
    std::strncpy(argv[i], it.c_str(), it.length() + 1);
    i++;
  }
  argv[args.size()] = NULL;
  return argv;
}

static pid_t spawn_process(std::unique_ptr<char*[]>&& argv) {
  {
    std::ifstream infile(argv[0]);
    if (!infile.good()) throw std::runtime_error("file does not exist");
  }
  posix_spawn_file_actions_t actions;
  int err;
  pid_t pid;
  if ((err = posix_spawn_file_actions_init(&actions)))
    throw std::system_error(err, std::generic_category(), "posix_spawn_file_actions_init failed");
  posix_spawnattr_t attributes;
  if ((err = posix_spawnattr_init(&attributes)))
    throw std::system_error(err, std::generic_category(), "posix_spawnattr_init failed");
  if ((err = posix_spawn(&pid, argv[0], &actions, &attributes, argv.get(), environ)))
    throw std::system_error(err, std::generic_category(), "posix_spawn failed");
  if ((err = posix_spawnattr_destroy(&attributes)))
    throw std::system_error(err, std::generic_category(), "posix_spawnattr_destroy failed");
  if ((err = posix_spawn_file_actions_destroy(&actions)))
    throw std::system_error(err, std::generic_category(),
        "posix_spawn_file_actions_destroy failed");
  return pid;
}

Process::Process(const std::list<std::string>& args, pid_t pid) : args(args), pid(pid) {}

Process::Process(const std::list<std::string>& args) : Process(args,
    spawn_process(build_args(args))) {}

Process::Process(Process&& o) : Process(o.args, o.pid) { o.pid = 0; }

Process::~Process(void) { if (pid) wait(); }


pid_t Process::getpid(void) const {
  if (pid) return pid;
  throw std::runtime_error("Process has been moved");
}

/**
 * Sends sig to the process
 */
void Process::kill(int sig) {
  ::kill(getpid(), sig);
}

/**
 * Waits for a process to terminate.
 * returns 127 if an error occurred in process initialization
 */
int Process::wait(int options) const {
  int status;
  pid_t ret = waitpid(getpid(), &status, options);
  pid = 0;
  if (ret == -1) throw std::system_error(errno, std::generic_category(), "waitpid failed");
  if (WIFEXITED(status)) return WEXITSTATUS(status);
  return status;
}

const std::string Process::to_string(void) const {
  return std::to_string(getpid());
}
} // namespace wrapper
