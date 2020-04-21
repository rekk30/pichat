#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <PipeReader.h>

const std::string PATH{"/tmp/pichat"};

void handler(std::string_view msg)
{
  std::cout << "New message received: " << std::endl
            << msg << std::endl;
}

int main(int argc, char* argv[])
{
  std::cout << "Server" << std::endl;

  mkfifo(PATH.c_str(), 0666);
  PipeReader reader;

  if(!reader.open(PATH))
  {
    std::cout << "File open error" << std::endl;
  }

  reader.setHandler(handler);

  reader.startLoop();

  unlink(PATH.c_str());

  return 0;
}