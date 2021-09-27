#include <arch/x86/port_io.h>
#include <drivers/vga.h>
#include <kernel/fs/fs.h>
#include <kernel/fs/udev.h>
#include <kernel/printk.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// The b-8000 is the newest development in our line of video buffers!
#define VIDEO_BUFFER ((char *)0xc00b8000)

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

using namespace vga;

static size_t line_number = 0;   ///< The current cursor line number
static size_t column_number = 0; ///< The current cursor column

static char *vbuf = VIDEO_BUFFER; ///< Pointer to the cursor location

FILE _k_stdout;

/**
 * Get the cursor offset
 */
uint16_t vga::get_cursor() {
  outb(REG_SCREEN_CTRL, 14);
  int offset = inb(REG_SCREEN_DATA) << 8; // high byte
  outb(REG_SCREEN_CTRL, 15);
  offset += inb(REG_SCREEN_DATA); // low byte

  return offset;
}

/**
 * Set the cursor offset
 */
void vga::set_cursor(uint16_t offset) {
  outb(REG_SCREEN_CTRL, 14);
  outb(REG_SCREEN_DATA, (offset >> 8) & 0xff); // high byte
  outb(REG_SCREEN_CTRL, 15);
  outb(REG_SCREEN_DATA, offset & 0xff); // low byte
}

/**
 * Set the cursor offset to the current cursor position
 */
static void place_cursor() {
  set_cursor(SCREEN_WIDTH * line_number + column_number);
  vbuf = (char *)((size_t)VIDEO_BUFFER + line_number * SCREEN_WIDTH * 2 +
                  column_number * 2);
}

/**
 * Scroll down n lines
 */
static void scroll_lines(int l) {
  uint16_t *vb = (uint16_t *)VIDEO_BUFFER;
  for (int i = l; i < SCREEN_HEIGHT; ++i) {
    memcpy(&vb[(i - 1) * SCREEN_WIDTH], &vb[i * SCREEN_WIDTH],
           SCREEN_WIDTH * 2);
  }

  for (int i = SCREEN_HEIGHT - l; i < SCREEN_HEIGHT; ++i) {
    memset(&vb[i * SCREEN_WIDTH], 0, SCREEN_WIDTH * 2);
  }
}

/**
 * Move to new line and carrage return
 */
static void next_line() {
  line_number++;
  if (line_number >= SCREEN_HEIGHT) {
    scroll_lines(1);
    line_number = SCREEN_HEIGHT - 1;
  }
  column_number = 0;
  place_cursor();
}

/**
 * Clear the screen
 */
void cleark() {
  vbuf = VIDEO_BUFFER;
  for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * 2; ++i) {
    vbuf[i] = 0x0;
  }
  line_number = 0;
  column_number = 0;
  place_cursor();
}

/**
 * Add char to end of buffer and move cursor.
 */
static void kputc(char c) {
  vbuf[0] = c;
  vbuf[1] = 0x0f;
  vbuf += 2;
  column_number++;
  place_cursor();
}

/**
 * Add char to buffer while checking if it is a control character
 */
static int kputchar(int c) {
  if (c == '\n') {
    next_line();
  } else if (c == '\r') {
    column_number = 0;
    place_cursor();
  } else {
    if (column_number >= SCREEN_WIDTH) {
      next_line();
    }
    kputc(c);
  }
  return c;
}

char out_buffer[100];

static int io_flush_handler() {
  for (int i = 0; out_buffer[i] != 0; ++i) {
    kputchar(out_buffer[i]);
  }
  memset(out_buffer, 0, 100);
  stdout->_IO_write_ptr = out_buffer;
  return 0;
}

void k_init_printk() {
  stdout = &_k_stdout;
  stdout->_IO_write_base = out_buffer;
  stdout->_IO_write_ptr = out_buffer;
  stdout->_flush_func = io_flush_handler;
}

class vga_dev : public fs::inode {
public:
  vga_dev(rc<fs::dirent> m_dirent, ino_t id)
      : fs::inode(m_dirent, id, S_IFCHR | S_IRWXA) {}
  virtual ~vga_dev() override {}
  virtual ssize_t read(void *buf, size_t nbyte, off_t offset) override {
    return 0;
  };
  virtual ssize_t write(const void *buf, size_t n, off_t offset) override {
    const char *buf_c = (char *)buf;
    size_t i;
    for (i = 0; i < n; i++) {
      kputchar(*buf_c++);
    }
    return i;
  }
};

void vga::init_vga() { fs::virt_udev->make_file<vga_dev>("vga"); }

/**
 * Print a string
 */
void printk(const char *text) { puts(text); }
