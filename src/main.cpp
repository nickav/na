#include "../nja.h"

int main() {
  os_init();

  print("Hello, Sailor!\n");

  auto result = os_clipboard_get_text();

  os_clipboard_set_text(S("Yoooooooooooooooo!"));

  print("clipboard text: %.*s\n", LIT(result));

  auto exe_path = os_get_executable_path();
  print("executable path: %.*s\n", LIT(exe_path));

  auto dir = os_get_executable_directory();
  print("executable directory: %.*s\n", LIT(dir));

  auto cwd = os_get_current_directory();

  File_Info it = {};
  auto iter = os_file_list_begin(thread_get_temporary_arena(), dir);

  while (os_file_list_next(&iter, &it)) {
    auto path = path_join(dir, it.name);

    print("%.*s\n", LIT(path));
    print("  name:         %.*s\n", LIT(it.name));
    print("  size:         %llu\n", it.size);
    print("  date:         %llu\n", it.date);
    print("  is_directory: %d\n", it.is_directory);
  }

  os_file_list_end(&iter);


  return 0;
}