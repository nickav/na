/*
    na_net.h - v0.02
    Nick Aversano's C++ networking library for TCP/UDP and HTTP
===========================================================================

LICENSE
    This software is dual-licensed to the public domain and under the following
    license: you are granted a perpetual, irrevocable license to copy, modify,
    publish, and distribute this file as you see fit.

CREDITS
    Written by Nick Aversano

VERSION HISTORY
    0.02  - Update to na.h v0.07
    0.01  - Initial release
*/

#ifndef NA_NET_H
#define NA_NET_H

//
// Simple HTTP Example:
//
#if 0
    socket_init();

    auto r = http_get(S("http://nickav.co/"));

    while (r.status == HttpStatus_Pending) {
        reset_temporary_storage();

        http_process(&r);
        os_sleep(1.0f);
    }

    if (r.status == HttpStatus_Completed)
    {
        dump(r.status_code);
        dump(r.content_type);

        auto headers = http_parse_headers(r.response_headers);
        For (headers) {
            print("%S = %S\n", it.key, it.value);
        }

        dump(r.response_body);
    }
#endif

//
// HTTP Request Manager Example:
//
#if 0
    socket_init();

    auto r1 = http_get(S("http://nickav.co/"));
    http_add(&man, r1);

    auto r2 = http_get(S("http://nickav.co/images/dronepollock.jpg"));
    http_add(&man, r2);

    while (man.requests.count > 0)
    {
        http_update(&man);

        Forp (man.requests)
        {
            if (it->status == HttpStatus_Pending) continue;

            print("finished! ");
            dump(it->status_code);
            dump(it->content_type);
        }

        os_sleep(1.0f);
    }
#endif

//
// Simple TCP Example
//
#if 0
    Socket socket = socket_create_tcp_server(socket_make_address_from_url(S("127.0.0.1:3000")));
    Socket client;

    while (1)
    {
        reset_temporary_storage();

        Socket client;
        Socket_Address client_address;
        if (socket_accept(&socket, &client, &client_address))
        {
            print("Got request from %S:%d\n", socket_get_address_name(client_address), client_address.port);
            
            auto request = socket_recieve_entire_stream(temp_arena(), &client);
            socket_send(&client, {}, S("HTTP/1.1 200 OK\r\n\r\nHello, World!\r\n\r\n"));
            socket_close(&client);
        }

        os_sleep(1);
    }
#endif

//
// UDP Client/Server Example
//
#if 0
    socket_init();

    Socket_Address server_address = socket_make_address_from_url(S("127.0.0.1:9999"));

    Socket sock = socket_create_udp_server(server_address);
    bool is_server = socket_is_valid(sock);
    if (!is_server)
    {
        sock = socket_create_udp_client(server_address);
    }

    if (is_server)
    {
        print("Server started!\n");
    }
    else
    {
        print("Client started!\n");
    }

    if (!is_server)
    {
        socket_send(&sock, server_address, S("Hello from Hawaii's amazing adventure"));
    }

    while (true)
    {
        if (is_server)
        {
            String buffer = make_string((char *)talloc(1024), 1024);
            Socket_Address address;
            if (socket_recieve(&sock, &buffer, &address))
            {
                print("Got bytes: %S\n", buffer);
            }
        }

        os_sleep(1);
    }

    return 0;
#endif

//
// TODO(nick):
// - handle chunked responses
// - replace `select` calls with Linux epoll / win32 / MacOS kqueue solution
//

typedef u32 Socket_Type;
enum
{
    SocketType_TCP,
    SocketType_UDP,
};

typedef struct Socket Socket;
struct Socket
{
    Socket_Type type;
    i64  handle;
};

typedef struct Socket_Address Socket_Address;
struct Socket_Address
{
    u32 host; // ipv4
    //u64 host[2]; // ipv6 requires more space
    u16 port;
};

typedef u32 Http_Status;
enum
{
    HttpStatus_Pending,
    HttpStatus_Completed,
    HttpStatus_Failed,
};

typedef struct Http Http;
struct Http
{
    Http *next;

    Socket      socket;
    Http_Status status;

    // Request data
    String request_data;
    bool   is_ready;
    bool   request_sent;

    // Response data
    String response_data;
    String response_headers;
    String response_body;

    int    status_code;
    String content_type;
};

typedef struct Http_Header Http_Header;
struct Http_Header
{
    String key;
    String value;
};

typedef struct Http_Header_Array Http_Header_Array;
struct Http_Header_Array
{
    Http_Header *data;
    u64 count;
    u64 capacity;
};

typedef struct Http_Request Http_Request;
struct Http_Request
{
    Socket_Address address;
    String method;
    String url;
};

typedef struct Http_Response Http_Response;
struct Http_Response
{
    i32 status_code;
    Http_Header_Array headers;
    String content_type;
    String body;
};


typedef struct Http_Url_Parts Http_Url_Parts;
struct Http_Url_Parts
{
    String protocol; // http or https
    String host;     // nickav.co
    i64    port;     // 80
    String path;     // robots.txt
};

typedef struct Http_Manager Http_Manager;
struct Http_Manager
{
    Arena *arena;
    Http *first_request;
    Http *last_request;
};


//
// Socket methods
//

function bool socket_init();
function void socket_shutdown();

function Socket socket_open(Socket_Type type);
function void socket_close(Socket *socket);

function bool socket_bind(Socket *socket, Socket_Address address);
function bool socket_connect(Socket *socket, Socket_Address address);
function bool socket_listen(Socket *socket);
function bool socket_accept(Socket *socket, Socket_Address *address);

function bool socket_can_write(Socket *socket);

function bool socket_is_ready(Socket *socket);
function bool socket_send(Socket *socket, Socket_Address address, String message);
function i64 socket_recieve_bytes(Socket *socket, u8 *data, u64 count, Socket_Address *address);
function bool socket_recieve(Socket *socket, String *buffer, Socket_Address *address);

function String socket_get_address_name(Socket_Address address);

function Socket_Address socket_make_address(const char *host, u16 port);
function Socket_Address socket_make_address_from_url(String url);

function Socket socket_create_udp_server(Socket_Address address);
function Socket socket_create_udp_client(Socket_Address address);


//
// HTTP methods
//

function Http http_request(String verb, String url, String headers, String data);

function Http http_get(String url);
function Http http_post(String url, String data);
function Http http_put(String url, String data);
function Http http_delete(String url);

function void http_process(Http *http);


function Http_Header_Array http_parse_headers(Arena *arena, String headers);
function Http_Request http_parse_request(String request);

function void http_manager_init(Http_Manager *manager);
function void http_manager_add(Http_Manager *manager, Http *request);
function void http_manager_update(Http_Manager *manager);
function Http *http_manager_get_completed(Http_Manager *manager);



#endif // NA_NET_H


#ifdef impl

#if _WIN32

#ifndef OS_WINDOWS
#define OS_WINDOWS 1
#endif

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <winsock2.h>
#include <ws2tcpip.h>

// NOTE(nick): all the inet_ functions, etc, need to be loaded from the DLL
#pragma comment(lib, "ws2_32.lib")

static HMODULE w32_socket_lib;

typedef int WSAStartup_Func(WORD wVersionRequired, LPWSADATA lpWSAData);
static WSAStartup_Func *W32_WSAStartup;

typedef int WSAGetLastError_Func();
static WSAGetLastError_Func *W32_WSAGetLastError;

typedef void WSACleanup_Func();
static WSACleanup_Func *W32_WSACleanup;

#define SOCKET_WOULD_BLOCK WSAEWOULDBLOCK
#define SOCKET_IN_PROGRESS WSAEINPROGRESS 
#define SOCKET_LAST_ERROR() WSAGetLastError()

#else // _WIN32

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/select.h>

#define SOCKET_ERROR   -1
#define INVALID_SOCKET -1

#define SOCKET_WOULD_BLOCK EWOULDBLOCK
#define SOCKET_IN_PROGRESS WINPROGRESS 
#define SOCKET_LAST_ERROR() errno

#endif // _WIN32


function Socket_Address socket_make_address(const char *host, u16 port)
{
    Socket_Address result = {0};

    if (host == NULL)
    {
        result.host = INADDR_ANY;
    }
    else
    {
        inet_pton(AF_INET, host, &result.host);
        if (result.host == 0 || result.host == INADDR_NONE)
        {
            struct hostent *hostent = gethostbyname(host);
            if (hostent)
            {
                MemoryCopy(hostent->h_addr, &result.host, hostent->h_length);
            }
            else
            {
                print("[socket] Invalid host name: %s\n", host);
                return result;
            }
        }

        /*
        inet_pton(AF_INET6, host, &result.host[0]);
        */
    }

    result.port = port;

    return result;
}

function Http_Url_Parts socket_parse_url(String url)
{
    Http_Url_Parts result = {0};

    i64 index;

    index = string_find(url, S("://"), 0, 0);
    if (index < url.count)
    {
        result.protocol = string_slice(url, 0, index);
        url = string_slice(url, index + S("://").count, url.count);
    }

    index = string_find(url, S("/"), 0, 0);
    if (index < url.count)
    {
        result.path = string_slice(url, index, url.count);
        url = string_prefix(url, index);
    }

    result.port = 80;

    index = string_find(url, S(":"), 0, 0);
    if (index < url.count)
    {
        String port_str = string_slice(url, index + 1, url.count);
        result.port = string_to_i64(port_str, 10);
        if (result.port <= 0 || result.port > U16_MAX) {
            result.port = 0;
        }

        url = string_slice(url, 0, index);
    }

    result.host = url;

    return result;
}

function bool socket_init()
{
    static bool initted = false;
    if (initted) return true;

    #if OS_WINDOWS
    w32_socket_lib = LoadLibraryA("ws2_32.dll");
    if (w32_socket_lib == 0)
    {
        print("[net] Failed to load ws2_32.dll\n");
        return false;
    }

    W32_WSAStartup      = (WSAStartup_Func *)     GetProcAddress(w32_socket_lib, "WSAStartup");
    W32_WSACleanup      = (WSACleanup_Func *)     GetProcAddress(w32_socket_lib, "WSACleanup");
    W32_WSAGetLastError = (WSAGetLastError_Func *)GetProcAddress(w32_socket_lib, "WSAGetLastError");

    if (W32_WSAStartup == 0 || W32_WSACleanup == 0 || W32_WSAGetLastError == 0)
    {
        print("[net] Failed to load required win32 functions\n");
        return false;
    }

    WSADATA wsdata;
    if (W32_WSAStartup(0x202, &wsdata))
    {
      print("WSAStartup failed: %d", W32_WSAGetLastError());
      return false;
    }
    #endif // OS_WINDOWS

    initted = true;
    return true;
}

function bool socket_set_non_blocking_internal(i64 handle)
{
    int ret;
    #if OS_WINDOWS
    u_long nb = 1;
    ret = ioctlsocket(handle, FIONBIO, &nb);
    #else
    ret = fcntl(handle, F_SETFL, O_NONBLOCK);
    #endif

    return ret == 0;
}

function Socket socket_open(Socket_Type type)
{
    Socket result = {0};

    bool udp = type == SocketType_UDP;
    // AF_INET6
    #if OS_WINDOWS
    result.handle = WSASocket(AF_INET, udp ? SOCK_DGRAM : SOCK_STREAM, udp ? IPPROTO_UDP : IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
    #else
    result.handle = socket(AF_INET, udp ? SOCK_DGRAM : SOCK_STREAM, udp ? IPPROTO_UDP : IPPROTO_TCP);
    #endif
    result.type   = type;

    if (result.handle == INVALID_SOCKET)
    {
        print("[socket] Failed to open socket!\n");
        socket_close(&result);
        return result;
    }

    // Set non-blocking
    {
        bool success = socket_set_non_blocking_internal(result.handle);
        if (!success)
        {
            print("[socket] Failed to set socket to non-blocking!\n");
            socket_close(&result);
            return result;
        }
    }

    return result;
}

function void socket_close(Socket *socket)
{
    if (socket && socket->handle)
    {
        #if OS_WINDOWS
        closesocket(socket->handle);
        #else
        close(socket->handle);
        #endif

        socket->handle = 0;
    }
}

function bool socket_bind(Socket *socket, Socket_Address address)
{
    struct sockaddr_in addr;
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = address.host;
    addr.sin_port        = htons(address.port);

    if (bind(socket->handle, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) != 0)
    {
        return false;
    }

    return true;
}

function bool socket_listen(Socket *socket)
{
    #ifndef SOMAXCONN
    #define SOMAXCONN 10
    #endif

    if (listen(socket->handle, SOMAXCONN) != 0)
    {
        return false;
    }

    return true;
}

function bool socket_accept(Socket *socket, Socket *from_socket, Socket_Address *from_address)
{
    struct sockaddr_in from;
    int addr_len = sizeof(from);

    i64 sock = accept(socket->handle, (struct sockaddr *)&from, (socklen_t *)&addr_len);

    if (sock != INVALID_SOCKET)
    {
        socket_set_non_blocking_internal(sock);

        if (from_address != NULL)
        {
            from_address->host = from.sin_addr.s_addr;
            from_address->port = ntohs(from.sin_port);
        }

        if (from_socket != NULL)
        {
            from_socket->type   = SocketType_TCP; // @Incomplete: will this always be true?
            from_socket->handle = sock;
        }

        return true;
    }

    // @Incomplete
    return false;
}

function bool socket_connect(Socket *socket, Socket_Address address)
{
    struct sockaddr_in addr;
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = address.host;
    addr.sin_port        = htons(address.port);

    int result = connect(socket->handle, (struct sockaddr *)&addr, sizeof(addr));
    if (result == SOCKET_ERROR)
    {
        #ifdef _WIN32
            if ( WSAGetLastError() != WSAEWOULDBLOCK && WSAGetLastError() != WSAEINPROGRESS )
            {
                return false;
            }
        #else
            if ( errno != EWOULDBLOCK && errno != EINPROGRESS && errno != EAGAIN )
            {
                return false;
            }
        #endif
    }

    return true;
}

function bool socket_can_write(Socket *socket)
{
    if (!socket) return false;

    fd_set write_fds;
    FD_ZERO(&write_fds);
    FD_SET(socket->handle, &write_fds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    int result = select(0, NULL, &write_fds, NULL, &timeout);
    if (result > 0)
    {
        if (FD_ISSET(socket->handle, &write_fds))
        {
            return true;
        }
    }

    return false;
}

function bool socket_is_ready(Socket *socket)
{
    if (!socket) return false;

    fd_set write_fds;
    FD_ZERO(&write_fds);
    FD_SET(socket->handle, &write_fds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    // select returns the number of file descriptors
    int result = select(0, NULL, &write_fds, NULL, &timeout);
    return result > 0;
}

function bool socket_send(Socket *socket, Socket_Address address, String message)
{
    if (!socket) return false;

    if (socket->type == SocketType_UDP)
    {
        struct sockaddr_in addr;
        addr.sin_family      = AF_INET;
        addr.sin_addr.s_addr = address.host;
        addr.sin_port        = htons(address.port);

        int sent_bytes = sendto(socket->handle, (char *)message.data, message.count, 0, (struct sockaddr *)&addr, sizeof(addr));
        if (sent_bytes != message.count)
        {
            //return zed_net__error("Failed to send data");
            return false;
        }

        return true;
    }
    else
    {
        // @Incomplete: non-blocking TCP

        int sent_bytes = send(socket->handle, (char *)message.data, message.count, 0);
        if (sent_bytes != message.count)
        {
            //return zed_net__error("Failed to send data");
            return false;
        }

        return true;
    }
}

function i64 socket_recieve_bytes(Socket *socket, u8 *data, u64 count, Socket_Address *address)
{
    if (!socket) return false;

    if (!data || count == 0)
    {
        print("[socket] Recive must have a buffer of non-zero size!\n");
        return 0;
    }

    if (socket->type == SocketType_UDP)
    {
        struct sockaddr_in from;
        int from_size = sizeof(from);

        int bytes_received = recvfrom(
            socket->handle,
            (char *)data, count, 0, (struct sockaddr *)&from, (socklen_t *)&from_size
        );

        if (address != NULL)
        {
            address->host = from.sin_addr.s_addr;
            address->port = ntohs(from.sin_port);
        }

        return bytes_received;
    }
    else
    {
        // @Incomplete: non-blocking TCP?
        int bytes_received = recv(socket->handle, (char *)data, count, 0);
        return bytes_received;
    }
}

function bool socket_recieve(Socket *socket, String *buffer, Socket_Address *address)
{
    i64 count = socket_recieve_bytes(socket, buffer->data, buffer->count, address);
    if (count >= 0)
    {
        buffer->count = count;
    }
    return count > 0;
}

function String socket_recieve_entire_stream(Arena *arena, Socket *socket)
{
    i64 bytes_received = 0;
    i64 buffer_size = 4096;
    u8 *buffer = PushArray(arena, u8, buffer_size);
    u8 *at = buffer;

    for (;;)
    {
        i64 count = socket_recieve_bytes(socket, at, buffer_size, NULL);
        if (count <= 0) break;

        bytes_received += count;
        at += count;
        buffer_size *= 2;
        assert(arena_push(arena, buffer_size));
    }

    arena_pop(arena, buffer_size - bytes_received);
    return string_make(buffer, bytes_received);
}

function void socket_shutdown()
{
    #if OS_WINDOWS
    if (w32_socket_lib)
    {
        FreeLibrary(w32_socket_lib);
        w32_socket_lib = 0;
    }

    W32_WSACleanup();
    #endif // OS_WINDOWS
}

function String socket_get_address_name(Socket_Address address)
{
    char ip4[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address.host, ip4, INET_ADDRSTRLEN);

    /*
    char ip6[INET6_ADDRSTRLEN];
    u64 host_ipv6[2];
    inet_ntop(AF_INET6, &host_ipv6, ip6, INET6_ADDRSTRLEN);
    */

    return string_from_cstr(ip4);
}

function bool socket_is_valid(Socket socket)
{
    // @Incomplete @Robustness: can valid socket handles be 0?
    return socket.handle != INVALID_SOCKET && socket.handle != 0;
}

function Socket_Address socket_make_address_from_url(String url)
{
    Http_Url_Parts parts = socket_parse_url(url);
    return socket_make_address(string_to_cstr(temp_arena(), parts.host), parts.port);
}

function Socket socket_create_udp_server(Socket_Address address)
{
    socket_init();

    Socket result = socket_open(SocketType_UDP);

    if (!socket_bind(&result, address))
    {
        String url = socket_get_address_name(address);
        print("[socket] Failed to bind to address: %.*s\n", LIT(url));
        socket_close(&result);
        return result;
    }

    return result;
}

function Socket socket_create_udp_client(Socket_Address address)
{
    socket_init();

    Socket result = socket_open(SocketType_UDP);

    //socket_bind(&result, socket_make_address(0, 0));

    if (!socket_connect(&result, address))
    {
        String url = socket_get_address_name(address);
        print("[socket] Failed to connect to server address: %.*s\n", LIT(url));
        socket_close(&result);
        return result;
    }

    return result;
}

function Http http_request(String verb, String url, String headers, String data)
{
    socket_init();

    Http result = {0};

    Http_Url_Parts parts = socket_parse_url(url);
    if (!parts.path.count)
    {
        parts.path = S("/");
    }

    if (parts.protocol.count > 0 && !string_equals(parts.protocol, S("http")))
    {
        print("[http] Protocol not supported: %.*s\n", LIT(parts.protocol));
        result.status = HttpStatus_Failed;
        return result;
    }

    if (parts.port == 0)
    {
        print("[http] Invalid port\n");
        result.status = HttpStatus_Failed;
        return result;
    }

    Socket_Address address = socket_make_address(string_to_cstr(temp_arena(), parts.host), parts.port);

    result.socket = socket_open(SocketType_TCP);
    if (!socket_connect(&result.socket, address))
    {
        print("[http] Failed to connect to TCP socket\n");
        socket_close(&result.socket);
        result.status = HttpStatus_Failed;
        return result;
    }

    assert(string_starts_with(parts.path, S("/")));

    M_Temp scratch = GetScratch(0, 0);

    String_Array chunks = {0};
    chunks.data = PushArray(scratch.arena, String, 16);
    chunks.count = 0;

    String request_data = string_print(scratch.arena, "%.*s %.*s HTTP/1.0\r\nHost: %.*s:%d\r\n", LIT(verb), LIT(parts.path), LIT(parts.host), parts.port);
    chunks.data[chunks.count] = request_data;
    chunks.count += 1;

    if (data.count)
    {
        String content_header = string_print(scratch.arena, "Content-Length: %d", data.count);
        chunks.data[chunks.count] = content_header;
        chunks.count += 1;
    }

    if (headers.count)
    {
        chunks.data[chunks.count] = headers;
        chunks.count += 1;
    }

    chunks.data[chunks.count] = string_print(scratch.arena, "\r\n\r\n");
    request_data.count += 1;

    if (data.count)
    {
        chunks.data[chunks.count] = data;
        chunks.count += 1;
    }

    String request = string_concat_array(scratch.arena, chunks.data, chunks.count);

    result.request_data = string_alloc(request);
    result.status       = HttpStatus_Pending;

    ReleaseScratch(scratch);

    return result;
}

function Http http_get(String url)
{
    return http_request(S("GET"), url, S(""), S(""));
}

function Http http_post(String url, String data)
{
    return http_request(S("POST"), url, S(""), data);
}

function Http http_put(String url, String data)
{
    return http_request(S("PUT"), url, S(""), data);
}

function Http http_delete(String url)
{
    return http_request(S("DELETE"), url, S(""), S(""));
}

function Http_Header_Array http_parse_headers(Arena *arena, String headers)
{
    Http_Header_Array result = {0};

    result.data = PushArray(arena, Http_Header, 1024);
    result.count = 0;
    result.capacity = 1024;

    i64 prev_index = 0;
    i64 index = string_find(headers, S("\r\n"), 0, 0);

    while (index < headers.count)
    {
        String line = string_slice(headers, prev_index, index);
        i64 i = string_find(line, S(":"), 0, 0);
        if (i < line.count)
        {
            assert(result.count < result.capacity);
            Http_Header *it = &result.data[result.count];
            result.count += 1;
            it->key   = string_slice(line, 0, i);
            it->value = string_trim_whitespace(string_slice(line, i + 1, line.count));
        }

        prev_index = index + S("\r\n").count;
        index = string_find(headers, S("\r\n"), prev_index + 1, 0);
    }

    String line = string_slice(headers, prev_index, index);
    i64 i = string_find(line, S(":"), 0, 0);
    if (i < line.count)
    {
        assert(result.count < result.capacity);
        Http_Header *it = &result.data[result.count];
        result.count += 1;
        it->key   = string_slice(line, 0, i);
        it->value = string_trim_whitespace(string_slice(line, i + 1, line.count));
    }

    // @Memory: release over-allocated count

    return result;
}

void http_process(Http *http)
{
    if (http->status == HttpStatus_Failed || http->status == HttpStatus_Completed)
    {
        return;
    }

    if (!http->is_ready)
    {
        if (socket_is_ready(&http->socket))
        {
            http->is_ready = true;
        }
    }

    if (!http->is_ready) return;

    if (!http->request_sent)
    {
        if (!socket_send(&http->socket, {}, http->request_data))
        {
            http->status = HttpStatus_Failed;
            return;
        }

        string_free(&http->request_data);
        http->request_sent = true;
        return;
    }

    // NOTE(nick): it seems like having this structure created _before_ the while loop
    // is actually critical to returning all the data needed from the response!
    // Otherwise, we were only getting parts of it sometimes, so we couldn't just return it all in one go
    fd_set sockets_to_check; 
    FD_ZERO(&sockets_to_check);
    FD_SET(http->socket.handle, &sockets_to_check);
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    while (select(0, &sockets_to_check, NULL, NULL, &timeout) > 0)
    {
        Arena *arena = temp_arena();

        i64 bytes_received = 0;
        i64 buffer_size = 4096;
        u8 *buffer = PushArray(arena, u8, buffer_size);
        u8 *at = buffer;

        for (;;)
        {
            i64 count = socket_recieve_bytes(&http->socket, at, buffer_size, NULL);
            if (count <= 0) break;

            bytes_received += count;
            at += count;
            buffer_size *= 2;
            assert(arena_push(arena, buffer_size));
        }

        arena_pop(arena, buffer_size - bytes_received);

        String response_data = string_make(buffer, bytes_received);

        i64 index = string_find(response_data, S("\r\n"), 0, 0);
        if (index < response_data.count)
        {
            String first = string_slice(response_data, 0, index);
            String_Array first_parts = string_array_from_list(temp_arena(), string_split(temp_arena(), first, S(" ")));

            i64 status_code = string_to_i64(first_parts.data[1], 10);
            if (!(status_code > 0 && status_code < 1000))
            {
                status_code = 0;
            }
            http->status_code = status_code;

            i64 header_end_index = string_find(response_data, S("\r\n\r\n"), index + S("\r\n").count, 0);
            if (header_end_index < response_data.count)
            {
                http->response_headers = string_slice(response_data, index + S("\r\n").count, header_end_index);
                http->response_body = string_slice(response_data, header_end_index + S("\r\n\r\n").count, response_data.count);
            }
        }

        if (http->response_headers.count)
        {
            i64 content_type_index = string_find(http->response_headers, S("Content-Type:"), 0, MatchFlag_IgnoreCase);
            content_type_index += S("Content-Type:").count;
            i64 content_type_end_index = string_find(http->response_headers, S("\r\n"), content_type_index, 0);

            if (
                content_type_index < http->response_headers.count &&
                content_type_end_index < http->response_headers.count
            )
            {
                http->content_type = string_trim_whitespace(string_slice(http->response_headers, content_type_index, content_type_end_index));
            }
        }

        http->status = HttpStatus_Completed;
        http->response_data = response_data;

        socket_close(&http->socket);
        break;
    }
}


function void http_init(Http_Manager *manager)
{
    manager->arena = arena_alloc(Gigabytes(1));
}

function void http_add(Http_Manager *manager, Http request)
{
    // @Incomplete!
    //array_push(&manager->requests, request);
}

function void http_update(Http_Manager *manager)
{
    // @Incomplete!
    /*
    For_Index (manager->requests)
    {
        auto it = &manager->requests[index];
        if (it->status != HttpStatus_Pending)
        {
            array_remove_ordered(&manager->requests, index);
            index --;
        }
    }

    Forp (manager->requests)
    {
        http_process(it);
    }
    */
}


function Http_Request http_parse_request(String request)
{
    Http_Request result = {0};

    i64 index;

    index = string_find(request, S("\r\n"), 0, 0);
    if (index < request.count)
    {
        request = string_slice(request, 0, index);
    }

    index = string_find(request, S(" "), 0, 0);
    if (index < request.count)
    {
        result.method = string_slice(request, 0, index);

        i64 index2 = string_find(request, S(" "), index + 1, 0);
        if (index2 < request.count)
        {
            result.url = string_slice(request, index + 1, index2);
        }
    }

    return result;
}

function Socket socket_create_tcp_server(Socket_Address address)
{
    Socket result = socket_open(SocketType_TCP);

    if (!socket_bind(&result, address))
    {
        String url = socket_get_address_name(address);
        print("[socket] Failed to bind to address: %.*s:%d\n", LIT(url), address.port);
        socket_close(&result);
        return result;
    }

    if (!socket_listen(&result))
    {
        print("[socket] Failed to listen\n");
        socket_close(&result);
        return result;
    }

    return result;
}

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Status
function String http_status_name_from_code(i32 status_code)
{
    String result = {0};
    switch (status_code)
    {
        // 100s
        case 100: { result = S("Continue"); } break;
        case 101: { result = S("Switching Protocols"); } break;

        // 200s
        case 200: { result = S("OK"); } break;
        case 201: { result = S("Created"); } break;
        case 202: { result = S("Accepted"); } break;
        case 203: { result = S("Non-Authoritative Information"); } break;
        case 204: { result = S("No Content"); } break;
        case 205: { result = S("Reset Content"); } break;
        case 206: { result = S("Partial Content"); } break;

        // 300s
        case 300: { result = S("Multiple Choices"); } break;
        case 301: { result = S("Moved Permanently"); } break;
        case 302: { result = S("Found"); } break;
        case 303: { result = S("See Other"); } break;
        case 304: { result = S("Not Modified"); } break;
        case 305: { result = S("Use Proxy"); } break;

        case 307: { result = S("Temporary Redirect"); } break;
        case 308: { result = S("Permanent Redirect"); } break;

        // 400s
        case 400: { result = S("Bad Request"); } break;
        case 401: { result = S("Unauthorized"); } break;
        case 402: { result = S("Payment Required"); } break;
        case 403: { result = S("Forbidden"); } break;
        case 404: { result = S("Not Found"); } break;
        case 405: { result = S("Method Not Allowed"); } break;
        case 406: { result = S("Not Acceptable"); } break;
        case 407: { result = S("Proxy Authentication Required"); } break;
        case 408: { result = S("Request Timeout"); } break;
        case 409: { result = S("Conflict"); } break;
        case 410: { result = S("Gone"); } break;
        case 411: { result = S("Length Required"); } break;
        case 412: { result = S("Precondition Failed"); } break;
        case 413: { result = S("Payload Too Large"); } break;
        case 414: { result = S("URI Too Long"); } break;
        case 415: { result = S("Unsupported Media Type"); } break;

        case 418: { result = S("I'm a teapot"); } break;

        case 429: { result = S("Too Many Requests"); } break;
        case 451: { result = S("Unavailable For Legal Reasons"); } break;

        // 500s
        case 500: { result = S("Internal Server Error"); } break;
        case 501: { result = S("Not Implemented"); } break;
        case 502: { result = S("Bad Gateway"); } break;
        case 503: { result = S("Service Unavailable"); } break;
        case 504: { result = S("Gateway Timeout"); } break;
        case 505: { result = S("HTTP Version Not Supported"); } break;

        default: break;
    }
    return result;
}

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types
function String http_content_type_from_extension(String ext)
{
    String result = {0};

    string_to_lower(&ext);

    if (false) {}
    else if (string_equals(ext, S(".html"))) { result = S("text/html"); }
    else if (string_equals(ext, S(".css")))  { result = S("text/css"); }
    else if (string_equals(ext, S(".js")))   { result = S("text/javascript"); }
    else if (string_equals(ext, S(".json"))) { result = S("application/json"); }
    else if (string_equals(ext, S(".txt")))  { result = S("text/plain"); }
    else if (string_equals(ext, S(".md")))   { result = S("text/plain"); }
    else if (string_equals(ext, S(".bmp")))  { result = S("image/bmp"); }
    else if (string_equals(ext, S(".gif")))  { result = S("image/gif"); }
    else if (string_equals(ext, S(".png")))  { result = S("image/png"); }
    else if (string_equals(ext, S(".jpg")))  { result = S("image/jpeg"); }
    else if (string_equals(ext, S(".jpeg"))) { result = S("image/jpeg"); }
    else if (string_equals(ext, S(".jpe")))  { result = S("image/jpeg"); }
    else if (string_equals(ext, S(".ico")))  { result = S("image/x-icon"); }
    else if (string_equals(ext, S(".svg")))  { result = S("image/svg+xml"); }
    else if (string_equals(ext, S(".mp3")))  { result = S("audio/mpeg"); }
    else if (string_equals(ext, S(".mp4")))  { result = S("audio/mp4"); }
    else if (string_equals(ext, S(".wav")))  { result = S("audio/x-wav"); }
    else if (string_equals(ext, S(".bin")))  { result = S("application/octet-stream"); }
    else if (string_equals(ext, S(".exe")))  { result = S("application/octet-stream"); }
    else if (string_equals(ext, S(".pdf")))  { result = S("application/pdf"); }
    
    return result;
}

typedef struct Http_Server Http_Server;
struct Http_Server
{
    Socket socket;
};

#define HTTP_REQUEST_CALLBACK(name) void name(Http_Request *request, Http_Response *response)
typedef HTTP_REQUEST_CALLBACK(Http_Request_Callback);

typedef struct Http_Thread_Params Http_Thread_Params;
struct Http_Thread_Params
{
    Socket client;
    Socket_Address client_address;
    Http_Request_Callback *request_handler;
};

THREAD_PROC(http_responder_thread)
{
    Http_Thread_Params *params = (Http_Thread_Params *)data;
    Socket *client = &params->client;

    String raw_request = socket_recieve_entire_stream(temp_arena(), client);
    Http_Request request = http_parse_request(raw_request);
    request.address = params->client_address;

    Http_Response response = {0};
    params->request_handler(&request, &response);

    if (!response.status_code) response.status_code = (response.body.count > 0) ? 200 : 500;
    if (!response.content_type.count) response.content_type = S("text/plain");

    
    M_Temp scratch = GetScratch(0, 0);

    String_Array chunks = {0};
    chunks.data = PushArray(scratch.arena, String, 2048);
    chunks.count = 0;

    String status_name = http_status_name_from_code(response.status_code);
    String response_data = string_print(scratch.arena, "HTTP/1.0 %d %.*s\r\n", response.status_code, LIT(status_name));
    chunks.data[chunks.count] = response_data;
    chunks.count += 1;

    if (response.content_type.count)
    {
        chunks.data[chunks.count] = string_print(scratch.arena, "Content-Type: %.*s\r\n", LIT(response.content_type));
        chunks.count += 1;
    }

    if (response.body.count)
    {
        chunks.data[chunks.count] = string_print(scratch.arena, "Content-Length: %d\r\n", response.body.count);
        chunks.count += 1;
    }

    if (response.headers.count > 0)
    {
        for (int index = 0; index < response.headers.count; index += 1)
        {
            Http_Header it = response.headers.data[index];

            assert(chunks.count < 2048);
            chunks.data[chunks.count] = string_print(scratch.arena, "%.*s: %.*s\r\n", LIT(it.key), LIT(it.value));
            chunks.count += 1;
        }
    }

    assert(chunks.count < 2048);
    chunks.data[chunks.count] = S("\r\n");
    chunks.count += 1;

    String response_payload = string_concat_array(scratch.arena, chunks.data, chunks.count);
    socket_send(client, {}, response_payload);

    if (response.body.count)
    {
        socket_send(client, {}, response.body);
    }

    socket_close(client);
    return 0;
}

function Http_Server http_server_init(String server_url)
{
    Http_Server result = {0};
    Socket_Address address = socket_make_address_from_url(server_url);
    result.socket = socket_create_tcp_server(address);
    return result;
}

function bool http_server_poll(Http_Server *server, Socket *client, Socket_Address *client_address)
{
    return socket_accept(&server->socket, client, client_address);
}

function void http_server_tick(Http_Server *server, Http_Request_Callback request_handler)
{
    Socket client;
    Socket_Address client_address;
    while (http_server_poll(server, &client, &client_address))
    {
        Http_Thread_Params params = {0};
        params.client = client;
        params.client_address = client_address;
        params.request_handler = request_handler;

        // TODO(nick): we should spin up a bunch of threads ahead of time
        Thread thread = thread_create(http_responder_thread, &params, sizeof(Http_Thread_Params));
        thread_detach(thread);
    }
}

function void http_server_run(String server_url, Http_Request_Callback request_handler)
{
    socket_init();

    Http_Server server = http_server_init(server_url);
    if (!socket_is_valid(server.socket))
    {
        print("Failed to start HTTP server at %.*s\n", LIT(server_url));
        return;
    }

    while (1)
    {
        arena_reset(temp_arena());
        http_server_tick(&server, request_handler);
        os_sleep(1);
    }
}

#endif // impl