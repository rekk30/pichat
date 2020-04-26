#pragma once
#include "IWriter.h"
#include <fstream>
#include <iostream>

class PipeWriter : public IWriter
{
public:
  ~PipeWriter() override;

  bool open(std::string path) override;
  void write(const ByteArray& msg) override;

private:
  std::ofstream mPipe;
};