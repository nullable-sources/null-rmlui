#include <Windows.h>
#include <Psapi.h>
#include <tlhelp32.h>

#include <null-sdk.h>
#include "features.h"

namespace features {
	void inject(const std::uint32_t& pid, const std::string& dll_path) {
		HANDLE handle{ OpenProcess(PROCESS_ALL_ACCESS, false, pid) };
		void* dll_path_address_in_process{ VirtualAllocEx(handle, 0, strlen(dll_path.data()), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE) };
		if(!dll_path_address_in_process) return;

		if(!WriteProcessMemory(handle, dll_path_address_in_process, dll_path.data(), strlen(dll_path.data()), nullptr)) return;

		HMODULE kernel_base{ GetModuleHandleA("kernel32.dll") };
		if(!kernel_base) return;

		void* load_library_address{ GetProcAddress(kernel_base, "LoadLibraryA") };
		if(!load_library_address) return;

		HANDLE remote_thread{ CreateRemoteThread(handle, nullptr, 0, (LPTHREAD_START_ROUTINE)load_library_address, dll_path_address_in_process, 0, nullptr) };
		if(!remote_thread) return;

		WaitForSingleObject(remote_thread, INFINITE);
		VirtualFreeEx(handle, dll_path_address_in_process, 0, MEM_RELEASE);
		CloseHandle(handle);
	}

	std::vector<process_info_t> get_processes_list() {
        HANDLE snapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };
        PROCESSENTRY32 entry{ { sizeof(PROCESSENTRY32) } };
        if(!Process32First(snapshot, &entry)) return { };

        std::vector<process_info_t> list;
        while(Process32Next(snapshot, &entry)) {
            process_info_t process{ utils::encoding::utf8_to_ascii(entry.szExeFile), entry.th32ProcessID };
            if(!OpenProcess(PROCESS_ALL_ACCESS, false, entry.th32ProcessID)) continue;
            list.push_back(process);
        }

        CloseHandle(snapshot);
        return list;
	}

    std::string select_dll() {
		WCHAR buffer[260]{ '\0' };
		OPENFILENAME opf{
			.lStructSize{ sizeof(OPENFILENAME) },
			.hwndOwner{ NULL },
			.lpstrFilter{ L".dll\0*.dll" },
			.nFilterIndex{ 1 },
			.lpstrFile{ buffer },
			.nMaxFile{ sizeof(buffer) },
			.lpstrFileTitle{ NULL },
			.nMaxFileTitle{ 0 },
			.lpstrInitialDir{ NULL },
			.Flags{ OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST }
		};

		if(GetOpenFileName(&opf))
			return utils::encoding::utf8_to_ascii(opf.lpstrFile);
		return { };
    }
}