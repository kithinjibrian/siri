#include "strings.hpp"

char *stitch(int numStrings, ...)
{
	va_list args;
	int totalLength = 0;
	char *result, *str;

	// Calculate total length of all strings
	va_start(args, numStrings);
	for (int i = 0; i < numStrings; i++)
	{
		str = va_arg(args, char *);
		totalLength += strlen(str);
	}
	va_end(args);

	// Allocate memory for the concatenated string
	result = (char *)malloc(totalLength + 1); // +1 for the null terminator
	if (!result)
	{
		// printf("Memory allocation failed.\n");
		return NULL;
	}

	// Concatenate strings
	va_start(args, numStrings);
	result[0] = '\0'; // Ensure the result is an empty string initially
	for (int i = 0; i < numStrings; i++)
	{
		str = va_arg(args, char *);
		strcat(result, str);
	}
	va_end(args);

	return result;
}
