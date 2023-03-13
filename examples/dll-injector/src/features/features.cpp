#include <Windows.h>
#include <Psapi.h>
#include <tlhelp32.h>

#include <null-sdk.h>
#include "features.h"

#include <Windows.h>
#include <Dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

//@credits: https://stackoverflow.com/a/14562060
bool get_image_file_headers(std::wstring file_name, IMAGE_NT_HEADERS& headers) {
	HANDLE file_handle{ CreateFile(file_name.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0) };
	if(file_handle == INVALID_HANDLE_VALUE)
		return false;

	HANDLE image_handle{ CreateFileMapping(file_handle, nullptr, PAGE_READONLY, 0, 0, nullptr) };
	if(image_handle == 0) {
		CloseHandle(file_handle);
		return false;
	}

	void* image_ptr{ MapViewOfFile(image_handle, FILE_MAP_READ, 0, 0, 0) };
	if(image_ptr == nullptr) {
		CloseHandle(image_handle);
		CloseHandle(file_handle);
		return false;
	}

	if(PIMAGE_NT_HEADERS headers_ptr{ ImageNtHeader(image_ptr) }; headers_ptr != nullptr) {
		headers = *headers_ptr;
	} else {
		UnmapViewOfFile(image_ptr);
		CloseHandle(image_handle);
		CloseHandle(file_handle);
		return false;
	}

	UnmapViewOfFile(image_ptr);
	CloseHandle(image_handle);
	CloseHandle(file_handle);

	return true;
}

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

		static const auto enum_windows{ [](HWND hwnd, LPARAM pid) -> BOOL {
			DWORD hwnd_pid{ };
			GetWindowThreadProcessId(hwnd, &hwnd_pid);

			return *(std::uint32_t*)pid != hwnd_pid;
		} };

        std::vector<process_info_t> list;
        while(Process32Next(snapshot, &entry)) {
            process_info_t process{ utils::encoding::utf8_to_ascii(entry.szExeFile), entry.th32ProcessID };
			HANDLE process_handle{ OpenProcess(PROCESS_ALL_ACCESS, false, entry.th32ProcessID) };
			if(!process_handle)
				continue;

			if(!EnumWindows(enum_windows, (LPARAM)&process.pid)) process.type = "application";
			else process.type = "background";

			SYSTEM_INFO system_info{ };
			GetNativeSystemInfo(&system_info);
			if(system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
				process.architecture = "x86";
			} else {
				BOOL result{ };
				IsWow64Process(process_handle, &result);
				process.architecture = (bool)!result ? "x64" : "x86";
			}

			list.push_back(process);
        }

        CloseHandle(snapshot);
        return list;
	}

	dll_info_t select_dll() {
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

		if(GetOpenFileName(&opf)) {
			dll_info_t dll_info{ utils::encoding::utf8_to_ascii(opf.lpstrFile) };

			IMAGE_NT_HEADERS headers{ };
			if(!get_image_file_headers(opf.lpstrFile, headers))
				utils::logger.log(utils::e_log_type::error, "get_image_file_headers return false.");

			switch(headers.FileHeader.Machine) {
				case IMAGE_FILE_MACHINE_I386: { dll_info.architecture = "x86"; } break;
				case IMAGE_FILE_MACHINE_AMD64: { dll_info.architecture = "x64"; } break;
				default: { dll_info.architecture = "unk"; } break;
			}

			return dll_info;
		}
		return { };
    }
}