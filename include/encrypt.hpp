#ifndef SIRI_ENCRYPT_HPP
#define SIRI_ENCRYPT_HPP

#include "file.hpp"
#include "file2.hpp"
#include "jfunc.hpp"
#include "master.hpp"
#include "linked.hpp"
#include "sxor.hpp"

KeyIV genKeyIv();
char *encryptDisk();
void sealKeys(char *data);
KeyIV *genIV(KeyIV *keyiv);
char *XOR(char *data, char key);
char *deXOR(char *hex, char *key);
char *thread(const char *directory);
Cipher chacha(KeyIV keyiv, File file);
// char *encryptFolder(const char *basePath);
Meta fileEncrypt(KeyIV keyiv, const char *filename);

#endif