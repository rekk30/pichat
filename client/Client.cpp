#include "Client.h"

Client::Client(ICredentials& creds, ISerializer& serializer, IReader& reader, IWriter& writer)
  : mCredentials{creds}
  , mSerializer{serializer}
  , mReader{reader}
  , mWriter{writer}
{
  if(mReaderThread.joinable())
  {
    mReaderThread.join();
  }
}

bool Client::connect()
{
  using namespace std::placeholders;
  mReader.setHandler(std::bind(&Client::handleMessage, this, _1));
  mReaderThread(&IReader.startLoop, mReader);

  Login loginMsg;
  loginMsg.name = mCredentials.getName();
  loginMsg.key = mCredentials.getPublicKey();
  mWriter.write()
}

void Client::sendMessage(std::string_view msg)
{

}

void Client::handleMessage(std::string_view msg)
{

}