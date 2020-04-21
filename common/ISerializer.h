#pragma once
#include "Messages.h"

#include <string_view>
#include <string>

class ISerializer
{
public:
  virtual ~ISerializer() = default;

  virtual bool serialize(std::string&, Login&) = 0;
  virtual bool serialize(std::string&, LoginStatus&) = 0;

  virtual bool deserialize(std::string_view&, Login&) = 0;
  virtual bool deserialize(std::string_view&, LoginStatus&) = 0;
};