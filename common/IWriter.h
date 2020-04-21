#pragma once
#include <string_view>

class IWriter
{
public:
  virtual ~IWriter() = default;

  virtual bool open(std::string) = 0;
  virtual void write(std::string_view) = 0;
};