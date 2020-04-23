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
  std::cout << "Msg: " << std::endl << msg << std::endl;
  switch(mSerializer.getType(msg))
  {
  case EMessage::Login:
  {
    std::cout << "New login attempt: " << std::endl
              << msg << std::endl;
    Login packet;
    if(!mSerializer.deserialize(msg, packet))
    {
      std::cout << "Wrong login packet" << std::endl;
      break;
    }

    auto userWriter = std::make_unique<PipeWriter>();
    if(!userWriter->open(packet.path))
    {
      std::cout << "Wrong user pipe" << std::endl;
      break;
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
      break;
    }

    auto& user = mUsers.at(packet.key);
    user.writer->write(data);
    std::cout << "New user \"" << user.name << "\"" << std::endl;
    std::cout << "Path: " << user.path << std::endl;

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

    auto it = mUsers.find(packet.key);
    if(it != mUsers.end())
    {
      auto& user = it->second;
      std::cout << user.name << ": " << packet.message << std::endl;
    }

    break;
  }
  default:
    std::cout << "Wrong message: " << std::endl
              << msg << std::endl;
  }
}