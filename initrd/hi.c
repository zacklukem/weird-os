void func(int a, ...) {}

void _start() {
  ((char *)0xc00b8000)[0] = 'h';
  ((char *)0xc00b8000)[1] = 0xf0;
  ((char *)0xc00b8000)[2] = 'i';
  ((char *)0xc00b8000)[3] = 0xf0;
  ((char *)0xc00b8000)[4] = '!';
  ((char *)0xc00b8000)[5] = 0xf0;
  func(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  __asm__ __volatile__("mov $42, %ebx\n"
                       "mov $41, %eax\n"
                       "int $0x80\n");
}
