#ifndef SIRI_FILE2_HPP
#define SIRI_FILE2_HPP

#include "master.hpp"

void freeFilePaths(FilePaths *filePaths);
void initializeFilePaths(FilePaths *filePaths);
void addFilePath(FilePaths *filePaths, const char *path);
void listFilesRecursively(
	const char *path,
	FilePaths *filePaths,
	const char **blacklistFolders,
	size_t numBlacklistFolders,
	const char **whitelistFileTypes,
	size_t numWhitelistFileTypes,
	const char **blacklistFiles,
	size_t numBlacklistFiles);

#endif