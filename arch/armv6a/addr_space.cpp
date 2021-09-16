// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <addr_space.h>

AddrSpace::AddrSpace()
{

}

void AddrSpace::setup()
{

}

AddrSpace& AddrSpace::kernel()
{
  return *(AddrSpace*)nullptr;
}

void AddrSpace::apply()
{

}

AddrSpace* AddrSpace::clone()
{
  return this;
}


void AddrSpace::map(size_t virt, size_t phys, int flags)
{

}


void AddrSpace::unmap(size_t virt)
{

}


bool AddrSpace::triggerCow(size_t virt)
{
  return false;
}

AddrSpace::~AddrSpace()
{

}
