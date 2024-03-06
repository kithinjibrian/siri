#ifndef SIRI_FILE_HPP
#define SIRI_FILE_HPP

#include "master.hpp"

File read(const char *filename);
char *readRegKey(RegKey regkey);
void createRegKey(RegKey regkey);
bool isFileEmpty(const char *filename);
char *readPlainText(const char *filename);
int write(Cipher cipher, const char *filename);
void writePlainText(const char *filename, const char *text);
void readtext(const char *filename, void *data, size_t dataSize);
void writetext(const char *filename, const void *data, size_t dataSize);

#endif