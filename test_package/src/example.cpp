#include <string>
#include <vector>
#include <iostream>

#include <testcontainers/testcontainers.hpp>

using namespace testcontainers;

int main() {
  {
    auto image = GenericImage("redis", "7.2.4")
                     .with_exposed_port(ContainerPort::Tcp(6379))
                     .with_wait_for(WaitFor::message_on_stdout("Ready to accept connections"));
  }

  std::cout << "Redis container created" << std::endl;
}
