#include <Windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <stdio.h>

#define MSG_SIZE 256

char msg[MSG_SIZE];
PDH_HQUERY hquery;
PDH_HCOUNTER hcountercpu;
PDH_HCOUNTER hcounterram;

#pragma comment(lib, "pdh.lib")

extern "C"
{
	BOOL WINAPI init();

	BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
	{
		BOOL res = TRUE;
		switch (fdwReason)
		{
		case DLL_PROCESS_ATTACH:
			res = init();
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
		}

		return res;
	}

	BOOL WINAPI init()
	{
		if (hquery)
			return TRUE;

		FILE* log = fopen("log.txt", "w");
		PDH_STATUS status;

		if((status=PdhOpenQuery(NULL, 0, &hquery)) != ERROR_SUCCESS){
			fprintf(log, "PdhOpenQuery %lx\n", status);
			return FALSE;
		}

		if((status=PdhAddEnglishCounter(hquery,
			TEXT("\\Processor(_Total)\\% Processor Time"),
			0,
			&hcountercpu)) != ERROR_SUCCESS){
				fprintf(log, "PdhAddCounter (cpu) %lx\n", status);
				return FALSE;
		}
		if((status=PdhAddEnglishCounter(hquery,
			TEXT("\\Memory\\Available Bytes"),
			0,
			&hcounterram)) != ERROR_SUCCESS){
				fprintf(log, "PdhAddCounter (ram) %lx\n", status);
				return FALSE;
		}

		if((status=PdhCollectQueryData(hquery)) != ERROR_SUCCESS){
			fprintf(stderr, "PdhCollectQueryData %lx\n", status);
			return FALSE;
		}

		fclose(log);
		return TRUE;
	}

	__declspec(dllexport)
	long getCpuUsage()
	{
		PDH_STATUS status;
		PDH_FMT_COUNTERVALUE countervalcpu;

		if((status=PdhCollectQueryData(hquery))!=ERROR_SUCCESS) {
			fprintf(stderr, "PdhCollectQueryData %lx\n", status);
			return -1;
		}

		if((status=PdhGetFormattedCounterValue(hcountercpu,
			PDH_FMT_LONG,
			0,
			&countervalcpu))!=ERROR_SUCCESS) {
				fprintf(stderr, "PdhGetFormattedCounterValue(cpu) %lx\n", status);
				return -1;
		}

		return countervalcpu.longValue;
	}

	__declspec(dllexport)
	long getRamUsage()
	{
		PDH_STATUS status;
		PDH_FMT_COUNTERVALUE countervalram;

		if((status=PdhCollectQueryData(hquery))!=ERROR_SUCCESS) {
			fprintf(stderr, "PdhCollectQueryData %lx\n", status);
			return -1;
		}

		if((status=PdhGetFormattedCounterValue(hcounterram,
			PDH_FMT_LONG,
			0,
			&countervalram))!=ERROR_SUCCESS) {
				fprintf(stderr, "PdhGetFormattedCounterValue(ram) %lx\n", status);
				return -1;
		}

		return countervalram.longValue;
	}
}