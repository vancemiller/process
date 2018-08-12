#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <list>
#include <memory>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>

class Process {
  private:
    const int argc;
    std::unique_ptr<char*[]> argv;
    mutable pid_t pid;
  public:
    Process(const std::list<std::string>& args);
    Process(int argc, std::unique_ptr<char*[]> argv);
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

#endif
