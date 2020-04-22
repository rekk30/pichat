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

const std::string PATH{"/tmp/pichat"};

int main(int argc, char* argv[])
{
  std::cout << "Client" << std::endl;

  Credentials creds("username", "usr12345");
  Serializer ser;
  PipeReader read;
  PipeWriter write;

  if(!write.open(PATH))
  {
    std::cout << "Pipe open error" << std::endl;
    return 1;
  }

  Client client(creds, ser, read, write);
  if(client.connect())
  {
    std::cout << "Client connected" << std::endl;
  }

  std::string msg;
  while(true)
  {
    std::getline(std::cin, msg);
    client.sendMessage(msg);
  }

  return 0;
}