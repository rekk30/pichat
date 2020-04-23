#pragma once
#include "Messages.h"

#include <string_view>
#include <string>

class ISerializer
{
public:
  virtual ~ISerializer() = default;

  virtual EMessage getType(std::string_view) = 0;

  virtual bool serialize(std::string&, Login&) = 0;
  virtual bool serialize(std::string&, LoginStatus&) = 0;
  virtual bool serialize(std::string&, Message&) = 0;

  virtual bool deserialize(std::string_view, Login&) = 0;
  virtual bool deserialize(std::string_view, LoginStatus&) = 0;
  virtual bool deserialize(std::string_view, Message&) = 0;
};