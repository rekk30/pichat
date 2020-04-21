#pragma once
#include <string_view>
#include <string>

class ICredentials
{
public:
  virtual ~ICredentials() = default;

  virtual std::string encode(std::string_view) = 0;
  virtual std::string decode(std::string_view) = 0;

  virtual std::string getName() = 0;
  virtual std::string getPublicKey() = 0;
};