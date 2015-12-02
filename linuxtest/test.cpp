#include <thread/thread.h>
#include <common.h>

#include "stdio.h"
#pragma comment(lib, "driver.lib")
int32_t main() {
  printf("1 + 5 = %d\n", add(1, 5));
  printf("int8_t = %d\n", sizeof(int8_t));
  printf("int16_t = %d\n", sizeof(int16_t));
  printf("int32_t = %d\n", sizeof(int32_t));
  printf("int63_t = %d\n", sizeof(int64_t));
  return 1;
}
