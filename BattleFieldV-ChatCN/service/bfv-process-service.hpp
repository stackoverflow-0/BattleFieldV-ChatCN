#pragma once
#include <windows.h>
#include <thread>

namespace svc
{
    using BYTE_PTR = BYTE*;

    struct Bfv_Process_Service final
    {
        Bfv_Process_Service() {}
        ~Bfv_Process_Service() {}

        auto request_stop() noexcept -> void;
        auto stop_requested() noexcept -> bool;
        auto run() noexcept -> void;
    protected:
        DWORD bfv_pid{-1};
        HWND bfv_window{};
        HANDLE bfv_process{};
        BYTE_PTR bfv_base_addr{};
        BYTE_PTR bfv_msg_buffer_addr{};
    };
} // namespace svc
