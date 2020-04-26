#pragma once
#include "ByteArray.h"
#include <string>

class IWriter
{
public:
  virtual ~IWriter() = default;

  virtual bool open(std::string) = 0;
  virtual void write(const ByteArray&) = 0;
};