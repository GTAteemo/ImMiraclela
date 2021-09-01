#pragma once
#include "memory.hpp"

namespace driver_control
{


	ULONG64 get_module_base_address(const char* module_name, ULONG process_id);




	bool draw_box(int x, int y, int w, int h, int t, int r, int g, int b);
	
	bool draw_text(int x, int y, const char* text);

	uint32_t readbuffer(uint64_t address, PVOID buffer, size_t size, ULONG process_id);

	uint64_t ReadChain(uint64_t read_address, const std::vector<uint64_t>& offsets, ULONG process_id);


	template <class T>
	T Read(UINT_PTR read_address, ULONG process_id);

	template <class T>
	T Read(UINT_PTR read_address, ULONG process_id)
	{
		T response{};
		INFO_STRUCT instructions;
		instructions.pid = process_id;
		instructions.size = sizeof(T);
		instructions.address = read_address;
		instructions.read = TRUE;
		instructions.write = FALSE;
		instructions.req_base = FALSE;
		instructions.draw_box = FALSE;
		instructions.output = &response;
		hook::call_hook(&instructions);

		return response;
	}


	template<typename S>
	bool write_memory(ULONG write_address, ULONG source_address, S value, ULONG process_id);


	template<typename S>
	bool write_memory(ULONG write_address, ULONG source_address, S value, ULONG process_id)
	{
		INFO_STRUCT instructions;
		instructions.address = write_address;
		instructions.pid = process_id;
		instructions.write = TRUE;
		instructions.read = FALSE;
		instructions.req_base = FALSE;
		instructions.draw_box = FALSE;
		instructions.buffer_address = (void*)source_address;
		instructions.size = sizeof(S);

		hook::call_hook(&instructions);

		return true;
	}
}

