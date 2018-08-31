#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <list>
#include <memory>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>

namespace wrapper {
class Process {
  protected:
    const std::list<std::string> args;
    mutable pid_t pid;
  private:
    Process(const std::list<std::string>& args, pid_t pid);
  public:
    Process(const std::list<std::string>& args);
    Process(void) = delete;
    Process(const Process&) = delete;
    Process(Process&) = delete;
    Process(Process&& o);
    ~Process(void);
  public:
    pid_t getpid(void) const;
    void kill(int sig=SIGKILL);
    const std::string to_string(void) const;
    int wait(int options=0) const;
};
} // namespace wrapper
#endif
