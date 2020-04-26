#pragma once
#include "ByteArray.h"
#include <functional>
#include <string_view>

class IReader
{
public:
  virtual ~IReader() = default;
  virtual bool open(std::string) = 0;
  virtual void setHandler(std::function<void(ByteArray)>) = 0;
  virtual void startLoop() = 0;
  virtual void stopLoop() = 0;
};