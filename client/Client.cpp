#include "Client.h"

Client::Client(IReader& reader,
               IWriter& writer,
               std::string name,
               std::string id)
  : mReader{reader}
  , mWriter{writer}
  , mName{name}
  , mId{id}
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
    = "/tmp/" + mId;

  mReader.setHandler(std::bind(&Client::handleMessage, this, _1));
  mReaderThread = std::thread{[&]{mReader.startLoop();}};
  std::future loginFuture = mLoginPromise.get_future();

  LoginRequest loginMsg;
  loginMsg.name = mName;
  loginMsg.id = mId;
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

      if(result)
      {
        std::cout << COLOR_GREEN
                  << "Client connected"
                  << COLOR_NORMAL
                  << std::endl;

        RoomInfoRequest req;
        req.id = mId;

        ByteArray _data;
        result = req.serialize(_data);
        if(result)
        {
          mWriter.write(_data);
        }
      }
      else
      {
        std::cout << COLOR_RED
                  << "Client connection failed"
                  << COLOR_NORMAL
                  << std::endl;
      }

      mConnected = true;
    }
  } while(false);

  return result;
}

void Client::sendMessage(std::string_view msg)
{
  Message packet;
  packet.id = mId;
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
  case EMessage::LoginResponse:
  {
    LoginResponse packet;
    if(!packet.deserialize(msg))
    {
      std::cout << "Wrong login response packet" << std::endl;
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

    auto it = std::find_if(std::begin(mUsers),
                        std::end(mUsers), [&](const User& usr)
                        {
                          return usr.id == packet.id;
                        });

    if(it != std::end(mUsers))
    {
      std::cout << COLOR_BLUE   << it->name << ": "
                << COLOR_NORMAL << packet.message << std::endl;
    }

    break;
  }
  case EMessage::RoomInfoResponse:
  {
    RoomInfoResponse packet;

    if(!packet.deserialize(msg))
    {
      std::cout << "Wrong RoomInfoResponse packet" << std::endl;
      break;
    }

    std::vector<User> newUsers;
    std::vector<User> lostUsers;

    auto userComp = [](const User& l, const User& r)
    {
      return l.id == r.id;
    };

    std::set_difference(packet.users.begin(), packet.users.end(),
                        mUsers.begin(), mUsers.end(),
                        std::back_inserter(newUsers), userComp);

    std::set_difference(mUsers.begin(), mUsers.end(),
                        packet.users.begin(), packet.users.end(),
                        std::back_inserter(lostUsers), userComp);

    if(newUsers.size() > 0)
    {
      std::cout << "User connected: ";
      for(const auto& user : newUsers)
      {
        std::cout << user.name << " ";
      }
      std::cout << std::endl;
    }

    if(lostUsers.size() > 0)
    {
      std::cout << "User disconnected: ";
      for(const auto& user : lostUsers)
      {
        std::cout << user.name << " ";
      }
      std::cout << std::endl;
    }

    mUsers = std::move(packet.users);

    break;
  }
  default:
    std::cout << "Wrong message: " << std::endl;
    std::copy(msg.begin(), msg.end(),
              std::ostream_iterator<int>(std::cout, " "));
  }
}