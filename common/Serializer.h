#pragma once
#include "ISerializer.h"

#include <charconv>
#include <tuple>

class Serializer : public ISerializer
{
public:
  EMessage getType(std::string_view) override;

  bool serialize(std::string&, Login&) override;
  bool serialize(std::string&, LoginStatus&) override;
  bool serialize(std::string&, Message&) override;

  bool deserialize(std::string_view, Login&) override;
  bool deserialize(std::string_view, LoginStatus&) override;
  bool deserialize(std::string_view, Message&) override;
};