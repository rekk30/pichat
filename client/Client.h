#pragma once
#include <PipeReader.h>
#include <PipeWriter.h>
#include <Credentials.h>
#include <Serializer.h>
#include <Messages.h>

#include <thread>
#include <chrono>
#include <future>
#include <iostream>

class Client
{
public:
  Client(ICredentials&, ISerializer&, IReader&, IWriter&);

  bool connect();
  void sendMessage(std::string_view msg);
  void handleMessage(std::string_view msg);

private:
  bool mConnected{false};

  ICredentials& mCredentials;
  ISerializer& mSerializer;

  IReader& mReader;
  IWriter& mWriter;

  std::thread mReaderThread;
  std::promise<bool> mLoginPromise;
};