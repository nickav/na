//
// Compile:
// clang++ na_inliner.cpp -o na_inliner
// cl /Od -nologo -Zo -Z7 na_inliner.cpp /link -subsystem:console -incremental:no -opt:ref -OUT:na_inliner.exe
//
// Run:
// inliner <input> (output)
//

#define impl
#include "../na.h"

#include <stdio.h>

static Arena *g_arena = {};
static b32 ignore_comment = false;

bool na_inliner__file(String in_file, FILE *output)
{
    if (!os_file_exists(in_file)) return false;

    String content = os_read_entire_file(g_arena, in_file);
    if (!content.data)
    {
        return false;
    }

    for (i64 index = 0; index < content.count; index += 1)
    {
        u8 at = content.data[index];

        if (index < content.count - 1)
        {
            // NOTE(nick): skip comments
            if (at == '/')
            {
                if (content.data[index + 1] == '*')
                {
                    i64 start = index;
                    index += 2;

                    while (true)
                    {
                        if (content.data[index] == '*' && (index < content.count - 1 && content.data[index + 1] == '/'))
                        {
                            index += 2;
                            break;
                        }

                        index += 1;
                    }

                    String comment = string_slice(content, start, index);
                    fprintf(output, "%.*s", LIT(comment));
                    index -= 1;
                    continue;
                }

                if (content.data[index + 1] == '/')
                {
                    // NOTE(nick): hanlde special :InlinerIgnoreBegin / :InlinerIgnoreEnd blocks
                    String substr = string_eat_whitespace(string_slice(content, index+2, content.count));
                    if (string_starts_with(substr, S(":InlinerIgnoreBegin")))
                    {
                        while (!string_starts_with(substr, S(":InlinerIgnoreEnd")))
                        {
                            index += 1;
                            substr = string_slice(content, index, content.count);
                        }

                        index += S(":InlinerIgnoreEnd").count;
                        while (content.data[index] != '\n') { index += 1; }
                        continue;
                    }

                    // NOTE(nick): print other comments normally
                    i64 start = index;
                    index += 2;

                    while (index < content.count && content.data[index] != '\n') { index += 1; }

                    String comment = string_slice(content, start, index);
                    fprintf(output, "%.*s\n", LIT(comment));
                    continue;
                }
            }

            if (at == '#')
            {
                String substr = string_slice(content, index, content.count);

                // NOTE(nick): ignore #pragma once
                if (string_starts_with(substr, S("#pragma once")))
                {
                    index += S("#pragma once").count;
                    continue;
                }

                // NOTE(nick): handle includes
                if (string_starts_with(substr, S("#include \"")))
                {
                    i64 start = index + S("#include \"").count;

                    index = start;
                    while (content.data[index] != '"') index += 1;

                    String file_name = string_slice(content, start, index);
                    String include_path = path_join(path_dirname(in_file), file_name);
                    if (!na_inliner__file(include_path, output))
                    {
                        fprintf(output, "#include \"%.*s\"", LIT(file_name));
                        continue;
                    }

                    index += 1;
                    continue;
                }
            }
        }

        fprintf(output, "%c", at);
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
        String in_file = string_from_cstr(argv[1]);
        CHECK(na_inliner_print(in_file));
        return 0;
    }

    if (argc == 3)
    {
        String in_file = string_from_cstr(argv[1]);
        String out_file = string_from_cstr(argv[2]);
        CHECK(na_inliner_write(in_file, out_file));
        return 0;
    }

    printf("Too many options!\n");
    return -1;
}
