#include <PipeWriter.h>
#include <Serializer.h>
#include <Credentials.h>
#include "Client.h"

#include <iostream>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>

const std::string PATH{"/tmp/pichat"};

int main(int argc, char* argv[])
{
  std::string name;
  std::cout << CLEAR_SCREEN;
  std::cout << "Enter your name: ";
  std::getline(std::cin, name);
  std::cout << CLEAR_LINE;

  Credentials creds(name, name);
  Serializer ser;
  PipeReader read;
  PipeWriter write;

  std::string usrPath = "/tmp/" + creds.getName() + creds.getPublicKey();
  mkfifo(usrPath.c_str(), 0666);

  if(!read.open(usrPath))
  {
    std::cout << "Pipe open reader error" << std::endl;
    return 1;
  }

  if(!write.open(PATH))
  {
    std::cout << "Pipe open writer error" << std::endl;
    return 1;
  }

  Client client(creds, ser, read, write);
  if(client.connect())
  {
    std::cout << COLOR_GREEN << "Client connected"
              << COLOR_NORMAL << std::endl;

    std::string msg;
    while(true)
    {
      std::getline(std::cin, msg, '\n');
      std::cout << CLEAR_LINE;
      client.sendMessage(msg);
    }
  }
  else
  {
    std::cout << "Connection failed" << std::endl;
  }

  unlink(usrPath.c_str());
  return 0;
}