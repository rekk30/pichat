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
  std::cout << "Client" << std::endl;

  std::string name;
  std::cout << "Enter your name: ";
  std::getline(std::cin, name);

  Credentials creds(name, "usr12345");
  Serializer ser;
  PipeReader read;
  PipeWriter write;

  std::string usrPath = "/tmp/" + name + "usr12345";
  if(mkfifo(usrPath.c_str(), 0666) == -1)
  {
    std::cout << "Pipe creation error" << std::endl;
    std::cout << errno << std::endl;
  }

  if(!write.open(PATH))
  {
    std::cout << "Pipe open error" << std::endl;
    return 1;
  }

  Client client(creds, ser, read, write);
  if(client.connect())
  {
    std::cout << "Client connected" << std::endl;

    std::string msg;
    while(true)
    {
      std::getline(std::cin, msg);
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