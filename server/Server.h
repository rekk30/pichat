#pragma once
#include <PipeReader.h>
#include <PipeWriter.h>
#include <Credentials.h>
#include <Serializer.h>
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
  Server(ISerializer&, IReader&);

  void start();
  void handleMessage(std::string_view msg);

private:
  ISerializer& mSerializer;
  IReader& mReader;

  std::map<std::string, User> mUsers;
};