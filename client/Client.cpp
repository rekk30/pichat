#include "Client.h"

Client::Client(ICredentials& creds,
               IReader& reader,
               IWriter& writer)
  : mCredentials{creds}
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

  mReader.setHandler(std::bind(&Client::handleMessage, this, _1));
  mReaderThread = std::thread{[&]{mReader.startLoop();}};
  std::future loginFuture = mLoginPromise.get_future();

  Login loginMsg;
  loginMsg.name = mCredentials.getName();
  loginMsg.key = mCredentials.getPublicKey();
  loginMsg.path = readerPath;

  bool result{false};
  ByteArray data;

  do
  {
    if(!loginMsg.serialize(data))
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
  packet.name = mCredentials.getName();
  packet.key = mCredentials.getPublicKey();
  packet.message = msg;
  ByteArray data;

  if(packet.serialize(data))
  {
    mWriter.write(data);
  }
}

void Client::handleMessage(ByteArray msg)
{
  uint8_t type{static_cast<uint8_t>(EMessage::Error)};
  msg >> type;

  switch(static_cast<EMessage>(type))
  {
  case EMessage::LoginStatus:
  {
    LoginStatus packet;
    if(!packet.deserialize(msg))
    {
      std::cout << "Wrong login status packet" << std::endl;
      break;
    }

    mLoginPromise.set_value(packet.status == ELoginStatus::Succeeded);
    break;
  }
  case EMessage::Message:
  {
    Message packet;
    if(!packet.deserialize(msg))
    {
      std::cout << "Wrong message packet" << std::endl;
      break;
    }

    std::cout << COLOR_BLUE   << packet.name << ": "
              << COLOR_NORMAL << packet.message << std::endl;

    break;
  }
  default:
    std::cout << "Wrong message: " << std::endl;
    std::copy(msg.begin(), msg.end(),
              std::ostream_iterator<int>(std::cout, " "));
  }
}