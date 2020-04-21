#include "Credentials.h"

Credentials::Credentials(std::string name, std::string key)
  : mName{name}
  , mPublicKey{key}
{
}

std::string Credentials::encode(std::string_view msg)
{
  std::string encodedMsg{msg};

  return encodedMsg;
}

std::string Credentials::decode(std::string_view msg)
{
  std::string decodedMsg{msg};

  return decodedMsg;
}

std::string Credentials::getName()
{
  return mName;
}

std::string Credentials::getPublicKey()
{
  return mPublicKey;
}