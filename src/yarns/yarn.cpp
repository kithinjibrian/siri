#include "yarn.hpp"

void yarn(int yarnNo, ...)
{
	va_list args;
	va_start(args, yarnNo);
	HANDLE hThreads[yarnNo];

	for (int i = 0; i < yarnNo; i++)
	{
		LPTHREAD_START_ROUTINE threadFunc = va_arg(args, LPTHREAD_START_ROUTINE);
		hThreads[i] = CreateThread(
			NULL,
			0,
			threadFunc,
			NULL,
			0,
			NULL);

		if (hThreads[i] == NULL)
		{
			fprintf(stderr, "Error creating thread %d\n", i);
			return;
		}
	}
	WaitForMultipleObjects(yarnNo, hThreads, TRUE, INFINITE);
	for (int i = 0; i < yarnNo; i++)
	{
		CloseHandle(hThreads[i]);
	}
	va_end(args);
}