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