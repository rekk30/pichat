#pragma once
#include "IReader.h"
#include <fstream>
#include <iterator>

class PipeReader : public IReader
{
public:
  ~PipeReader() override;

  bool open(std::string path) override;
  void setHandler(std::function<void(ByteArray)> handler) override;
  void startLoop() override;
  void stopLoop() override;

private:
  void getData(std::istream& stream, ByteArray& array);

private:
  std::ifstream mPipe;
  std::function<void(ByteArray)> mHandler;

  bool mIsRunning{false};
};