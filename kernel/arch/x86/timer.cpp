#include <arch/x86/irq.h>
#include <arch/x86/port_io.h>
#include <kernel/timer.h>
#include <stdint.h>

uint32_t timer_ticks = 0;

void sleep(uint32_t ms) {
  uint32_t timer_start = timer_ticks;
  while (timer_ticks - timer_start < ms) {
    __asm__("hlt");
  }
}

static void timer_set_phase(int hz) {
  int divisor = 1193180 / hz; /* Calculate our divisor */
  outb(0x43, 0x36);           /* Set our command byte 0x36 */
  outb(0x40, divisor & 0xFF); /* Set low byte of divisor */
  outb(0x40, divisor >> 8);   /* Set high byte of divisor */
}

static void timer_handler(struct regs *r) {
  /* Increment our 'tick count' */
  timer_ticks++;
}

void install_timer() {
  timer_set_phase(1000);
  install_irq_routine(0, timer_handler);
}
