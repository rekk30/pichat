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
  mPipe.open(path);

  return mPipe.is_open();
}

void PipeWriter::write(std::string_view msg)
{
  mPipe << msg << std::endl;
  mPipe.flush();
}