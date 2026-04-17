/*
    na_hotloader.h - v0.10
    Nick Aversano's C/C++ helper library

    This is a single header file for watching for file changes on your filesystem.
    Depends on "na.h"
===========================================================================

USAGE
    Define this in your source file:

    #define impl
    #include "na.h"
    #include "na_hotloader.h"

LICENSE
    This software is dual-licensed to the public domain and under the following
    license: you are granted a perpetual, irrevocable license to copy, modify,
    publish, and distribute this file as you see fit.

CREDITS
    Written by Nick Aversano
    Credits are much appreciated but not required.

VERSION HISTORY
    0.01  - Initial version
*/
#ifndef NA_HOTLOADER_H
#define NA_HOTLOADER_H

//
// NOTE(nick): Usage:
//
//   Hotloader hot = {0};
//   hotloader_init(&hot);
//   hotloader_add_path(&hot, S("C:/myproject"));
//
//   while (true) {
//     Hot_Change_Array changes = hotloader_get_events(temp_arena(), &hot);
//     for (i64 i = 0; i < changes.count; i++) {
//       print("Changed: %S\n", changes.data[i].name);
//     }
//     os_sleep(1);
//   }
//

typedef u32 Hot_Change_Type;
enum
{
    HotChange_Null,
    HotChange_Added,
    HotChange_Removed,
    HotChange_Modified,
    HotChange_Renamed,
    HotChange_COUNT,
};

typedef struct Hot_Change Hot_Change;
struct Hot_Change
{
    Hot_Change_Type type;
    String name;       // relative to watch path
    String prev_name;
    String file;       // absolute
    String prev_file;
    File_Info info;
};

typedef struct Hot_Change_Array Hot_Change_Array;
struct Hot_Change_Array
{
    _ArrayHeader_;
    Hot_Change *data;
};

typedef struct Hotloader Hotloader;

function b32              hotloader_init(Hotloader *hot);
function b32              hotloader_add_path(Hotloader *hot, String path);
function void             hotloader_free(Hotloader *hot);
function Hot_Change_Array hotloader_get_events(Arena *arena, Hotloader *hot);

typedef struct Hotloader_Find Hotloader_Find;
struct Hotloader_Find
{
    Hot_Change_Type type;
    String name;
    String file;
};

function Hot_Change *hotloader_find_change(Hot_Change_Array events, Hotloader_Find find);
function String      hotloader_change_type_to_string(Hot_Change_Type type);

//
// Implementation
//

#ifdef impl

#ifndef HOTLOADER_MAX_PATHS
#define HOTLOADER_MAX_PATHS 32
#endif

#if OS_WINDOWS

typedef struct Hot_Watch Hot_Watch;
struct Hot_Watch
{
    String path;
    HANDLE file;
    DWORD flags;
    OVERLAPPED overlapped;
    u8 buffer[1024];
};

struct Hotloader
{
    Hot_Watch watches[HOTLOADER_MAX_PATHS];
    u32 watch_count;
};

function b32 hotloader_init(Hotloader *hot)
{
    MemoryZero(hot, sizeof(*hot));
    return true;
}

function b32 hotloader_add_path(Hotloader *hot, String path)
{
    if (hot->watch_count >= HOTLOADER_MAX_PATHS) return false;

    M_Temp scratch = GetScratch(0, 0);

    HANDLE file = CreateFileA(
        string_to_cstr(scratch.arena, path),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        NULL
    );

    ReleaseScratch(scratch);

    if (file == INVALID_HANDLE_VALUE) return false;

    DWORD flags = FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE;

    Hot_Watch *w = &hot->watches[hot->watch_count++];
    w->path  = string_alloc(path);
    w->file  = file;
    w->flags = flags;
    w->overlapped.hEvent = CreateEvent(NULL, FALSE, 0, NULL);

    BOOL ok = ReadDirectoryChangesW(
        w->file, w->buffer, count_of(w->buffer), TRUE, w->flags, NULL, &w->overlapped, NULL);

    return ok;
}

function Hot_Change_Array hotloader_get_events(Arena *arena, Hotloader *hot)
{
    Hot_Change_Array results = {0};

    for (u32 wi = 0; wi < hot->watch_count; wi++)
    {
        Hot_Watch *w = &hot->watches[wi];

        if (WaitForSingleObject(w->overlapped.hEvent, 0) != WAIT_OBJECT_0) continue;

        DWORD bytes_transferred;
        GetOverlappedResult(w->file, &w->overlapped, &bytes_transferred, FALSE);

        FILE_NOTIFY_INFORMATION *event = cast(FILE_NOTIFY_INFORMATION *)w->buffer;

        while (true)
        {
            DWORD name_count = event->FileNameLength / sizeof(wchar_t);

            Hot_Change change = {0};
            change.name = string_from_string16(arena, Str16(event->FileName, name_count));

            switch (event->Action)
            {
                case FILE_ACTION_ADDED:    { change.type = HotChange_Added;    } break;
                case FILE_ACTION_REMOVED:  { change.type = HotChange_Removed;  } break;
                case FILE_ACTION_MODIFIED: { change.type = HotChange_Modified; } break;

                case FILE_ACTION_RENAMED_OLD_NAME:
                {
                    change.type      = HotChange_Renamed;
                    change.prev_name = change.name;
                    change.name      = StructLit(String){0};

                    if (event->NextEntryOffset)
                    {
                        FILE_NOTIFY_INFORMATION *next = cast(FILE_NOTIFY_INFORMATION *)((u8 *)event + event->NextEntryOffset);
                        if (next->Action == FILE_ACTION_RENAMED_NEW_NAME)
                        {
                            DWORD nc = next->FileNameLength / sizeof(wchar_t);
                            change.name = string_from_string16(arena, Str16(next->FileName, nc));
                            // consume FILE_ACTION_RENAMED_NEW_NAME
                            *((u8 **)&event) += event->NextEntryOffset;
                        }
                    }
                } break;

                case FILE_ACTION_RENAMED_NEW_NAME: break; // consumed above
            }

            if (change.type != HotChange_Null && change.name.count > 0)
            {
                change.file = path_join2(arena, w->path, change.name);
                change.info = os_get_file_info(change.file);
                if (change.prev_name.count > 0)
                {
                    change.prev_file = path_join2(arena, w->path, change.prev_name);
                }
                array_push(arena, &results, change);
            }

            if (!event->NextEntryOffset) break;
            *((u8 **)&event) += event->NextEntryOffset;
        }

        ReadDirectoryChangesW(
            w->file, w->buffer, count_of(w->buffer), TRUE, w->flags, NULL, &w->overlapped, NULL);
    }

    return results;
}

function void hotloader_free(Hotloader *hot)
{
    for (u32 i = 0; i < hot->watch_count; i++)
    {
        Hot_Watch *w = &hot->watches[i];
        string_free(&w->path);
        if (w->file) { CloseHandle(w->file); w->file = 0; }
    }
    hot->watch_count = 0;
}

#endif // OS_WINDOWS


#if OS_MACOS

#pragma push_macro("function")
#undef function
#include <CoreServices/CoreServices.h>
#pragma pop_macro("function")

// Internal malloc-backed linked list for FSEvent callback buffering
typedef struct Hot__Node Hot__Node;
struct Hot__Node
{
    Hot_Change change;
    Hot__Node *next;
    Hot__Node *prev;
};

struct Hotloader
{
    String paths[HOTLOADER_MAX_PATHS];
    u32 path_count;

    FSEventStreamRef stream;

    Hot__Node *pending_first;
    Hot__Node *pending_last;
    u64 pending_count;
};

static void macos__fsevent_callback(
    ConstFSEventStreamRef streamRef, void *info, size_t numEvents, void *eventPaths,
    const FSEventStreamEventFlags eventFlags[], const FSEventStreamEventId eventIds[]
) {
    Hotloader *hot = cast(Hotloader *)info;

    for (size_t i = 0; i < numEvents; i++)
    {
        char *path = ((char **)eventPaths)[i];
        FSEventStreamEventFlags flags = eventFlags[i];

        b32 created  = (flags & kFSEventStreamEventFlagItemCreated)  != 0;
        b32 removed  = (flags & kFSEventStreamEventFlagItemRemoved)  != 0;
        b32 renamed  = (flags & kFSEventStreamEventFlagItemRenamed)  != 0;
        b32 modified = (flags & kFSEventStreamEventFlagItemModified) != 0;

        Hot_Change_Type type = HotChange_Null;
        if      (created)  type = HotChange_Added;
        else if (removed)  type = HotChange_Removed;
        else if (renamed)  type = HotChange_Renamed;
        else if (modified) type = HotChange_Modified;

        if (type != HotChange_Null && path)
        {
            String path_str = string_from_cstr(path);
            Hot__Node *node = cast(Hot__Node *)malloc(sizeof(Hot__Node));
            MemoryZero(node, sizeof(*node));
            node->change.type = type;
            node->change.file = string_alloc(path_str);
            node->change.name = path_filename(node->change.file);
            DLLPushBack(hot->pending_first, hot->pending_last, node);
            hot->pending_count += 1;
        }
    }
}

function void macos__hotloader_rebuild_stream(Hotloader *hot)
{
    if (hot->stream)
    {
        FSEventStreamStop(hot->stream);
        FSEventStreamInvalidate(hot->stream);
        FSEventStreamRelease(hot->stream);
        hot->stream = NULL;
    }

    if (!hot->path_count) return;

    M_Temp scratch = GetScratch(0, 0);

    CFStringRef cf_paths[HOTLOADER_MAX_PATHS];
    for (u32 i = 0; i < hot->path_count; i++)
    {
        cf_paths[i] = CFStringCreateWithCString(NULL, string_to_cstr(scratch.arena, hot->paths[i]), kCFStringEncodingUTF8);
    }

    CFArrayRef paths_array = CFArrayCreate(NULL, (const void **)cf_paths, hot->path_count, &kCFTypeArrayCallBacks);

    FSEventStreamContext ctx = {0};
    ctx.info = hot;

    hot->stream = FSEventStreamCreate(
        NULL, macos__fsevent_callback, &ctx, paths_array,
        kFSEventStreamEventIdSinceNow, 0.05, kFSEventStreamCreateFlagFileEvents
    );

    FSEventStreamScheduleWithRunLoop(hot->stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    FSEventStreamStart(hot->stream);

    for (u32 i = 0; i < hot->path_count; i++) CFRelease(cf_paths[i]);
    CFRelease(paths_array);
    ReleaseScratch(scratch);
}

function b32 hotloader_init(Hotloader *hot)
{
    MemoryZero(hot, sizeof(*hot));
    return true;
}

function b32 hotloader_add_path(Hotloader *hot, String path)
{
    if (hot->path_count >= HOTLOADER_MAX_PATHS) return false;
    hot->paths[hot->path_count++] = string_alloc(path);
    macos__hotloader_rebuild_stream(hot);
    return true;
}

function Hot_Change_Array hotloader_get_events(Arena *arena, Hotloader *hot)
{
    Hot_Change_Array results = {0};

    // Pump run loop to flush pending FSEvents callbacks
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.0, false);

    Hot__Node *next = NULL;
    for (Hot__Node *it = hot->pending_first; it != NULL; it = next)
    {
        next = it->next;

        Hot_Change change    = it->change;
        change.file          = string_push(arena, it->change.file);
        change.name          = path_filename(change.file);
        change.info          = os_get_file_info(change.file);

        array_push(arena, &results, change);

        string_free(&it->change.file);
        free(it);
    }

    hot->pending_first = NULL;
    hot->pending_last  = NULL;
    hot->pending_count = 0;

    return results;
}

function void hotloader_free(Hotloader *hot)
{
    if (hot->stream)
    {
        FSEventStreamStop(hot->stream);
        FSEventStreamInvalidate(hot->stream);
        FSEventStreamRelease(hot->stream);
        hot->stream = NULL;
    }
    for (u32 i = 0; i < hot->path_count; i++)
    {
        string_free(&hot->paths[i]);
    }
    hot->path_count = 0;
}

#endif // OS_MACOS


#if OS_LINUX

#include <sys/inotify.h>
#include <unistd.h>

typedef struct Hot_Watch Hot_Watch;
struct Hot_Watch
{
    int wd;
    String path;
};

struct Hotloader
{
    int fd;
    Hot_Watch watches[HOTLOADER_MAX_PATHS];
    u32 watch_count;
};

function b32 hotloader_init(Hotloader *hot)
{
    MemoryZero(hot, sizeof(*hot));
    hot->fd = inotify_init1(IN_NONBLOCK);
    return hot->fd >= 0;
}

function b32 hotloader_add_path(Hotloader *hot, String path)
{
    if (hot->watch_count >= HOTLOADER_MAX_PATHS) return false;

    M_Temp scratch = GetScratch(0, 0);
    int wd = inotify_add_watch(hot->fd, string_to_cstr(scratch.arena, path),
        IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO);
    ReleaseScratch(scratch);

    if (wd < 0) return false;

    hot->watches[hot->watch_count++] = (Hot_Watch){ .wd = wd, .path = string_alloc(path) };
    return true;
}

function String linux__hotloader_path_for_wd(Hotloader *hot, int wd)
{
    for (u32 i = 0; i < hot->watch_count; i++)
    {
        if (hot->watches[i].wd == wd) return hot->watches[i].path;
    }
    return (String){0};
}

function Hot_Change_Array hotloader_get_events(Arena *arena, Hotloader *hot)
{
    Hot_Change_Array results = {0};

    // Buffer for matching IN_MOVED_FROM/IN_MOVED_TO pairs by cookie
    struct { u32 cookie; String name; String file; } pending_renames[64];
    u32 pending_rename_count = 0;

    u8 buf[4096] __attribute__((aligned(__alignof__(struct inotify_event))));

    while (true)
    {
        ssize_t len = read(hot->fd, buf, sizeof(buf));
        if (len <= 0) break;

        u8 *ptr = buf;
        u8 *end = buf + len;

        while (ptr < end)
        {
            struct inotify_event *e = cast(struct inotify_event *)ptr;
            ptr += sizeof(struct inotify_event) + e->len;

            String watch_path = linux__hotloader_path_for_wd(hot, e->wd);
            if (!watch_path.count) continue;

            String name = (e->len > 0) ? string_from_cstr(e->name) : (String){0};
            if (!name.count) continue;

            String file = path_join2(arena, watch_path, name);
            name = path_filename(file); // re-slice from arena copy

            if (e->mask & IN_MOVED_FROM)
            {
                if (pending_rename_count < count_of(pending_renames))
                {
                    pending_renames[pending_rename_count++] = (typeof(*pending_renames)){
                        .cookie = e->cookie, .name = name, .file = file
                    };
                }
                continue;
            }

            Hot_Change change = {0};
            change.name = name;
            change.file = file;

            if      (e->mask & IN_CREATE)   change.type = HotChange_Added;
            else if (e->mask & IN_DELETE)   change.type = HotChange_Removed;
            else if (e->mask & IN_MODIFY)   change.type = HotChange_Modified;
            else if (e->mask & IN_MOVED_TO)
            {
                change.type = HotChange_Renamed;
                for (u32 i = 0; i < pending_rename_count; i++)
                {
                    if (pending_renames[i].cookie == e->cookie)
                    {
                        change.prev_name = pending_renames[i].name;
                        change.prev_file = pending_renames[i].file;
                        pending_renames[i] = pending_renames[--pending_rename_count];
                        break;
                    }
                }
            }

            if (change.type != HotChange_Null)
            {
                change.info = os_get_file_info(change.file);
                array_push(arena, &results, change);
            }
        }
    }

    // Unmatched MOVED_FROM = renamed out of watch dir, treat as removed
    for (u32 i = 0; i < pending_rename_count; i++)
    {
        Hot_Change change = {0};
        change.type = HotChange_Removed;
        change.name = pending_renames[i].name;
        change.file = pending_renames[i].file;
        array_push(arena, &results, change);
    }

    return results;
}

function void hotloader_free(Hotloader *hot)
{
    for (u32 i = 0; i < hot->watch_count; i++)
    {
        inotify_rm_watch(hot->fd, hot->watches[i].wd);
        string_free(&hot->watches[i].path);
    }
    if (hot->fd >= 0) { close(hot->fd); hot->fd = -1; }
    hot->watch_count = 0;
}

#endif // OS_LINUX


function String hotloader_change_type_to_string(Hot_Change_Type type)
{
    String table[] = {
        S("Null"),
        S("Added"),
        S("Removed"),
        S("Modified"),
        S("Renamed"),
    };

    String result = {0};
    if (type < count_of(table))
    {
        result = table[type];
    }
    return result;
}

function Hot_Change *hotloader_find_change(Hot_Change_Array events, Hotloader_Find find)
{
    for (i64 i = 0; i < events.count; i++)
    {
        Hot_Change *it = &events.data[i];
        b32 match = false;
        if (find.name.count) match |= string_index(it->name, find.name, 0) >= 0;
        if (find.type)       match |= find.type == it->type;
        if (find.file.count) match |= string_find(it->file, find.file, 0, MatchFlag_IgnoreCase | MatchFlag_SlashInsensitive) < it->file.count;
        if (match) return it;
    }
    return NULL;
}

#endif // impl
#endif // NA_HOTLOADER_H
