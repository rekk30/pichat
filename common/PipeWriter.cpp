#include "PipeWriter.h"

PipeWriter::~PipeWriter()
{
  if(mPipe.is_open())
  {
    mPipe.close();
  }
}

bool PipeWriter::open(std::string path)
{
  mPipe.open(path, std::ios::in | std::ios::binary);

  return mPipe.is_open();
}

void PipeWriter::write(const ByteArray& msg)
{
  mPipe.write(reinterpret_cast<const char*>(msg.data()), msg.size());
  mPipe << std::endl;
}