#include "Serializer.h"
#include <iostream>

namespace {
template <typename T>
auto parse_int(const char* first, const char* last, T& val)
-> std::pair<const char*, bool>
{
    auto[ptr, ec] = std::from_chars(first, last, val);
    return {ptr, ec == std::errc()};
}
} // anonymous

EMessage Serializer::getType(std::string_view str)
{
  uint8_t type{static_cast<uint8_t>(EMessage::Error)};

  parse_int(str.begin(), str.begin()+1, type);

  return static_cast<EMessage>(type);
}

bool Serializer::serialize(std::string& str, Login& pkg)
{
  str += std::to_string(static_cast<uint8_t>(EMessage::Login));
  str += std::to_string(pkg.name.size());
  str += pkg.name;
  str += std::to_string(pkg.key.size());
  str += pkg.key;
  str += std::to_string(pkg.path.size());
  str += pkg.path;

  return true;
}

bool Serializer::serialize(std::string& str, LoginStatus& pkg)
{
  str += std::to_string(static_cast<uint8_t>(EMessage::LoginStatus));
  str += std::to_string(static_cast<uint8_t>(pkg.status));

  return true;
}

bool Serializer::serialize(std::string& str, Message& pkg)
{
  str += std::to_string(static_cast<uint8_t>(EMessage::Message));
  str += std::to_string(pkg.key.size());
  str += pkg.key;
  str += std::to_string(pkg.message.size());
  str += pkg.message;

  return true;
}

bool Serializer::deserialize(std::string_view str, Login& pkg)
{
  bool result{false};
  size_t size{0};

  auto ptr = str.begin()+1;
  do
  {
    if(std::tie(ptr, result) = parse_int(ptr, ptr + str.size(), size); !result)
    {
      break;
    }

    pkg.name = {ptr, ptr+size};
    ptr += size;

    if(std::tie(ptr, result) = parse_int(ptr, ptr + str.size(), size); !result)
    {
      break;
    }

    pkg.key = {ptr, ptr+size};
    ptr += size;

    if(std::tie(ptr, result) = parse_int(ptr, ptr + str.size(), size); !result)
    {
      break;
    }

    pkg.path = {ptr, ptr+size};
    ptr += size;

    result = true;
  } while (false);

  return result;
}

bool Serializer::deserialize(std::string_view str, LoginStatus& pkg)
{
  uint8_t val;
  std::from_chars(str.begin() + 1,str.begin() + 2, val);
  pkg.status = static_cast<ELoginStatus>(val);

  return true;
}

bool Serializer::deserialize(std::string_view str, Message& pkg)
{
  bool result{false};
  size_t size{0};

  auto ptr = str.begin()+1;
  do
  {
    if(std::tie(ptr, result) = parse_int(ptr, ptr + str.size(), size); !result)
    {
      break;
    }

    pkg.key = {ptr, ptr+size};
    ptr += size;

    if(std::tie(ptr, result) = parse_int(ptr, ptr + str.size(), size); !result)
    {
      break;
    }

    pkg.message = {ptr, ptr+size};

    result = true;
  } while (false);

  return result;
}