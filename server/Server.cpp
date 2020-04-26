#include "Server.h"

Server::Server(IReader& reader)
  : mReader{reader}
{
}

void Server::start()
{
  using namespace std::placeholders;
  mReader.setHandler(std::bind(&Server::handleMessage, this, _1));

  mReader.startLoop();
}

void Server::handleMessage(ByteArray msg)
{
  uint8_t type{static_cast<uint8_t>(EMessage::Error)};
  msg >> type;

  switch(static_cast<EMessage>(type))
  {
  case EMessage::Login:
  {
    Login packet;
    if(!packet.deserialize(msg))
    {
      std::cout << "Wrong login packet" << std::endl;
      break;
    }
    handle(packet);

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
    handle(packet);

    break;
  }
  default:
    std::cout << "Wrong message: " << std::endl;
    std::copy(msg.begin(), msg.end(),
              std::ostream_iterator<int>(std::cout, " "));
  }
}

void Server::handle(Login packet)
{
    auto userWriter = std::make_unique<PipeWriter>();
    if(!userWriter->open(packet.path))
    {
      std::cout << "Wrong user pipe" << std::endl;
      return;
    }

    User newUser{std::move(packet.name),
                 std::move(packet.path),
                 std::move(userWriter)};

    mUsers.emplace(packet.key, std::move(newUser));

    ByteArray data;
    LoginStatus status{ELoginStatus::Succeeded};

    if(!status.serialize(data))
    {
      std::cout << "Serialization failed" << std::endl;
      return;
    }

    auto& user = mUsers.at(packet.key);
    user.writer->write(data);
    std::cout << "New user \"" << user.name << "\"" << std::endl;
}

void Server::handle(Message packet)
{
    auto it = mUsers.find(packet.key);
    if(it == mUsers.end())
    {
      std::cout << "Message from unauthorized used" << std::endl;
    }

    ByteArray data;
    if(!packet.serialize(data))
    {
      return;
    }

    for(auto &[_, user] : mUsers)
    {
      user.writer->write(data);
    }
}