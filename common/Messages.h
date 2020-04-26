#pragma once
#include <cstdint>
#include <string>

enum class EMessage : uint8_t
{
  Error,
  Login,
  LoginStatus,
  Message
};

struct Login
{
  std::string name;
  std::string key;
  std::string path;

  bool serialize(ByteArray& packet)
  {
    packet << static_cast<uint8_t>(EMessage::Login)
           << name << key << path;

    return true;
  }
  bool deserialize(const ByteArray& packet)
  {
    packet >> name >> key >> path;

    return true;
  }
};

enum class ELoginStatus : uint8_t
{
  Failed = 0,
  Succeeded = 1
};

struct LoginStatus
{
  ELoginStatus status;

  bool serialize(ByteArray& packet)
  {
    packet << static_cast<uint8_t>(EMessage::LoginStatus)
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
  std::string name;
  std::string key;
  std::string message;

  bool serialize(ByteArray& packet)
  {
    packet << static_cast<uint8_t>(EMessage::Message)
           << name << key << message;

    return true;
  }
  bool deserialize(const ByteArray& packet)
  {
    packet >> name >> key >> message;

    return true;
  }
};