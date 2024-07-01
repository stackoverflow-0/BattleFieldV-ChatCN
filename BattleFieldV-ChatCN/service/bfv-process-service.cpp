#include "bfv-process-service.hpp"
#include <iostream>
#include <tlhelp32.h>

namespace svc
{
    auto Bfv_Process_Service::request_stop() noexcept -> void
    {

    }

    auto Bfv_Process_Service::stop_requested() noexcept -> bool
    {
    }

    auto Bfv_Process_Service::run() noexcept -> void
    {
        auto init = [&]() -> bool {
            auto init_success{true};
            bfv_window = FindWindow(nullptr, L"Battlefield™ V");
            if (bfv_window != 0) {
                if (!GetWindowThreadProcessId(bfv_window, &bfv_pid)) {
                    std::cout << "[FAILED] 获取游戏线程失败\n";
                    init_success = false;
                }
                auto get_base_addr = [](DWORD bfv_pid, auto name) -> BYTE_PTR {
                    auto base_addr = BYTE_PTR{};
                    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, bfv_pid);
                    if (snapshot != INVALID_HANDLE_VALUE) {
                        auto entry = MODULEENTRY32{};
                        entry.dwSize = sizeof(entry);
                        if (Module32First(snapshot, &entry)) {
                            do {
                                if (_wcsicmp(entry.szModule, name) == 0) {
                                    base_addr = entry.modBaseAddr;
                                    break;
                                }
                            } while (Module32Next(snapshot, &entry));
                        }
                    }
                    CloseHandle(snapshot);
                    return base_addr;
                };
                bfv_base_addr = get_base_addr(bfv_pid, L"bf5.exe");
            }
        };

        auto loop = [&]() -> void {

        };

        while (!stop_requested()) {
            while (!stop_requested()) {
                if (init()) {
                    break;
                }
                Sleep(100);
            }

            bfv_process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, bfv_pid);
            bfv_msg_buffer_addr = BYTE_PTR(VirtualAllocEx(bfv_process, nullptr, 256 * 4, MEM_COMMIT, PAGE_READWRITE));

            loop();

            VirtualFreeEx(bfv_process, LPVOID(bfv_msg_buffer_addr), 0, MEM_RELEASE);
            CloseHandle(bfv_process);
        }
        
    }
} // namespace svc
