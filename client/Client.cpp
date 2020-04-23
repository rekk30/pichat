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
    {
      std::cout << "Login msg serialization error" << std::endl;
      break;
    }

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
      mConnected = true;
    }
  } while(false);

  return result;
}

void Client::sendMessage(std::string_view msg)
{
  Message packet;
  packet.key = mCredentials.getPublicKey();
  packet.message = msg;
  std::string data;

  if(mSerializer.serialize(data, packet))
  {
    mWriter.write(data);
  }
}

void Client::handleMessage(std::string_view msg)
{
  switch(mSerializer.getType(msg))
  {
  case EMessage::LoginStatus:
  {
    std::cout << "Login status packet: " << std::endl << msg << std::endl;
    LoginStatus packet;
    if(!mSerializer.deserialize(msg, packet))
    {
      std::cout << "Wrong login status packet" << std::endl;
      break;
    }

    mLoginPromise.set_value(packet.status == ELoginStatus::Succeeded);
    break;
  }
  case EMessage::Message:
    std::cout << "New message received: " << std::endl
              << msg << std::endl;

    break;
  default:
    std::cout << "Wrong message: " << std::endl
              << msg << std::endl;
  }
}