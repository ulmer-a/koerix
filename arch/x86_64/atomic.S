.section .text

// size_t xchg(size_t val, size_t* mem)
.global xchg
xchg:
  xchg %rdi, (%rsi)
  mov %rdi, %rax
  retq

// size_t atomic_add(size_t* mem, ssize_t increment)
.global atomic_add
atomic_add:
  lock xadd %rsi, (%rdi)
  mov %rsi, %rax
  ret

// uint8_t atomic_add8(uint8_t* mem, int8_t increment)
.global atomic_add8
atomic_add8:
  mov %rsi, %rax
  lock xaddb %al, (%rdi)
  movzb %al, %rax
  ret
