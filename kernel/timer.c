#include <kernel/irq.h>
#include <kernel/port_io.h>
#include <kernel/timer.h>
#include <stdint.h>

uint32_t timer_ticks = 0;

void timer_set_phase(int hz) {
  int divisor = 1193180 / hz;          /* Calculate our divisor */
  port_byte_out(0x43, 0x36);           /* Set our command byte 0x36 */
  port_byte_out(0x40, divisor & 0xFF); /* Set low byte of divisor */
  port_byte_out(0x40, divisor >> 8);   /* Set high byte of divisor */
}

void timer_handler(struct regs *r) {
  /* Increment our 'tick count' */
  timer_ticks++;
}

void install_timer() { install_irq_routine(0, timer_handler); }