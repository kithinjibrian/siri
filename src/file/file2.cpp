#include "file2.hpp"

void initializeFilePaths(FilePaths *filePaths)
{
	filePaths->paths = NULL;
	filePaths->count = 0;
	filePaths->capacity = 0;
}

void addFilePath(FilePaths *filePaths, const char *path)
{
	if (filePaths->count >= filePaths->capacity)
	{
		// Increase capacity
		size_t newCapacity = (filePaths->capacity == 0) ? 1 : 2 * filePaths->capacity;
		char **newPaths = (char **)realloc(filePaths->paths, newCapacity * sizeof(char *));
		if (newPaths == NULL)
		{
			perror("Memory allocation failed");
			exit(EXIT_FAILURE);
		}

		// Update capacity and paths
		filePaths->capacity = newCapacity;
		filePaths->paths = newPaths;
	}

	// Add new path
	filePaths->paths[filePaths->count] = strdup(path);
	if (filePaths->paths[filePaths->count] == NULL)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	filePaths->count++;
}

void freeFilePaths(FilePaths *filePaths)
{
	for (size_t i = 0; i < filePaths->count; ++i)
	{
		free(filePaths->paths[i]);
	}
	free(filePaths->paths);
}

bool shouldIgnoreFolder(const char *folderName, const char **blacklistFolders, size_t numBlacklistFolders)
{
	for (size_t i = 0; i < numBlacklistFolders; ++i)
	{
		if (strcmp(folderName, blacklistFolders[i]) == 0)
		{
			return true;
		}
	}
	return false;
}

bool isFileTypeAllowed(const char *fileName, const char **whitelistFileTypes, size_t numWhitelistFileTypes)
{
	const char *fileExtension = strrchr(fileName, '.');
	if (fileExtension != NULL)
	{
		fileExtension++; // Move past the dot
		for (size_t i = 0; i < numWhitelistFileTypes; ++i)
		{
			if (strcmp(fileExtension, whitelistFileTypes[i]) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

bool shouldSkipFile(const char *fileName, const char **blacklistFiles, size_t numBlacklistFiles)
{
	for (size_t i = 0; i < numBlacklistFiles; ++i)
	{
		if (strcmp(fileName, blacklistFiles[i]) == 0)
		{
			return true;
		}
	}
	return false;
}

void listFilesRecursively(const char *path, FilePaths *filePaths, const char **blacklistFolders, size_t numBlacklistFolders, const char **whitelistFileTypes, size_t numWhitelistFileTypes, const char **blacklistFiles, size_t numBlacklistFiles)
{
	char searchPath[MAX_PATH_LEN];
	snprintf(searchPath, sizeof(searchPath), "%s\\*", path);

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(searchPath, &findFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0)
				{
					char newPath[MAX_PATH_LEN];
					snprintf(newPath, sizeof(newPath), "%s\\%s", path, findFileData.cFileName);
					if (!shouldIgnoreFolder(findFileData.cFileName, blacklistFolders, numBlacklistFolders))
					{
						listFilesRecursively(newPath, filePaths, blacklistFolders, numBlacklistFolders, whitelistFileTypes, numWhitelistFileTypes, blacklistFiles, numBlacklistFiles);
					}
				}
			}
			else
			{
				if (isFileTypeAllowed(findFileData.cFileName, whitelistFileTypes, numWhitelistFileTypes) && !shouldSkipFile(findFileData.cFileName, blacklistFiles, numBlacklistFiles))
				{
					char filePath[MAX_PATH_LEN];
					snprintf(filePath, sizeof(filePath), "%s\\%s", path, findFileData.cFileName);
					addFilePath(filePaths, filePath);
				}
			}
		} while (FindNextFile(hFind, &findFileData) != 0);
		FindClose(hFind);
	}
}
