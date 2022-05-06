#include "stddef.h"
#include "syscall.h"
#include "i386.h"

void meeseeks_quieto_func(void);
void meeseeks_inquieto_func(void);

void task(void) {
  for (int i = 0; i < 10; i++) {
    syscall_meeseeks((uint32_t)&meeseeks_inquieto_func, 1, i);
  }
  
  // Estos meeseeks nunca deberian ser creados.
  syscall_meeseeks((uint32_t)&meeseeks_inquieto_func, 2, 0);
  syscall_meeseeks((uint32_t)&meeseeks_inquieto_func, 2, 1);
  syscall_meeseeks((uint32_t)&meeseeks_inquieto_func, 2, 2);
  syscall_meeseeks((uint32_t)&meeseeks_inquieto_func, 2, 3);

  while (1) {
    __asm volatile("nop");
  }
}

void meeseeks_quieto_func(void) {
  int8_t x, y;
  for (;;) {
    syscall_look(&x, &y);
  }
}

void meeseeks_inquieto_func(void) {
  for (;;) {
    syscall_move(1, 0);
  }
}
