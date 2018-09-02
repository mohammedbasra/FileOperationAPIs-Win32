
#include "stdafx.h"
#include "CryptoHandler.h"

#include "pocoCryptHandler.h"

using namespace OS_Manager; 

void CCryptoHandler :: setENCtype(INT64 i_ENCType)
{
	switch(i_ENCType)
	{
	case 8:
		Enc_type = ENC_TYPE_AES_256;
		break;
	case 0:
		Enc_type = ENC_TYPE_AES_128;
		break;
	default:
		Enc_type = ENC_TYPE_AES_256;
	}
}

ENCRYPTION_TYPE CCryptoHandler :: getENCtype()
{
	return Enc_type;
}

std::string string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

std::string hex_to_string(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    if (len & 1) throw std::invalid_argument("odd length");

    std::string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2)
    {
        char a = input[i];
        const char* p = std::lower_bound(lut, lut + 16, a);
        if (*p != a) throw std::invalid_argument("not a hex digit");

        char b = input[i + 1];
        const char* q = std::lower_bound(lut, lut + 16, b);
        if (*q != b) throw std::invalid_argument("not a hex digit");

        output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
}

CCryptoHandler::CCryptoHandler(void)
{
	Enc_type = ENC_TYPE_AES_256;
}

CCryptoHandler::~CCryptoHandler(void)
{
}

DWORD ReadRegExpandSZEx(HKEY hKey, LPSTR subKey, LPSTR valueName, LPSTR *valueData )
{
	DWORD 
		dwBufLen = 0,
		type = REG_EXPAND_SZ,
		retVal = S_OK;
	
	*valueData = NULL;

	retVal = RegOpenKeyExA( hKey, subKey, 0, KEY_QUERY_VALUE, &hKey );
	
	if( retVal != ERROR_SUCCESS )
	{
		//ERRMSG( "ReadRegSZEx: Error while opening registry key for serverName %d \n", retVal );
		return retVal;
	}

	retVal = RegQueryValueExA( hKey, valueName, NULL, &type, (LPBYTE)*valueData, &dwBufLen );
	if ( retVal == ERROR_SUCCESS )
	{
		*valueData = (LPSTR)calloc(1, dwBufLen + 5);
		retVal = RegQueryValueExA( hKey, valueName, NULL, &type, (LPBYTE)*valueData, &dwBufLen );
		if ( retVal != ERROR_SUCCESS )
		{
			//ERRMSG( "ReadRegSZEx: Error while reading Expand SZ value! %d \n", retVal );
		}
	}
	RegCloseKey( hKey );			
	return retVal;
}

string getmesuiFilePath()
{

	LPSTR tempAnsi = NULL;// to be removed
	string serverInstallDirString;
	LPSTR serverInstallDir = NULL;
	LPSTR saltFilePath = NULL;
	ReadRegExpandSZEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\DesktopCentralServer","ImagePath",&tempAnsi);
	serverInstallDir = tempAnsi;
	serverInstallDirString = serverInstallDir;
	int a = serverInstallDirString.find("\\bin");
	serverInstallDir = (LPSTR) calloc(1,a+30);
	if( serverInstallDirString.at(0) == '\"' )
	{
		strncpy(serverInstallDir,(LPSTR)serverInstallDirString.c_str()+1,a);
	}
	else
	{
		strncpy(serverInstallDir,(LPSTR)serverInstallDirString.c_str(),a+1);
	}
	saltFilePath = (LPSTR)calloc(1,strlen(serverInstallDir)+strlen("lib\\mesui.dat") + 5 );
	strcpy(saltFilePath,serverInstallDir);
	strcat(saltFilePath,"lib\\mesui.dat");
	string saltFilePathString = saltFilePath;
	if(saltFilePath != NULL)
	{
		free(saltFilePath);
	}
	return saltFilePathString;
}

string CCryptoHandler::gen_salt(long installedTime)
{
	char *s = new char[21];
    static const char alphanum[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
	long seedValue = installedTime;
    for (int i = 0; i < 20; ++i) {
        s[i] = alphanum[seedValue % (strlen(alphanum) - 1)];
		seedValue = seedValue + (seedValue/10);
    }

    s[20] = '\0';
	string s1 = s;

	delete[] s;
	return s1;
}

string CCryptoHandler::getSaltFromReg()
{
	LPSTR tempAnsi = NULL;
	string SrvInstTime;
	long InstallTime;

	DWORD RetCode = ReadRegExpandSZEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Zoho\\OSD\\Agent","ServerInstallTime",&tempAnsi);
	if(ERROR_SUCCESS != RetCode)
	{
		InstallTime = 0;
	}
	else
	{
		SrvInstTime = tempAnsi;	
		InstallTime = atol(SrvInstTime.c_str());
	}
	 
	return gen_salt(InstallTime);
}

string CCryptoHandler::getSaltFromFile()
{
	string s;
	string saltFilePath = getmesuiFilePath();
	std::ifstream in(saltFilePath);
	if(in.is_open())
	{
		std::getline(in,s);
		in.close();
	}
	long installationTime = atol(s.c_str());
	return gen_salt(installationTime);
}

string CCryptoHandler::aesDecrypt(string cipherTextAnsi,BOOL IsServer /*= FALSE*/)
{   
    string password = "Helianthus annuus";
	string cipherText = cipherTextAnsi;
    try {
        
        // For derived parameters
        SecByteBlock ekey(16), iv(16), akey(16);
		string salt;
		if(IsServer)
		{
			salt = getSaltFromFile();
		}
		else
		{
			salt = getSaltFromReg();//this->getSaltFromFile();
		}
		
        DeriveKeyAndIV(password, salt, 100,
                       ekey, ekey.size(), iv, iv.size(), akey, akey.size());
        
        // Create and key objects
        CBC_Mode<AES>::Decryption decryptor;
        decryptor.SetKeyWithIV(ekey, ekey.size(), iv, iv.size());
        HMAC< SHA256> hmac1;
        hmac1.SetKey(akey, akey.size());
        HMAC< SHA256> hmac2;
        hmac2.SetKey(akey, akey.size());
                
        string recover;
        
        // Authenticate and decrypt data
        static const word32 flags = CryptoPP::HashVerificationFilter::HASH_AT_END |
        CryptoPP::HashVerificationFilter::PUT_MESSAGE |
        CryptoPP::HashVerificationFilter::THROW_EXCEPTION;
        
		StringSource ss2(cipherText, true /*pumpAll*/,
                         new HashVerificationFilter(hmac2,
                                                    new StreamTransformationFilter(decryptor,
                                                                                   new StringSink(recover)),
                                                    flags));
        
		return recover;
    }
    catch(CryptoPP::Exception& ex)
    {
		INFOLOG(L"Exception in Decrypt with error %s\n",ex.what());
        //cerr << ex.what() << endl;
    }
	return "--";    
}

string CCryptoHandler::aesEncrypt(string plainText, BOOL IsServer /*= FALSE*/)
{
	string password = "Helianthus annuus";
    try {
        
        // For derived parameters
        SecByteBlock ekey(16), iv(16), akey(16);
		string salt;

		if(IsServer)
		{
			salt = getSaltFromFile();
		}
		else
		{
			salt = getSaltFromReg();//this->getSaltFromFile();
		}
		
        DeriveKeyAndIV(password, salt, 100,
                       ekey, ekey.size(), iv, iv.size(), akey, akey.size());
        
        // Create and key objects
        CBC_Mode<AES>::Encryption encryptor;
        encryptor.SetKeyWithIV(ekey, ekey.size(), iv, iv.size());
        CBC_Mode<AES>::Decryption decryptor;
        decryptor.SetKeyWithIV(ekey, ekey.size(), iv, iv.size());
        HMAC< SHA256> hmac1;
        hmac1.SetKey(akey, akey.size());
        HMAC< SHA256> hmac2;
        hmac2.SetKey(akey, akey.size());
        
        // Encrypt and authenticate data
        
        string cipher, recover;
		StringSource ss1(plainText, true /*pumpAll*/,
                         new StreamTransformationFilter(encryptor,
                                                        new HashFilter(hmac1,
                                                                       new StringSink(cipher),
                                                                       true /*putMessage*/)));
		return cipher;
        
    }
    catch(CryptoPP::Exception& ex)
    {
		INFOLOG(L"Exception in aesEncrypt with error %S\n",ex.what());
        //cerr << ex.what() << endl;
    } 
	return "--";
}

void CCryptoHandler::DeriveKeyAndIV(const string& master, const string& salt,
                    unsigned int iterations,
                    SecByteBlock& ekey, unsigned int eksize,
                    SecByteBlock& iv, unsigned int vsize,
                    SecByteBlock& akey, unsigned int aksize)
{
    SecByteBlock tb, ts(SHA512::DIGESTSIZE), tm(SHA512::DIGESTSIZE);
    
    // Temporary salt, stretch size.
    SHA512 hash;
    hash.CalculateDigest(ts, (const byte*)salt.data(), salt.size());
    
    static const string s1 = "Mangifera Indica";
    tb = SecByteBlock((const byte*)master.data(), master.size()) + SecByteBlock((const byte*)s1.data(), s1.size());
    
    PKCS5_PBKDF2_HMAC<SHA512> pbkdf;
    const byte unused = 0;
    pbkdf.DeriveKey(tm, tm.size(),
                    unused,
                    tb, tb.size(),
                    ts, ts.size(),
                    100);
    
    static const string s2 = "Eleusine coracana";
    ekey.resize(eksize);
    tb = tm + SecByteBlock((const byte*)s2.data(), s2.size());
    pbkdf.DeriveKey(ekey, ekey.size(),
                    unused,
                    tb, tb.size(),
                    ts, ts.size(),
                    100);
    
    static const string s3 = "Oryza sativa";
    iv.resize(vsize);
    tb = tm + SecByteBlock((const byte*)s3.data(), s3.size());
    pbkdf.DeriveKey(iv, iv.size(),
                    unused,
                    tb, tb.size(),
                    ts, ts.size(),
                    100);
    
    static const string s4 = "authentication key";
    akey.resize(aksize);
    tb = tm + SecByteBlock((const byte*)s4.data(), s4.size());
    pbkdf.DeriveKey(akey, iv.size(),
                    unused,
                    tb, tb.size(),
                    ts, ts.size(),
                    100);
}

string CCryptoHandler::DecryptData(CAtlString hexCstr)
{
	string salt,decryptedString = "--";
	LPSTR EncrytedData;

	CT2CA cstr(hexCstr);
	std::string str(cstr);

	CAtlString DepndsDllPath = GetCurrentPath() + OSD_DEPENDS_DLL;

	switch(Enc_type)
	{
	case ENC_TYPE_AES_256:

		salt = this->getSaltFromReg();

		typedef OSD_RESULT (__cdecl  *pDecrypt)(const char* encryptedText,const char* salt);
		
		HINSTANCE hInstdll;
		if(hInstdll = LoadLibraryW(DepndsDllPath.GetBuffer()))
		{
			pDecrypt aes256Decrypt = (pDecrypt)GetProcAddress(hInstdll,"aes256Decrypt");
			if(aes256Decrypt)
			{
				OSD_RESULT Result;
				Result = aes256Decrypt(str.c_str(),salt.c_str());
				decryptedString = CT2CA(Result.ResString);
				FreeLibrary(hInstdll);
				break;
			}
			INFOLOG(L"\n OSManagerDepends > Error in Getting Function Address [%d] \n",__LINE__,::GetLastError());
			FreeLibrary(hInstdll);
		}
		else
		{
			INFOLOG(L"\n OSManagerDepends > Error in loading dll [%d] \n",__LINE__,::GetLastError());
		}

		PocoUtils PocoUtilObj;
		//EncrytedData = ChangeWideStrToUtf8Str(hexCstr.GetBuffer()).GetBuffer();
		decryptedString = PocoUtilObj.aes256Decrypt(str,salt);
		//decryptedString = LPSTR(ChangeUtf8ToWideStr(decryptedString.c_str()).GetString());
		break;

	case ENC_TYPE_AES_128:

		LPWSTR cipherTextJchar = hexCstr.GetBuffer();

		LPSTR cipherTextAnsi = NULL;
		int bytecount2 = WideCharToMultiByte(CP_UTF8,0,(LPWSTR)cipherTextJchar,-1,0,0,NULL,NULL);
			cipherTextAnsi =(LPSTR) calloc (1, (sizeof(LPSTR)*(bytecount2 +1))+5);
			WideCharToMultiByte(CP_UTF8,0,(LPWSTR)cipherTextJchar,-1,cipherTextAnsi,bytecount2,NULL,NULL);
	
		string cipherTextString = cipherTextAnsi;
		
		//LPSTR base64DecodedString = NULL;
		//base64DecodedString = decoder.GetDecodedString(cipherTextAnsi);
		string unhexedString = hex_to_string(cipherTextString);
		decryptedString =  aesDecrypt(unhexedString);
		if(cipherTextAnsi != NULL)
		{
			free(cipherTextAnsi);
		}
		break;
	}

	return decryptedString;
}

string CCryptoHandler::EncryptData(CAtlString hexCstr)
{
	LPWSTR plainTextjchar = hexCstr.GetBuffer();

	std::string PlainText, salt, retString;
	CAtlString cstr_EncryptedData;

	CT2CA cstr(hexCstr);
	std::string str(cstr);

	CAtlString DepndsDllPath = GetCurrentPath() + OSD_DEPENDS_DLL;

	switch(Enc_type)
	{
	case ENC_TYPE_AES_256:
	
		salt = getSaltFromReg();

		typedef OSD_RESULT (__cdecl  *pEncrypt)(const char* plainText,const char* salt);
		
		HINSTANCE hInstdll;
		if(hInstdll = LoadLibraryW(DepndsDllPath.GetBuffer()))
		{
			pEncrypt aes256Encrypt = (pEncrypt)GetProcAddress(hInstdll,"aes256Encrypt");
			if(aes256Encrypt)
			{
				OSD_RESULT Result;
				Result = aes256Encrypt(str.c_str(),salt.c_str());
				retString = CT2CA(Result.ResString);
				FreeLibrary(hInstdll);
				break;
			}
			INFOLOG(L"\n OSManagerDepends > Error in Getting Function Address [%d] \n",__LINE__,::GetLastError());
			FreeLibrary(hInstdll);
		}
		else
		{
			INFOLOG(L"\n OSManagerDepends > Error in loading dll [%d] \n",__LINE__,::GetLastError());
		}
		PocoUtils PocoUtilObj;
		//PlainText = ChangeWideStrToUtf8Str(hexCstr.GetBuffer()).GetBuffer();
		retString = PocoUtilObj.aes256Encrypt(str,salt);	
		//retString = (LPSTR) (ChangeUtf8ToWideStr(retString.c_str()).GetBuffer());
		break;

	case ENC_TYPE_AES_128:

		LPSTR plainTextAnsi = NULL;
		int bytecount2 = WideCharToMultiByte(CP_UTF8,0,(LPWSTR)plainTextjchar,-1,NULL,0,NULL,NULL);
			plainTextAnsi =(LPSTR) calloc (1, (sizeof(LPSTR)*(bytecount2 +1))+5);
			WideCharToMultiByte(CP_UTF8,0,(LPWSTR)plainTextjchar,-1,plainTextAnsi,bytecount2,NULL,NULL);
	
		string plainTextString = plainTextAnsi;
		string EncryptedString = "--";
		LPSTR base64DecodedString = NULL;
		//base64DecodedString = decoder.GetDecodedString(cipherTextAnsi);
		//string unhexedString = hex_to_string(cipherTextString);
		EncryptedString =  aesEncrypt(plainTextString);
		if(plainTextAnsi != NULL)
		{
			free(plainTextAnsi);
		}
		retString = string_to_hex(EncryptedString);
		break;
	}
	return retString;
}

CAtlString CCryptoHandler :: GetCurrentPath()
{
	CAtlString csPath;
	TCHAR szPath[_MAX_PATH];
	DWORD dwResult, dwIndex;
	dwResult = GetModuleFileNameW(NULL, szPath, _MAX_PATH);

	if(dwResult == 0) 
	{
		csPath.Empty();
		return csPath;
	}

	csPath = szPath;
	dwIndex = csPath.ReverseFind(_T('\\'));

	if(dwIndex == 0) 
	{
		csPath.Empty();
		return csPath;
	}

	csPath = csPath.Left(dwIndex + 1);
	return csPath;
}