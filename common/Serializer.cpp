#include "Serializer.h"

bool Serializer::serialize(std::string& str, Login& pkg)
{
  str += pkg.name.size();
  str += pkg.name;
  str += pkg.key.size();
  str += pkg.key;
  str += pkg.path.size();
  str += pkg.path;

  return true;
}

bool Serializer::serialize(std::string& str, LoginStatus& pkg)
{
  str += static_cast<uint8_t>(pkg.status);

  return true;
}

bool Serializer::deserialize(std::string_view& str, Login& pkg)
{
  size_t index = 0;
  size_t size = 0;

  size = std::stoul(str.substr(index, sizeof(size_t)));
  index += sizeof(size_t);
  pkg.name = str.substr(index, size);
  index += size;

  size = std::stoul(str.substr(index, sizeof(size_t)));
  index += sizeof(size_t);
  pkg.key = str.substr(index, size);
  index += size;

  size = std::stoul(str.substr(index, sizeof(size_t)));
  index += sizeof(size_t);
  pkg.path = str.substr(index, size);
  index += size;

  return true;
}

bool Serializer::deserialize(std::string_view& str, LoginStatus& pkg)
{
  uint8_t val;
  std::from_chars(str.data(),str.data() + str.size(), val);
  pkg.status = static_cast<ELoginStatus>(val);

  return true;
}