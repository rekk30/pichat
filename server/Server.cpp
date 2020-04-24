#include "Server.h"

Server::Server(ISerializer& serializer, IReader& reader)
  : mSerializer{serializer}
  , mReader{reader}
{
}

void Server::start()
{
  using namespace std::placeholders;
  mReader.setHandler(std::bind(&Server::handleMessage, this, _1));

  mReader.startLoop();
}

void Server::handleMessage(std::string_view msg)
{
  switch(mSerializer.getType(msg))
  {
  case EMessage::Login:
  {
    Login packet;
    if(!mSerializer.deserialize(msg, packet))
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

    if(!mSerializer.deserialize(msg, packet))
    {
      std::cout << "Wrong message packet" << std::endl;
      break;
    }
    handle(packet);

    break;
  }
  default:
    std::cout << "Wrong message: " << std::endl
              << msg << std::endl;
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

    std::string data;
    LoginStatus status{ELoginStatus::Succeeded};

    auto serResult = mSerializer.serialize(data, status);
    if(!serResult)
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

    std::string data;
    if(!mSerializer.serialize(data, packet))
    {
      return;
    }

    for(auto &[_, user] : mUsers)
    {
      user.writer->write(data);
    }
}