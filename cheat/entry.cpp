#include "includes.h"

UCHAR
szFileSys[255],
szVolNameBuff[255];

DWORD
dwMFL,
dwSysFlags,
dwSerial;


int __stdcall DllMain(HMODULE self, ulong_t reason, void* reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
#ifndef KOLO

		HANDLE thread = CreateThread(nullptr, 0, Core::init, nullptr, 0, nullptr);
		if (!thread)
			return 0;

		// CloseHandle( thread );
#else
		Core::init(nullptr);
#endif
		return 1;
	}

	return 0;
}

