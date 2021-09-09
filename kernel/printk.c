#include <kernel/kstl.h>
#include <stdint.h>

// The b-8000 is the newest development in our line of video buffers!
#define VIDEO_BUFFER ((char *)0xb8000)

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24

size_t line_number = 0;
size_t column_number = 0;

char *vbuf = VIDEO_BUFFER;

void next_line() {
  line_number++;
  column_number = 0;
  vbuf = (char *)((size_t)VIDEO_BUFFER + line_number * SCREEN_WIDTH * 2 +
                  column_number * 2);
}

void printk(const char *text) {
  for (int i = 0; text[i] != 0; ++i) {
    if (text[i] == '\n') {
      next_line();
    } else {
      if (column_number >= SCREEN_WIDTH) {
        next_line();
      }
      vbuf[0] = text[i];
      vbuf[1] = 0x0f;
      vbuf += 2;
      column_number++;
    }
  }
}
