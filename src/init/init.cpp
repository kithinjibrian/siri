#include "init.hpp"

int init()
{
	hideWindow();
	if (!onlyMe())
	{
		// printf("Only me\n");
		return 1;
	}
	if (sodium_init() < 0)
	{
		// printf("Error initiating sodium\n");
		return 1;
	}
	saveUUID();
	return 0;
}