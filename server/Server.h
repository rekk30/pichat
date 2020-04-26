#pragma once
#include <PipeReader.h>
#include <PipeWriter.h>
#include <Credentials.h>
#include <Messages.h>

#include <map>
#include <memory>
#include <iostream>

struct User
{
  std::string name;
  std::string path;
  std::unique_ptr<IWriter> writer;
};

class Server
{
public:
  Server(IReader&);

  void start();
  void handleMessage(ByteArray msg);
  void handle(Login packet);
  void handle(Message packet);

private:
  IReader& mReader;

  std::map<std::string, User> mUsers;
};