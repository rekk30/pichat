#include <PipeWriter.h>
#include "Client.h"

#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <csignal>
#include <unistd.h>
#include <cerrno>

const std::string PATH{"/tmp/pichat"};
std::string USER_PATH;

void exitHandler(int s)
{
  unlink(USER_PATH.c_str());
  exit(0);
}

int main(int argc, char* argv[])
{
  std::signal(SIGINT, exitHandler);

  std::string name;
  std::cout << CLEAR_SCREEN;
  std::cout << "Enter your name: ";
  std::getline(std::cin, name);
  std::cout << CLEAR_LINE;

  PipeReader read;
  PipeWriter write;

  std::string id{name};
  USER_PATH = "/tmp/" + name;
  mkfifo(USER_PATH.c_str(), 0666);

  if(!read.open(USER_PATH))
  {
    std::cout << "Pipe open reader error" << std::endl;
    return 1;
  }

  if(!write.open(PATH))
  {
    std::cout << "Pipe open writer error" << std::endl;
    return 1;
  }

  Client client(read, write, std::move(name), std::move(id));
  if(client.connect())
  {
    std::string msg;
    while(true)
    {
      std::getline(std::cin, msg, '\n');
      std::cout << CLEAR_LINE;
      client.sendMessage(msg);
    }
  }

  unlink(USER_PATH.c_str());
  return 0;
}