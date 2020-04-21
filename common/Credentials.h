#pragma once
#include "ICredentials.h"

class Credentials : public ICredentials
{
public:
  Credentials(std::string name, std::string key);
  std::string encode(std::string_view msg) override;
  std::string decode(std::string_view msg) override;

  std::string getName() override;
  std::string getPublicKey() override;

private:
  std::string mName;
  std::string mPublicKey;
};