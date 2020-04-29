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
  case EMessage::LoginRequest:
  {
    LoginRequest packet;
    if(!packet.deserialize(msg))
    {
      std::cout << "Wrong login request packet" << std::endl;
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
  case EMessage::RoomInfoRequest:
  {
    RoomInfoRequest packet;

    if(!packet.deserialize(msg))
    {
      std::cout << "Wrong RoomInfoRequest packet" << std::endl;
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

void Server::handle(LoginRequest packet)
{
  auto userWriter = std::make_unique<PipeWriter>();
  if(!userWriter->open(packet.path))
  {
    std::cout << "Wrong user pipe" << std::endl;

    return;
  }

  UserConnection newUser{std::move(packet.name),
                          std::move(packet.path),
                          std::move(userWriter)};

  mUsers.emplace(packet.id, std::move(newUser));

  ByteArray data;
  LoginResponse status{ELoginStatus::Succeeded};

  if(!status.serialize(data))
  {
    std::cout << "Serialization failed" << std::endl;
    return;
  }

  auto& user = mUsers.at(packet.id);
  user.writer->write(data);
  std::cout << "New user \"" << user.name << "\"" << std::endl;

  RoomInfoResponse resp;
  for(const auto&[id, user] : mUsers)
  {
    resp.users.emplace_back(User{id, user.name});
  }

  ByteArray infoData;
  if(!resp.serialize(infoData))
  {
    return;
  }

  for(const auto& user : mUsers)
  {
    user.second.writer->write(infoData);
  }
}

void Server::handle(Message packet)
{
  auto it = mUsers.find(packet.id);
  if(it == mUsers.end())
  {
    std::cout << "Message from unauthorized user" << std::endl;
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

void Server::handle(RoomInfoRequest packet)
{
  auto it = mUsers.find(packet.id);
  if(it == mUsers.end())
  {
    std::cout << "RoomInfoRequest from unauthorized user" << std::endl;
  }

  RoomInfoResponse resp;
  for(const auto&[id, user] : mUsers)
  {
    resp.users.emplace_back(User{id, user.name});
  }

  ByteArray data;
  if(!resp.serialize(data))
  {
    return;
  }

  (*it).second.writer->write(data);
}