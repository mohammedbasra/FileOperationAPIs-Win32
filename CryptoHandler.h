#pragma once

#include <cryptlib.h>
using CryptoPP::lword;
using CryptoPP::word32;
using CryptoPP::word64;
using CryptoPP::Exception;

#include <secblock.h>
using CryptoPP::SecByteBlock;
using CryptoPP::SecBlock;

#include <hex.h>
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include <filters.h>
using CryptoPP::Redirector;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::HashVerificationFilter;
using CryptoPP::HashFilter;

#include <files.h>
using CryptoPP::FileSink;

#include <sha.h>
using CryptoPP::SHA256;
using CryptoPP::SHA512;

#include <aes.h>
using CryptoPP::AES;

#include <modes.h>
using CryptoPP::CBC_Mode;

#include <pwdbased.h>
using CryptoPP::PKCS5_PBKDF2_HMAC;

#include <hmac.h>
using CryptoPP::HMAC;

typedef enum tagEncryptionType
{
	ENC_TYPE_INVALID,
	ENC_TYPE_AES_128,
	ENC_TYPE_AES_256

}ENCRYPTION_TYPE;

#define OSD_DEPENDS_DLL		L"OSManagerDepends.dll"

class CCryptoHandler
{
private:
	ENCRYPTION_TYPE Enc_type;
public:
	//CCryptoHandler(void);
	~CCryptoHandler(void);
public:
	CCryptoHandler();
	string aesDecrypt(string cipherText,BOOL IsServer = FALSE);
	string aesEncrypt(string plainTextAnsi, BOOL IsServer = FALSE);
	void DeriveKeyAndIV(const string& master, const string& salt,
                    unsigned int iterations,
                    SecByteBlock& ekey, unsigned int eksize,
                    SecByteBlock& iv, unsigned int vsize,
                    SecByteBlock& akey, unsigned int aksize);
	//void StoreSalt(string salt);
	string getSaltFromReg();
	string getSaltFromFile();
	string gen_salt(long installedTime);
	string DecryptData(CAtlString hexCstr);
	string EncryptData(CAtlString hexCstr);

	ENCRYPTION_TYPE getENCtype();
	void setENCtype( INT64 i_ENCType );

	CAtlString GetCurrentPath();
	
};

