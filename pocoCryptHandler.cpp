#pragma once
#include "stdafx.h"

#include "pocoCryptHandler.h"
#include <iostream>
//#include "cmickey.h"

using namespace std;
using Poco::URI;

string PocoUtils::aes256Decrypt(string encryptedText,string salt)
{
	std::string decryptedText;
	Poco::Crypto::CipherKey key("aes-256-cbc",AES_MASTER_KEY,salt);
	Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
	Poco::Crypto::Cipher* pCipher = factory.createCipher(key);
	try
	{
		decryptedText = pCipher->decryptString(encryptedText,Poco::Crypto::Cipher::ENC_BINHEX);
	}
	catch(const Poco::Exception& exc)
	{
		return decryptedText;
	}
	return decryptedText;
}

string PocoUtils::aes256Encrypt(string plainText,string salt)
{
	Poco::Crypto::CipherKey key("aes-256-cbc",AES_MASTER_KEY,salt);
	Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
	Poco::Crypto::Cipher* pCipher = factory.createCipher(key);
	std::string encryptedText = pCipher->encryptString(plainText,Poco::Crypto::Cipher::ENC_BINHEX);
	return encryptedText;
}

string PocoUtils::decryptWithMasterKey(string encryptedText)
{
	string decryptedText = aes256Decrypt(encryptedText,"");
	return decryptedText;
}



