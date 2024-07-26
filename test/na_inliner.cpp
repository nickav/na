//
//  cl /Od -nologo -Zo -Z7 nja_inliner.cpp /link -subsystem:windows -incremental:no -opt:ref -OUT:inliner.cpp
//

#include "na.h"

#include <stdio.h>

const String include_str = S("#include \"");
static Arena *g_arena = {};

#if 0
String path_dirname(String path) {
    // normalize path
    if (path.data[path.count - 1] == '/') path.count -= 1;

    for (i32 i = path.count - 1; i >= 0; i--) {
        if (path.data[i] == '/') {
            return string_slice(path, 0, i + 1);
        }
    }

    return S("./");
}
#endif

bool na_inliner__file(String in_file, FILE *output)
{
    auto content = os_read_entire_file(g_arena, in_file);
    if (!content.data)
    {
        printf("File doesn't exist! %.*s\n", LIT(in_file));
        return false;
    }

    i64 index = string_index(content, include_str, 0);
    if (index >= 0)
    {
        i64 prev_index = 0;

        while (index >= 0)
        {
            i64 end_quote_index = string_index(content, S("\""), index + include_str.count);
            if (end_quote_index < 0)
            {
                break;
            }

            auto prev = string_slice(content, prev_index, index);
            fprintf(output, "%.*s", LIT(prev));

            auto inc = string_slice(content, index + include_str.count, end_quote_index);
            if (inc.count)
            {
                auto inc_path = path_join(path_dirname(in_file), inc);
                auto inc_content = os_read_entire_file(g_arena, inc_path);
                if (inc_content.count)
                {
                    //print("\n// #include "%.*s"\n", LIT(inc));
                    fprintf(output, "%.*s", LIT(inc_content));
                    fprintf(output, "\n");
                }
            }

            index = end_quote_index + 1;

            prev_index = index;
            index = string_index(content, include_str, index + 1);
        }

        auto rest = string_slice(content, prev_index, content.count);
        fprintf(output, "%.*s", LIT(rest));
    }
    else
    {
        fprintf(output, "%.*s", LIT(content));
    }

    fflush(output);

    return true;
}

bool na_inliner_print(String in_file)
{
    return na_inliner__file(in_file, stdout);
}

bool na_inliner_write(String in_file, String out_file)
{
    FILE *f = fopen(string_to_cstr(temp_arena(), out_file), "wb");
    bool result = na_inliner__file(in_file, f);
    fclose(f);

    return result;
}

#define CHECK(result) do { if (!result) { return -1; } } while (0)

int main(int argc, char **argv)
{
    os_init();

    g_arena = arena_alloc(Gigabytes(1));

    if (argc <= 1)
    {
        printf("inliner <input> (output)\n");
        return 0;
    }

    if (argc == 2)
    {
        auto in_file = string_from_cstr(argv[1]);
        CHECK(na_inliner_print(in_file));
        return 0;
    }

    if (argc == 3)
    {
        auto in_file = string_from_cstr(argv[1]);
        auto out_file = string_from_cstr(argv[2]);
        CHECK(na_inliner_write(in_file, out_file));
        return 0;
    }

    printf("Too many options!\n");
    return -1;
}