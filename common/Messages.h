#pragma once
#include <cstdint>
#include <string>

enum class EMessage : uint8_t
{
  Error,
  LoginRequest,
  LoginResponse,
  Message,
  RoomInfoRequest,
  RoomInfoResponse
};

struct LoginRequest
{
  std::string name;
  std::string id;
  std::string path;

  bool serialize(ByteArray& packet)
  {
    packet << static_cast<uint8_t>(EMessage::LoginRequest)
           << name << id << path;

    return true;
  }
  bool deserialize(const ByteArray& packet)
  {
    packet >> name >> id >> path;

    return true;
  }
};

enum class ELoginStatus : uint8_t
{
  Failed = 0,
  Succeeded = 1
};

struct LoginResponse
{
  ELoginStatus status;

  bool serialize(ByteArray& packet)
  {
    packet << static_cast<uint8_t>(EMessage::LoginResponse)
           << static_cast<uint8_t>(status);

    return true;
  }
  bool deserialize(const ByteArray& packet)
  {
    uint8_t val;
    packet >> val;
    status = static_cast<ELoginStatus>(val);

    return true;
  }
};

struct Message
{
  std::string id;
  std::string message;

  bool serialize(ByteArray& packet)
  {
    packet << static_cast<uint8_t>(EMessage::Message)
           << id << message;

    return true;
  }
  bool deserialize(const ByteArray& packet)
  {
    packet >> id >> message;

    return true;
  }
};

struct RoomInfoRequest
{
  std::string id;

  bool serialize(ByteArray& packet)
  {
    packet << static_cast<uint8_t>(EMessage::RoomInfoRequest)
           << id;

    return true;
  }
  bool deserialize(const ByteArray& packet)
  {
    packet >> id;

    return true;
  }
};

struct User
{
  std::string id;
  std::string name;
};

struct RoomInfoResponse
{
  std::vector<User> users;

  bool serialize(ByteArray& packet)
  {
    packet << static_cast<uint8_t>(EMessage::RoomInfoResponse)
           << users.size();
    for(const auto& user : users)
    {
      packet << user.id << user.name;
    }

    return true;
  }
  bool deserialize(const ByteArray& packet)
  {
    size_t size;
    packet >> size;
    for(int i = 0; i < size; ++i)
    {
      User usr;
      packet >> usr.id >> usr.name;
      users.push_back(usr);
    }

    return true;
  }
};