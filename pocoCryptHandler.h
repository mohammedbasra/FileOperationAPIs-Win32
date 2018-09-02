#pragma once
#include <string>
using namespace std;

#include "Poco/Crypto/Cipher.h"
#include "Poco/Crypto/CipherKey.h"
#include "Poco/Crypto/CipherFactory.h"
#include "Poco/Exception.h"
#include <Poco\URI.h>
#include <string>

#define AES_MASTER_KEY "AESMASTERKEY0123456789"

class PocoUtils
{
public:
string aes256Decrypt(string encryptedText,string salt);
string aes256Encrypt(string plainText,string salt);
string decryptWithMasterKey(string encryptedText);
};

