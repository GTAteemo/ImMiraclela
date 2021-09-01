#include "driver_control.hpp"


bool driver_control::draw_box(int x, int y, int w, int h, int t, int r, int g, int b)
{
	INFO_STRUCT instructions;
	instructions.write = FALSE;
	instructions.read = FALSE;
	instructions.req_base = FALSE;
	instructions.draw_box = TRUE;

	instructions.x = x;
	instructions.y = y;
	instructions.w = w;
	instructions.h = h;
	instructions.t = t;

	instructions.r = r;
	instructions.g = g;
	instructions.b = b;

	hook::call_hook(&instructions);

	return true;
}

ULONG64 driver_control::get_module_base_address(const char* module_name, ULONG process_id)
{
	INFO_STRUCT instructions = { 0 };
	instructions.pid = process_id;
	instructions.req_base = TRUE;
	instructions.read = FALSE;
	instructions.write = FALSE;
	instructions.draw_box = FALSE;
	instructions.module_name = module_name;
	hook::call_hook(&instructions);

	ULONG64 base = NULL;
	base = instructions.base_address;
	return base;
}





uint64_t driver_control::ReadChain(uint64_t read_address, const std::vector<uint64_t>& offsets, ULONG process_id)
{
	uint64_t result = Read<uint64_t>(read_address + offsets.at(0), process_id);
	for (int i = 1; i < offsets.size(); i++) {
		result = Read<uint64_t>(result + offsets.at(i), process_id);
	}
	return result;
}


bool driver_control::draw_text(int x, int y, const char* text)
{
	INFO_STRUCT info = { 0 };
	info.draw_text = true;
	info.x = x;
	info.y = y;
	info.text = text;

	hook::call_hook(&info);

	return true;
}

static uint32_t copy_memory(
	const uintptr_t src_address,
	const uint32_t	process_id,
	const uintptr_t	dest_address,
	const size_t	size)
{
	INFO_STRUCT instructions;
	instructions.pid = process_id;
	instructions.size = uint64_t(size);
	instructions.address = uint64_t(src_address);
	instructions.read = TRUE;
	instructions.write = FALSE;
	instructions.req_base = FALSE;
	instructions.draw_box = FALSE;
	instructions.output = &uint64_t(dest_address);
	hook::call_hook(&instructions);

	return uint32_t(dest_address);
}

uint32_t driver_control::readbuffer(uint64_t address, PVOID buffer, size_t size, ULONG process_id)
{
	if (address == 0)
		return false;
	return copy_memory(address, process_id, uintptr_t(buffer), size);
}