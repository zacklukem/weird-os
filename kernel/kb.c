#include <kernel/irq.h>
#include <kernel/kb.h>
#include <kernel/port_io.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/**
 * US Keyboard layout
 */
char kbdus[2][128] = {
    {
        0,    27,  '1', '2', '3',  '4', '5', '6', '7',  '8', // 9
        '9',  '0', '-', '=', '\b',                           // Backspace
        '\t',                                                // Tab
        'q',  'w', 'e', 'r',                                 // 19
        't',  'y', 'u', 'i', 'o',  'p', '[', ']', '\n',      // Enter key
        0,                                                   // 29   - Control
        'a',  's', 'd', 'f', 'g',  'h', 'j', 'k', 'l',  ';', // 39
        '\'', '`', 0,                                        // Left shift
        '\\', 'z', 'x', 'c', 'v',  'b', 'n',                 // 49
        'm',  ',', '.', '/', 0,                              // Right shift
        '*',  0,                                             // Alt
        ' ',                                                 // Space bar
        0,                                                   // Caps lock
        0,                                           // 59 - F1 key ... >
        0,    0,   0,   0,   0,    0,   0,   0,   0, // < ... F10
        0,                                           // 69 - Num lock
        0,                                           // Scroll Lock
        0,                                           // Home key
        0,                                           // Up Arrow
        0,                                           // Page Up
        '-',  0,                                     // Left Arrow
        0,    0,                                     // Right Arrow
        '+',  0,                                     // 79 - End key
        0,                                           // Down Arrow
        0,                                           // Page Down
        0,                                           // Insert Key
        0,                                           // Delete Key
        0,    0,   0,   0,                           // F11 Key
        0,                                           // F12 Key
        0,
    },
    {
        0,    27,  '!', '@', '#',  '$', '%', '^', '&',  '(', // 9
        ')',  ')', '_', '+', '\b',                           // Backspace
        '\t',                                                // Tab
        'Q',  'W', 'E', 'R',                                 // 19
        'T',  'Y', 'U', 'I', 'O',  'P', '{', '}', '\n',      // Enter key
        0,                                                   // 29   - Control
        'A',  'S', 'D', 'F', 'G',  'H', 'J', 'K', 'L',  ':', // 39
        '"',  '~', 0,                                        // Left shift
        '|',  'Z', 'X', 'C', 'V',  'B', 'N',                 // 49
        'M',  '<', '>', '?', 0,                              // Right shift
        '*',  0,                                             // Alt
        ' ',                                                 // Space bar
        0,                                                   // Caps lock
        0,                                           // 59 - F1 key ... >
        0,    0,   0,   0,   0,    0,   0,   0,   0, // < ... F10
        0,                                           // 69 - Num lock
        0,                                           // Scroll Lock
        0,                                           // Home key
        0,                                           // Up Arrow
        0,                                           // Page Up
        '-',  0,                                     // Left Arrow
        0,    0,                                     // Right Arrow
        '+',  0,                                     // 79 - End key
        0,                                           // Down Arrow
        0,                                           // Page Down
        0,                                           // Insert Key
        0,                                           // Delete Key
        0,    0,   0,   0,                           // F11 Key
        0,                                           // F12 Key
        0,
    }};

static char key_buffer[100];

FILE _k_stdin;

static size_t shift_down = 0;

/**
 * Handle Keyboard Interupt
 */
void keyboard_handler(struct regs *r) {
  uint8_t scancode;
  // Read scancode from keyboard port
  scancode = port_byte_in(0x60);

  // // If the top bit of the byte we read from the keyboard is
  // //  set, that means that a key has just been released
  if (scancode & 0x80) {
    scancode &= 0x7f;
    if (scancode == 42) {
      shift_down = 0;
    }

  } else {
    // get the key code and convert it to ASCII
    if (scancode == 42) {
      shift_down = 1;
    } else {
      uint8_t c = kbdus[shift_down][scancode];
      *++stdin->_IO_read_ptr = c;
    }
    // Store in a char buffer with a terminator so we can print it with
    // printk

    // char buf[2];

    // buf[0] = c;

    // buf[1] = 0;

    // printk(buf);
  }
}

/**
 * Initialize the keyboard driver
 */
void install_keyboard() {
  install_irq_routine(1, keyboard_handler);
  memset(key_buffer, 0, 100);
  stdin = &_k_stdin;
  stdin->_IO_read_base = key_buffer;
  stdin->_IO_read_ptr = key_buffer;
}