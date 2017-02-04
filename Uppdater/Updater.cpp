#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <windows.h>
#include <wininet.h>

class Ftp {

public:
	Ftp();
	~Ftp();

public:
	bool connectToHost(const char *ftp_host, const char *uid, const char *pwd);
	void close();
	bool cd(const char *dir);
	bool put(const char *file_path, const char *file_name);
	bool deleteFile(const char *file_name);
	bool renameFile(const char *f_old, const char *f_new);

	HINTERNET internet;
	HINTERNET ftp_session;
	char host[256];

};
#include <iostream>
#include <string.h>
#include <string>
#include <sstream>
#include <atlstr.h> 
#include <tchar.h>
#include <fstream>
using namespace std;



string version = "1.0.1";
#pragma comment(lib,"wininet")

Ftp::Ftp() {
}

Ftp::~Ftp() {
}

bool Ftp::connectToHost(const char *ftp_host, const char *uid, const char *pwd) {
	strcpy(host, ftp_host);
	internet = InternetOpenA(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	ftp_session = InternetConnectA(internet, host, INTERNET_DEFAULT_FTP_PORT, uid, pwd, INTERNET_SERVICE_FTP, 0, 0);
	return (ftp_session != NULL);
}

void Ftp::close() {
	InternetCloseHandle(ftp_session);
	InternetCloseHandle(internet);
}

bool Ftp::cd(const char *dir) {
	return FtpSetCurrentDirectoryA(ftp_session, dir);
}

bool Ftp::put(const char *file_path, const char *file_name) {
	return FtpPutFileA(ftp_session, file_path, file_name, FTP_TRANSFER_TYPE_BINARY, 0);
}

bool Ftp::deleteFile(const char *file_name) {
	return FtpDeleteFileA(ftp_session, file_name);
}

bool Ftp::renameFile(const char *f_old, const char *f_new) {
	return FtpRenameFileA(ftp_session, f_old, f_new);
}

int main(int argc, char* argv[]) {
	setlocale(0, "");
	/*for (int i = 0; i < argc; i++)
	{
		cout << i << " " << argv[i] << endl;
	}*/
	
	system("taskkill /f /im FTPLoader.exe");
	system("cls");
	Ftp ftp;
	char* ip = "ftp.mta-sa.ru";
	char* login =  argv[1];
	char* pass = argv[2];
	if (argc == 1)
	{
		login = "Alex";
		pass = "456258";
	}

	if (ftp.connectToHost(ip, login, pass))
		std::cout << "Успешно подключились к серверу и авторизовались" << std::endl;
	stringstream f;
	f << "DEL /Q /F " << argv[3] << "\\FTPLoader.exe";
	system(f.str().c_str());
	f.str("");
	f.clear();
	//cout << argv[3] << endl;
	//system("pause");
	f << argv[3] << "\\FTPLoader.exe";
	//cout << f.str() << endl;
	if (!FtpGetFile(
		ftp.ftp_session,
		"FTPLoader.exe",
		f.str().c_str(),
		true,
		FILE_ATTRIBUTE_NORMAL,
		FTP_TRANSFER_TYPE_BINARY,
		NULL
	))
		cout << "Не получилось скачать новую версию. Обратитесь к разработчику." << endl;
	else
	{
		f.str("");
		f.clear();
		f << argv[3] << "\\FTPLoader.exe 0 0";
		system(f.str().c_str());
	}
	ftp.close();
	system("pause");
	return 0;
}