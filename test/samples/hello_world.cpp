#include <unistd.h>
#include <iostream>

int main(int argc, char** argv) {
  sleep(1);
  std::cout << "hello, world!" << std::endl;
  return 123;
}
