#include "ra3.h"
#include "menu.h"

decltype(ra3::main::vars) ra3::main::vars;
decltype(ra3::main::logger) ra3::main::logger(__$LoggerPath__);
ra3::main::fnPresent_t ra3::main::oPresent, ra3::main::mPresent;

#define init_vars(A, B) if (!vars.A) vars.A = (size_t)GetModuleHandleA(B)

DWORD RA3XAPI ra3::main::engine(LPVOID)
{
	while (!initvars()) Sleep(1000);

	logger.log_hex(__$GameModuleName__, vars.game);
	logger.log_hex(__$D3D9ModuleName__, vars.d3d9);

	std::uintptr_t* vftable = []() -> std::uintptr_t* {
		// C7 06 ? ? ? ? ? ? ? ? 89 86 ? ? ? ? ? ? ? ? 89 86
		for (size_t i = 0; i < 0x7FFFFFFF; ++i)
		{
			BYTE* ins = (BYTE*)(vars.d3d9 + i);
			if (*(ins + 0x0) == 0xC7
				&& *(ins + 0x1) == 0x06
				&& *(ins + 0x6) == 0x89
				&& *(ins + 0x7) == 0x86
				&& *(ins + 0xC) == 0x89
				&& *(ins + 0xD) == 0x86)
				return *(std::uintptr_t**)(ins + 2);
		}
		return NULL;
	}();

	logger.log_hex("Direct3DDevice9::vftable", (std::uintptr_t)vftable);
	logger.log("Start hook to IDirect3DDevice9::Present");

	if (vftable)
	{
		BYTE* present = (BYTE*)vftable[17];

		auto make_jmp = [](BYTE* src, BYTE* dst, UINT len)
		{
			DWORD old;
			VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &old);
			memset(src, 0x90, len);
			*src = 0xE9;
			*(DWORD*)(src + 1) = (DWORD)dst - (DWORD)src - 5;
			VirtualProtect(src, len, old, &old);
		};

		BYTE* buffer = (BYTE*)VirtualAlloc(NULL, 10, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		DWORD oldProtect;
		if (buffer && VirtualProtect(present, 5, PAGE_EXECUTE_READWRITE, &oldProtect))
		{
			ra3::render_t::assign(mPresent);
			memcpy(buffer, present, 5);
			make_jmp(buffer + 5, (BYTE*)((DWORD)present + 5), 5);
			make_jmp(present, (BYTE*)mPresent, 5);
			VirtualProtect(present, 5, oldProtect, &oldProtect);
			VirtualProtect(buffer, 10, PAGE_EXECUTE_READ, &oldProtect);
			oPresent = (fnPresent_t)buffer;
		}
		else logger.log("Error: Alloc or Protect failed");
	}
	else logger.log("Error: Can't find vftable");

	for (; process(); Sleep(10));

	FreeLibrary(GetModuleHandleA(NULL));

	return NULL;
}

BOOL RA3XAPI ra3::main::process()
{
	return TRUE;
}

BOOL RA3XAPI ra3::main::initvars()
{
	init_vars(game, __$GameModuleName__);
	init_vars(d3d9, __$D3D9ModuleName__);
	return vars.game && vars.d3d9;
}