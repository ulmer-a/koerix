// SPDX-FileCopyrightText: 2017-2021 Alexander Ulmer
// SPDX-License-Identifier: LGPL-2.1-or-later

// void* memset(void* dest, uint8_t c, size_t len);
.global memset
.type memset,@function
memset:
  mov %rdi, %r8
  mov %sil, %al
  mov %rdx, %rcx
  rep
  stosb
  mov %r8, %rax
  ret

// size_t strlen(const char* str)
.global strlen
.type strlen,@function
strlen:
  mov %rdi, %rax
  sl1:
  movb (%rax), %cl
  test %cl, %cl
  jz sl2
  inc %rax
  jmp sl1
  sl2:
  sub %rdi, %rax
  ret

// SPDX-FileCopyrightText: 2005-2020 Rich Felker, et al.
// SPDX-License-Identifier: MIT

.global memcpy
.global __memcpy_fwd
.hidden __memcpy_fwd
.type memcpy,@function
memcpy:
__memcpy_fwd:
  mov %rdi,%rax
  cmp $8,%rdx
  jc 1f
  test $7,%edi
  jz 1f
2:
  movsb
  dec %rdx
  test $7,%edi
  jnz 2b
1:
  mov %rdx,%rcx
  shr $3,%rcx
  rep
  movsq
  and $7,%edx
  jz 1f
2:
  movsb
  dec %edx
  jnz 2b
1:
  ret
