#pragma once
#include <PipeReader.h>
#include <PipeWriter.h>
#include <Messages.h>

#include <map>
#include <memory>
#include <iostream>

struct UserConnection
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
  void handle(LoginRequest packet);
  void handle(Message packet);
  void handle(RoomInfoRequest packet);

private:
  IReader& mReader;

  std::map<std::string, UserConnection> mUsers;
};