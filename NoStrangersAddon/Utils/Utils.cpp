#include "Utils.h"
#include <Psapi.h>

void* SignatureScan(std::string signature, uintptr_t handle)
{
	static const auto patternToByte = [](const char* pattern) noexcept -> std::vector<int>
	{
		auto bytes = std::vector<int>{ };
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + std::strlen(pattern);

		for (auto current = start; current < end; ++current)
		{
			if (*current == '?')
			{
				++current;

				if (*current == '?')
					++current;

				bytes.push_back(-1);
			}
			else
				bytes.push_back(std::strtoul(current, &current, 16));

		}

		return bytes;
	};

	if (!handle)
		handle = baseAddr;
	if (handle == 0)
		return nullptr;

	IMAGE_DOS_HEADER dosHeader = *(IMAGE_DOS_HEADER*)(handle);
	//WTA::Driver.readVirtualMemory((uintptr_t)handle, &dosHeader, sizeof(IMAGE_DOS_HEADER));
	IMAGE_NT_HEADERS ntHeaders = *(IMAGE_NT_HEADERS*)(handle + dosHeader.e_lfanew);
	//WTA::Driver.readVirtualMemory((uintptr_t)handle + (uintptr_t)dosHeader.e_lfanew, &ntHeaders, sizeof(IMAGE_NT_HEADERS));
	auto size = ntHeaders.OptionalHeader.SizeOfCode;
	auto bytes = patternToByte(signature.c_str());
	auto scanBytes = reinterpret_cast<std::uint8_t*>(handle);

	auto s = bytes.size();
	auto d = bytes.data();

	for (auto i = 0ul; i < size - s; ++i)
	{
		bool found = true;

		for (auto j = 0ul; j < s; ++j)
		{
			BYTE buffer = *(BYTE*)((uintptr_t)handle + i + j);
			//WTA::Driver.readVirtualMemory((uintptr_t)handle + i + j, &buffer, 1);
			if (buffer != d[j] && d[j] != -1)
			{
				found = false;
				break;
			}
		}

		if (found) {
			return &scanBytes[i];
		}

	}
	return nullptr;
}

std::mutex fileWrite;

void WriteToFile(std::string FileName, std::string text) {
	fileWrite.lock();
	std::ofstream myfile;
	myfile.open(FileName, std::ios_base::app);
	myfile << text << "\n";
	myfile.close();
	fileWrite.unlock();
}