#pragma once

class Mutex
{
  public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();
};
