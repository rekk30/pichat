#include "Client.h"

Client::Client(ICredentials& creds,
               ISerializer& serializer,
               IReader& reader,
               IWriter& writer)
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
  std::string readerPath
    = "/tmp/" + mCredentials.getName() + mCredentials.getPublicKey();

  mReader.open(readerPath);
  mReader.setHandler(std::bind(&Client::handleMessage, this, _1));
  mReaderThread = std::thread{[&]{mReader.startLoop();}};
  std::future loginFuture = mLoginPromise.get_future();

  Login loginMsg;
  loginMsg.name = mCredentials.getName();
  loginMsg.key = mCredentials.getPublicKey();
  loginMsg.path = readerPath;

  bool result{false};
  std::string data;

  do
  {
    result = mSerializer.serialize(data, loginMsg);
    if(!result)
      break;

    mWriter.write(data);

    auto status = loginFuture.wait_for(std::chrono::seconds(3));
    if(status == std::future_status::timeout)
    {
      std::cout << "Login timeout" << std::endl;
      result = false;
    }
    else
    {
      result = loginFuture.get();
    }
  } while(false);

  return result;
}

void Client::sendMessage(std::string_view msg)
{
  mWriter.write(msg);
}

void Client::handleMessage(std::string_view msg)
{
  std::cout << "Received msg: " << std::endl
            << msg              << std::endl;
}