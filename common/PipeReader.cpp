#include "PipeReader.h"
#include <iostream>

PipeReader::~PipeReader()
{
  if(mPipe.is_open())
  {
    mPipe.close();
  }
}

bool PipeReader::open(std::string path)
{
  mPipe.open(path, std::ios::out | std::ios::in | std::ios::binary);
  mPipe.unsetf(std::ios::skipws);

  return mPipe.is_open();
}

void PipeReader::setHandler(std::function<void(ByteArray)> handler)
{
  mHandler = handler;
}

void PipeReader::startLoop()
{
  mIsRunning = true;

  while(mIsRunning)
  {
    ByteArray buffer;
    getData(mPipe, buffer);

    if(buffer.size() > 0)
    {
      if(mHandler)
      {
        mHandler(std::move(buffer));
      }
    }
    mPipe.clear();
  }
}

void PipeReader::stopLoop()
{
  mIsRunning = false;
}

void PipeReader::getData(std::istream& stream, ByteArray& array)
{
  char ch;
  while (stream.get(ch) && ch != '\n')
    array.push_back(ch);
}
