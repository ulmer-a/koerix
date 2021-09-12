// SPDX-FileCopyrightText: 2005-2020 Rich Felker, et al.
// SPDX-License-Identifier: MIT

#ifndef LOCK_H
#define LOCK_H

hidden void __lock(volatile int *);
hidden void __unlock(volatile int *);
#define LOCK(x) __lock(x)
#define UNLOCK(x) __unlock(x)

#endif
