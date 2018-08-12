#include "process.hpp"
#include "gtest/gtest.h"

namespace wrapper {

TEST(Process, ConstructDestruct) {
  Process({"test/hello_world"});
}

TEST(Process, SpawnInvalid) {
  Process p({"asdfghjwe___"});
  EXPECT_EQ(127, p.wait());
}

TEST(Process, ReturnValue) {
  Process hello({"hello_world"});
  EXPECT_EQ(123, hello.wait());
}

TEST(Process, WaitTwice) {
  Process hello({"hello_world"});
  EXPECT_EQ(123, hello.wait());
  EXPECT_THROW(hello.wait(), std::runtime_error);
}

TEST(Process, Kill) {
  Process hello({"hello_world"});
  hello.kill();
  EXPECT_EQ(SIGKILL, hello.wait());
}

TEST(Process, KillAfterWait) {
  Process hello({"hello_world"});
  hello.wait();
  EXPECT_THROW(hello.kill(), std::runtime_error);
}

TEST(Process, InfiniteLoopKill) {
  Process inf({"infinite_loop"});
  sleep(1);
  inf.kill(SIGKILL);
  EXPECT_EQ(SIGKILL, inf.wait());
}

TEST(Process, KillTwice) {
  Process inf({"infinite_loop"});
  sleep(1);
  inf.kill(SIGKILL);
  EXPECT_NO_THROW(inf.kill()); // can call kill as much as we want
  EXPECT_EQ(SIGKILL, inf.wait());
}

TEST(Process, Move) {
  Process inf({"infinite_loop"});
  sleep(1);
  Process moved(std::move(inf));
  EXPECT_THROW(inf.kill(), std::runtime_error);
  EXPECT_THROW(inf.wait(), std::runtime_error);
  moved.kill();
  EXPECT_EQ(SIGKILL, moved.wait());
}
} // namespace wrapper
