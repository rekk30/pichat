#include "PipeReader.h"

PipeReader::~PipeReader()
{
  if(mPipe.is_open())
  {
    mPipe.close();
  }
}

bool PipeReader::open(std::string path)
{
  mPipe.open(path);

  return mPipe.is_open();
}

void PipeReader::setHandler(std::function<void(std::string_view)> handler)
{
  mHandler = handler;
}

void PipeReader::startLoop()
{
  mIsRunning = true;

  while(mIsRunning)
  {
    std::getline(mPipe, mBuffer);
    mPipe.clear();
    if(mBuffer.size() > 0)
    {
      if(mHandler)
      {
        mHandler(mBuffer);
      }
    }
  }
}

void PipeReader::stopLoop()
{
  mIsRunning = false;
}