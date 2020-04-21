#pragma once
#include "IReader.h"
#include <fstream>

class PipeReader : public IReader
{
public:
  ~PipeReader() override;

  bool open(std::string path) override;
  void setHandler(std::function<void(std::string_view)> handler) override;
  void startLoop() override;
  void stopLoop() override;

private:
  std::ifstream mPipe;
  std::function<void(std::string_view)> mHandler;

  bool mIsRunning{false};
  std::string mBuffer;
};