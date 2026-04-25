#define impl
#include "../na.h"
#include "../na_math.h"
#include "../na_window.h"

int app_run(String_Array args)
{
    window_init();

    Window_Create_Params params = {0};
    Window *window = window_create(v2i(1280, 800), S("Hello, Sailor!"), &(Window_Create_Params){ .center = true });

    while (!window_should_close(window))
    {
        arena_reset(temp_arena());
        f64 dt = window_begin_frame(temp_arena(), window);
        
        window_end_frame(window);
    }

    return 0;
}

#if OS_WINDOWS
int APIENTRY WinMain(HINSTANCE instance, HINSTANCE prev_inst, LPSTR argv, int show)
{
    os_init();
    String_Array args = string_array_from_c_array(temp_arena(), __argv, __argc);
    return app_run(args);
}
#else
int main(int argc, char **argv)
{
    os_init();
    String_Array args = string_array_from_c_array(temp_arena(), argv, argc);
    return app_run(args);
}
#endif // OS_WINDOWS
