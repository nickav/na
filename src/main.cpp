#include "../nja.h"

#include <stdlib.h>

int main() {
  os_init();

  print("Hello, Sailor!\n");

  print("\n");
  print("Arena:\n");
  {
    Arena a = arena_create_from_allocator(os_allocator(), 1024); 

    void *ptr1 = arena_alloc(&a, 32);
    print("ptr1: %p\n", ptr1);
    assert(a.offset == 32 && a.prev_offset == 0);

    ptr1 = arena_realloc(&a, 64, 32, ptr1);
    print("ptr1: %p\n", ptr1);
    assert(a.offset == 64 && a.prev_offset == 0);

    arena_alloc(&a, 16);

    void *ptr2 = arena_realloc(&a, 128, 64, ptr1);
    print("ptr2: %p\n", ptr2);
    assert(ptr2 != ptr1);
    assert(a.offset == (64 + 16 + 128));
  }

  print("\n");
  print("Clipboard:\n");
  {
    auto result = os_clipboard_get_text();

    os_clipboard_set_text(S("Yoooooooooooooooo!"));

    print("clipboard text: %.*s\n", LIT(result));
  }

  print("\n");
  print("Files:\n");
  {
    auto exe_path = os_get_executable_path();
    print("executable path: %.*s\n", LIT(exe_path));

    auto dir = os_get_executable_directory();
    print("executable directory: %.*s\n", LIT(dir));

    auto cwd = os_get_current_directory();

    os_delete_entire_directory(path_join(cwd, S("foo")));
    assert(os_make_directory(path_join(cwd, S("foo"))));
    assert(os_make_directory(path_join(cwd, S("foo/bar"))));
    assert(os_write_entire_file(path_join(cwd, S("foo/bar/hello_sailor.txt")), S("This is a test file!")));

    assert(os_make_directory_recursive(path_join(cwd, S("this/is/a/recursive/test"))));

    auto files = os_scan_files_recursive(cwd);

    For (files) {
      auto path = path_join(dir, it->name);

      print("%.*s\n", LIT(path));
      print("  name:         %.*s\n", LIT(it->name));
      print("  size:         %llu\n", it->size);
      print("  date:         %llu\n", it->date);
      print("  is_directory: %d\n", it->is_directory);

      auto contents = os_read_entire_file(path);
      print("  os_read_entire_file: %lld\n", contents.count);
    }
  }

  print("\n");
  print("Array:\n");
  {
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

    print("\n");
    print("array_sort:\n");
    array_sort(my_array);

    For (my_array) { print("%d\n", it); }

    print("\n");
    print("Slice:\n");
    
    auto my_slice = slice_array(my_array, 2, 5);

    dump(my_slice.count);
    For (my_slice) { print("%d\n", it); }
  }

  print("\n");
  print("Hash_Table:\n");
  {
    Hash_Table<i32, i32> my_table = {};

    table_add(my_table, 42, 9);
    table_add(my_table, 3,  99);
    table_add(my_table, 23, 999);
    table_add(my_table, 11, 9999);

    For (my_table) {
      if (it.hash < TABLE_FIRST_VALID_HASH) continue;
      print("(key: %d, value: %d) ", it.key, it.value);
    }

    print("\n");

    For_Table (it, my_table) {
      print("(key: %d, value: %d) ", it->key, it->value);
    }

    print("\n");
  }

  print("\n");
  print("String Extensions:\n");
  {
    auto parts = string_split(S("This is a cool little test!"), S(" "));
    For (parts) { print("%.*s\n", LIT(it)); }

    auto result = string_join(parts, S("__"));
    print("%.*s\n", LIT(result));
  }

  return 0;
}