#include "../na.h"
#include "../na_math.h"

#include <stdlib.h>


struct Node {
  int data;

  Node *next;
  Node *prev;
};


int main() {
  os_init();

  print("Init time: %fs\n", os_time());

  print("Hello, Sailor!\n");

  print("\n\n");
  print("Arena:\n");
  {
    print("Virtual Memory:\n");

    Arena *b = arena_alloc_from_memory(gigabytes(2));

    arena_alloc(b, kilobytes(2));
    assert(b->commit_position == ARENA_INITIAL_COMMIT_SIZE);

    arena_alloc(b, kilobytes(1));
    assert(b->commit_position == ARENA_INITIAL_COMMIT_SIZE);
    arena_alloc(b, kilobytes(1));
    assert(b->commit_position == ARENA_COMMIT_BLOCK_SIZE);

    arena_alloc(b, ARENA_COMMIT_BLOCK_SIZE);
    assert(b->commit_position == (ARENA_COMMIT_BLOCK_SIZE * 2));

    arena_pop(b, ARENA_COMMIT_BLOCK_SIZE);
    assert(b->commit_position == (ARENA_COMMIT_BLOCK_SIZE));

    arena_reset(b);
    assert(b->commit_position == ARENA_INITIAL_COMMIT_SIZE);
  }

  print("\n\n");
  print("Clipboard:\n");
  {
    auto result = os_clipboard_get_text();

    os_clipboard_set_text(S("Yoooooooooooooooo!"));

    print("clipboard text: %.*s\n", LIT(result));
  }

  print("\n\n");
  print("Files:\n");
  {
    auto exe_path = os_get_executable_path();
    print("executable path: %.*s\n", LIT(exe_path));

    auto dir = os_get_executable_directory();
    print("executable directory: %.*s\n", LIT(dir));

    auto cwd = os_get_current_directory();
    print("cwd: %.*s\n", LIT(cwd));

    auto from_file = path_join(cwd, S("foo.txt"));
    auto to_file   = path_join(cwd, S("bar.txt"));
    os_write_entire_file(from_file, S("bar"));

    os_delete_file(to_file);
    assert(os_file_rename(from_file, to_file));

    os_delete_entire_directory(path_join(cwd, S("foo")));
    print("cwd: %.*s\n", LIT(cwd));
    assert(os_make_directory(path_join(cwd, S("foo"))));
    assert(os_make_directory(path_join(cwd, S("foo/bar"))));
    assert(os_write_entire_file(path_join(cwd, S("foo/bar/hello_sailor.txt")), S("This is a test file!")));

    assert(os_make_directory_recursive(path_join(cwd, S("this/is/a/recursive/test"))));

    print("scan files...");

    #if 0
    auto files = os_scan_files_recursive(cwd);
    Forp (files) {
      auto path = path_join(dir, it->name);

      print("%.*s\n", LIT(path));
      print("  name:         %.*s\n", LIT(it->name));
      print("  size:         %llu\n", it->size);
      print("  created_at:   %llu\n", it->created_at);
      print("  is_directory: %d\n", file_is_directory(it));

      auto contents = os_read_entire_file(path);
      print("  os_read_entire_file: %lld\n", contents.count);
    }
    #endif
  }

  print("\n\n");
  print("Array:\n");
  {
    Array<i32> my_array = {};

    array_push(&my_array, 0);
    array_push(&my_array, 1);
    array_push(&my_array, 2);
    array_push(&my_array, 3);
    array_push(&my_array, 4);
    array_push(&my_array, 5);

    array_remove_ordered(&my_array, 2, 3);

    i32 stuff[] = {42, 41, 40, 39};
    array_concat(&my_array, stuff, count_of(stuff));

    For (my_array) {
      print("%d\n", it);
    }

    print("\n");
    print("array_sort:\n");
    array_sort(&my_array);

    For (my_array) { print("%d\n", it); }

    print("\n");
    print("Slice:\n");
    
    auto my_slice = slice_array(my_array, 2, 5);

    dump(my_slice.count);
    For (my_slice) { print("%d\n", it); }
  }

  print("\n\n");
  print("Hash_Table:\n");
  {
    Hash_Table<i32, i32> my_table = {};

    table_add(&my_table, 42, 9);
    table_add(&my_table, 3,  99);
    table_add(&my_table, 23, 999);
    table_add(&my_table, 11, 9999);

    For (my_table) {
      if (it.hash < TABLE_FIRST_VALID_HASH) continue;
      print("(key: %d, value: %d) ", it.key, it.value);
    }

    print("\n");

    For_Table (my_table) {
      print("(key: %d, value: %d) ", it->key, it->value);
    }

    print("\n");
  }

  print("\n\n");
  print("String Extensions:\n");
  {
    auto parts = string_split(S("This is a cool little test!"), S(" "));
    For (parts) { print("%.*s\n", LIT(it)); }

    auto result = string_join(parts, S("__"));
    print("%.*s\n", LIT(result));
  }

  print("\n\n");
  print("Linked List Helpers:\n");
  {
    Node *sll_first = NULL;
    Node *sll_last = NULL;

    Node n1 = {4};
    SLLQueuePushBack(sll_first, sll_last, &n1);

    Node n2 = {2};
    SLLQueuePushBack(sll_first, sll_last, &n2);

    Node n3 = {42};
    SLLQueuePushFront(sll_first, sll_last, &n3);

    print("linked list items: ");
    {
      Node *it = sll_first;
      while (it != NULL) {
        print("%d, ", it->data);
        it = it->next;
      }
    }
    print("\n");

    SLLQueuePop(sll_first, sll_last);
    SLLQueuePop(sll_first, sll_last);
    SLLQueuePop(sll_first, sll_last);
    assert(sll_first == 0);


    Node *dll_first = NULL;
    Node *dll_last = NULL;

    Node d1 = {55};
    DLLPushBack(dll_first, dll_last, &d1);

    Node d2 = {511};
    DLLPushBack(dll_first, dll_last, &d2);

    Node d3 = {5199};
    DLLPushBack(dll_first, dll_last, &d3);

    Node d4 = {59192};
    DLLPushBack(dll_first, dll_last, &d4);

    print("doubly-linked list items (forwards): ");
    {
      Node *it = dll_first;
      while (it != NULL) {
        print("%d, ", it->data);
        it = it->next;
      }
    }
    print("\n");

    DLLRemove(dll_first, dll_last, &d2);

    print("doubly-linked list items (backwards): ");
    {
      Node *it = dll_last;
      while (it != NULL) {
        print("%d, ", it->data);
        it = it->prev;
      }
    }
    print("\n");

    DLLRemove(dll_first, dll_last, &d1);
    DLLRemove(dll_first, dll_last, &d3);
    DLLRemove(dll_first, dll_last, &d4);
    assert(dll_first == 0);


    Node *stack = 0;

    Node s1 = {99};
    SLLStackPush(stack, &s1);

    Node s2 = {7};
    SLLStackPush(stack, &s2);

    Node s3 = {2};
    SLLStackPush(stack, &s3);

    print("stack items: ");
    {
      Node *it = stack;
      while (it != NULL) {
        print("%d, ", it->data);
        it = it->next;
      }
    }
    print("\n");

    SLLStackPop(stack);
    SLLStackPop(stack);
    SLLStackPop(stack);
    assert(stack == 0);
  }

  print("Done! time in ms: %f\n", os_time_in_miliseconds());
  return 0;
}