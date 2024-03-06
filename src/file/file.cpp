#include "file.hpp"

File read(const char *filename)
{
	FILE *input_file = fopen(filename, "rb");
	File file;
	if (!input_file)
	{
		printf("Error opening input file, %s\n", filename);
		file.text = NULL;
		return file;
	}

	fseek(input_file, 0, SEEK_END);
	file.size = ftell(input_file);
	fseek(input_file, 0, SEEK_SET);

	file.text = (unsigned char *)malloc(file.size);
	if (!file.text)
	{
		// printf("Memory allocation error\n");
		fclose(input_file);
		file.text = NULL;
		return file;
	}
	fread(file.text, 1, file.size, input_file);
	fclose(input_file);

	return file;
}

void readtext(const char *filename, void *data, size_t dataSize)
{
	FILE *file = fopen(filename, "rb");
	if (file != NULL)
	{
		fread(data, dataSize, 1, file);
		fclose(file);
	}
}

char *readPlainText(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		// printf("Error opening file %s\n", filename);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	rewind(file);

	char *content = (char *)malloc(file_size + 1);
	if (content == NULL)
	{
		// printf("Error allocating memory\n");
		fclose(file);
		return NULL;
	}

	size_t bytes_read = fread(content, 1, file_size, file);
	if (bytes_read != file_size)
	{
		// printf("Error reading file\n");
		fclose(file);
		free(content);
		return NULL;
	}

	content[file_size] = '\0';

	fclose(file);

	return content;
}

int write(Cipher cipher, const char *filename)
{
	FILE *output_file = fopen(filename, "wb");
	if (!output_file)
	{
		// printf("Error opening output file\n");
		return 1;
	}

	fwrite(cipher.text, 1, cipher.size + crypto_aead_xchacha20poly1305_ietf_ABYTES, output_file);
	fwrite(cipher.tag, 1, crypto_aead_xchacha20poly1305_ietf_ABYTES, output_file);
	fclose(output_file);

	return 0;
}

void writetext(const char *filename, const void *data, size_t dataSize)
{
	FILE *file = fopen(filename, "wb");
	if (file != NULL)
	{
		fwrite(data, dataSize, 1, file);
		fclose(file);
	}
}

void writePlainText(const char *filename, const char *text)
{

	FILE *file = fopen(filename, "w");
	if (file == NULL)
	{
		// printf("Error opening file %s\n", filename);
		return;
	}

	fprintf(file, "%s", text);

	fclose(file);
}

bool isFileEmpty(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		return true; // File doesn't exist
	}
	fseek(file, 0, SEEK_END);
	if (ftell(file) == 0)
	{
		fclose(file);
		return true; // File is empty
	}
	fclose(file);
	return false; // File exists and is not empty
}

void createRegKey(RegKey regkey)
{

	HKEY hKey;
	LONG regStatus = RegCreateKeyEx(
		regkey.hKey,
		regkey.lpSubKey,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_WRITE,
		NULL,
		&hKey,
		NULL);

	if (regStatus == ERROR_SUCCESS)
	{

		regStatus = RegSetValueEx(
			hKey,
			regkey.lpValueName,
			0,
			REG_SZ,
			(const BYTE *)regkey.lpData,
			strlen(regkey.lpData) + 1);

		/*if (regStatus == ERROR_SUCCESS)
		{
			printf("UUID successfully stored in the registry.\n");
		}
		else
		{
			printf("Error storing UUID in the registry. Error code: %d\n", regStatus);
		}*/

		RegCloseKey(hKey);
	}
	/*else
	{
		printf("Error creating registry key. Error code: %d\n", regStatus);
	}*/
}

char *readRegKey(RegKey regkey)
{
	HKEY hKey;
	LONG openStatus, queryStatus;
	DWORD type = REG_SZ;
	DWORD dataSize = MAX_VALUE_NAME;
	char *data = (char *)malloc(dataSize);
	if (data == NULL)
	{
		// printf("Memory allocation failed\n");
		return NULL;
	}

	openStatus = RegOpenKeyEx(regkey.hKey, regkey.lpSubKey, 0, KEY_QUERY_VALUE, &hKey);

	if (openStatus == ERROR_SUCCESS)
	{

		queryStatus = RegQueryValueEx(hKey, regkey.lpValueName, NULL, &type, (LPBYTE)data, &dataSize);

		RegCloseKey(hKey);
		if (queryStatus == ERROR_SUCCESS)
		{
			return data;
		}
		else
		{
			// printf("Error querying registry key: %d\n", queryStatus);
			free(data);
			return NULL;
		}
	}
	else
	{
		// printf("Error opening registry key: %d\n", openStatus);
		free(data);
		return NULL;
	}
}