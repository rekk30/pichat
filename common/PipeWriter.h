#pragma once
#include "IWriter.h"
#include <fstream>

class PipeWriter : public IWriter
{
public:
  ~PipeWriter() override;

  bool open(std::string path) override;
  void write(std::string_view msg) override;

private:
  std::ofstream mPipe;
};