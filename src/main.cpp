#include "../nja.h"

#include <stdlib.h>

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

  auto files = os_scan_directory(thread_get_temporary_arena(), cwd);

  #if 0
  For (files) {
    auto path = path_join(dir, it.name);

    print("%.*s\n", LIT(path));
    print("  name:         %.*s\n", LIT(it.name));
    print("  size:         %llu\n", it.size);
    print("  date:         %llu\n", it.date);
    print("  is_directory: %d\n", it.is_directory);
  }
  #endif

  dump(sizeof(File_Info));

  For (files) {
    auto path = path_join(dir, it->name);

    print("%.*s\n", LIT(path));
    print("  name:         %.*s\n", LIT(it->name));
    print("  size:         %llu\n", it->size);
    print("  date:         %llu\n", it->date);
    print("  is_directory: %d\n", it->is_directory);
  }

  print("\n");
  print("Array:\n");

  Array<i32> my_array = {};

  dump(sizeof(Array<i32>));

  array_push(my_array, 0);
  array_push(my_array, 1);
  array_push(my_array, 2);
  array_push(my_array, 3);
  array_push(my_array, 4);
  array_push(my_array, 5);

  array_remove_ordered(my_array, 2, 3);

  i32 stuff[] = {42, 41, 40, 39};
  array_concat(my_array, stuff, count_of(stuff));

  For (my_array) {
    print("%d\n", it);
  }

  print("sort array:\n");
  array_sort(my_array);

  For (my_array) { print("%d\n", it); }

  print("\n");
  print("Hash_Table:\n");

  Hash_Table<i32, i32> my_table = {};

  dump(sizeof(Hash_Table<i32, i32>));

  table_add(my_table, 42, 42);
  table_add(my_table, 3, 3);
  table_add(my_table, 23, 23);
  table_add(my_table, 11, 11);

  #if 0
  For (my_table) {
    if (it.hash < TABLE_FIRST_VALID_HASH) continue;

    print("key: %d, value: %d\n", it.key, it.value);
  }
  #endif

  For_Table (it, my_table) {
    print("key: %d, value: %d\n", it->key, it->value);
  }

  return 0;
}