#include <PipeWriter.h>
#include "Client.h"

#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

const std::string PATH{"/tmp/pichat"};

int main(int argc, char* argv[])
{
  std::cout << "Client" << std::endl;

  PipeWriter writer;

  if(!writer.open(PATH))
  {
    std::cout << "Pipe open error" << std::endl;
  }

  // Client client;
  // std::string msg;
  // std::getline(std::cin, msg);

  // writer.write(msg);

  return 0;
}