#include "../nja.h"

int main() {
  os_init();

  print("Hello, Sailor!\n");

  auto result = os_clipboard_get_text();

  print("clipboard text: %.*s\n", LIT(result));


  return 0;
}