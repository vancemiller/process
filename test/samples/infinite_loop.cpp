#include <iostream>
int main(int argc, char** argv) {
  std::cout << "infinite loop" << std::endl;
  while (true)
    ; // infinite loop
  return 0;
}
