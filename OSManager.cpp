#include "stdafx.h"
#include "../../OSManagerSplitter/include/FileAccess.h"
#include "../../OSManagerSplitter/include/Local_FileAccess.h"
#include "iostream"
#include "Windows.h"
#include "atlbase.h"
#include "atlstr.h"
#include "comutil.h"
#include "time.h"
using namespace OS_Manager;
using namespace std;

struct header {
	int counter;
};

int main()
{
	
	header h,r;
	h.counter = 0;
	char srcPath[500], destPath[500];
	BYTE *buffer;
	string s = "C:\\Users\\Administrator\\Desktop\\Text\\file1.txt";
	string s2 = "C:\\Users\\Administrator\\Desktop\\Text\\file1-duplicate.txt";
	strcpy_s(srcPath, s.c_str());
	strcpy_s(destPath, s2.c_str());
	CDiskIO *mDisk = CDiskIO::Instance();
	IFileAccess *fileAccess = IFileAccess::CreateInstance(FILE_ACCESS_LOCAL);
	OSD_RESULT o_Result;
	HANDLE hFile;
	DWORD num_of_bytes_read, num_of_bytes_write,lowByte,highByte;
	LONGLONG BUFFER_SIZE = 100*1024*1024, fileSize;
	INT x;
	LARGE_INTEGER file;
	mDisk->SetFileName(FILE_ACCESS_SRC, 0, srcPath);
	mDisk->SetFileName(FILE_ACCESS_DEST, 0, destPath);
	mDisk->SetFragmentSize(4*1024*1024*1024);
	mDisk->SetHeaderFlag(FALSE);
	fileAccess->Create(DISKIO_OPEN_EXISTING, FILE_ACCESS_SRC, 0, &o_Result);
	
	
	mDisk->SetReadSplitFlag(TRUE);
	mDisk->SetFragmentSize(0);
	
	fileAccess->Create(DISKIO_OPEN_EXISTING, FILE_ACCESS_SRC, 0, &o_Result);
	mDisk->SetSourcePath(srcPath);
	lowByte = fileAccess->GetFileSize(FILE_ACCESS_SRC, 0, &highByte, &o_Result);
	hFile = fileAccess->Create(DISKIO_CREATE_ALWAYS, FILE_ACCESS_DEST, 0, &o_Result);
	vector<int> reads;
	file.HighPart = highByte;
	file.LowPart = lowByte;
	LONGLONG FILE_SIZE = file.QuadPart;
	LONGLONG count = 1;
	
	/*
	map < int, LONGLONG > mp;
	mp[0] = 100*1024*1024;
	mp[1] = 500*1024;
	mp[2] = 100*1024;
	mp[3] = 50*1024;
	*/
	//IO_BUFFER_SIZE_LOW			(50*1024*1024)
	//IO_BUFFER_SIZE_MEDIUM		(100*1024*1024)
	//IO_BUFFER_SIZE_HIGH			(400*1024*1024)
	int IO_BUFFER_SIZE_MEDIUMl = 1024;
	while (FILE_SIZE > IO_BUFFER_SIZE_MEDIUMl) {
		reads.push_back(IO_BUFFER_SIZE_MEDIUMl);
		FILE_SIZE -= IO_BUFFER_SIZE_MEDIUMl;
	}
	if (FILE_SIZE > 0)reads.push_back(FILE_SIZE);
	buffer = (BYTE*)malloc(IO_BUFFER_SIZE_MEDIUMl * sizeof(BYTE*));
	char *ptr_new;
	for (int i = 0; i < reads.size() ; i++) {
		fileAccess->Read(FILE_ACCESS_SRC, 0, buffer, reads[i], &num_of_bytes_read, NULL, &o_Result);

		fileAccess->Write(FILE_ACCESS_DEST, 0, buffer, reads[i], &num_of_bytes_write, NULL, &o_Result);
	}
		
	/*
	LONGLONG fragSize = 1024;
	file.QuadPart = fragSize;
	fileAccess->SplitFile(FILE_ACCESS_SRC,0,file,&o_Result);
	*/
	
	/*
	hFile = fileAccess->Create(DISKIO_OPEN_EXISTING, FILE_ACCESS_SRC, 0, &o_Result);
	//hFile = fileAccess->Create(DISKIO_CREATE_ALWAYS, FILE_ACCESS_DEST, 0, &o_Result);
	lowByte = fileAccess->GetFileSize(FILE_ACCESS_SRC, 0, &highByte, &o_Result);
	file.LowPart = lowByte;
	file.HighPart = highByte;
	fileSize = file.QuadPart;
	buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));
	num_of_bytes_write = 0;
	while (fileSize > 0) {
		if (fileSize > BUFFER_SIZE) {
			fileAccess->Read(FILE_ACCESS_SRC, 0, buffer, BUFFER_SIZE, &num_of_bytes_read, NULL, &o_Result);
			fileAccess->Write(FILE_ACCESS_DEST, 1, buffer, num_of_bytes_read, &num_of_bytes_write, NULL, &o_Result);
			fileSize -= num_of_bytes_read;
		}
		else {
			fileAccess->Read(FILE_ACCESS_SRC, 0, buffer, fileSize, &num_of_bytes_read, NULL, &o_Result);
			fileAccess->Write(FILE_ACCESS_DEST, 0, buffer, num_of_bytes_read, &num_of_bytes_write, NULL, &o_Result);
			fileSize -= num_of_bytes_read;
		}
	}
	/*
	*/
	return 0;
}