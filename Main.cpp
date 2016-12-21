#define _CRT_SECURE_NO_WARNINGS
#include "ftp.h"
#include <iostream>
#include <string.h>
#include <string>
#include <sstream>
#include <atlstr.h> 
#include <tchar.h>
#include <fstream>
#include <algorithm>
using namespace std;

string version = "1.0.9";
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



string RusToEng(string m) {
	char R[] = {'А','Б' ,'В' ,'Г' ,'Д' ,'Е' ,'Ё' ,'Ж' ,'З' ,'И' ,'Й' ,'К' ,'Л' ,'М' ,'Н', 'О', 'П', 'Р', 'С', 'Т', 'У' ,'Ф' ,'Х' ,'Ц' ,'Ч' ,'Ш' ,'Щ' ,'Ъ' ,'Ы' ,'Ь' ,'Э' ,'Ю' ,'Я' };
	char r[] = { 'а','б' ,'в' ,'г' ,'д' ,'е' ,'ё' ,'ж' ,'з' ,'и' ,'й' ,'к' ,'л' ,'м' ,'н', 'о', 'п', 'р', 'с', 'т', 'у' ,'ф' ,'х' ,'ц' ,'ч' ,'ш' ,'щ' ,'ъ' ,'ы' ,'ь' ,'э' ,'ю' ,'я' };
	char E[] = { 'A', 'B', 'V', 'G', 'D', 'E', 'E', 'J', 'Z', 'I', 'I', 'K', 'L', 'M', 'N', 'O', 'P', 'R', 'S', 'T', 'U', 'F', 'H', 'C', 'ch', 'SH', 'SH', 'b', 'bI', 'b', '3', 'IO', '9I'};
	for (size_t i = 0; i < 33; i++)
	{
		replace(m.begin(), m.end(), R[i], E[i]);
		replace(m.begin(), m.end(), r[i], E[i]);
	}
	return m;
}


int main(int argc, char* argv[]) {
	setlocale(0, "");

	if (argc == 1)
	{
		string ans;
		cout << "Вы запустили программу не через контекстное меню. Провести автоматическую настройку реестра? (N/Y):";
		cin >> ans;
		//Reg configurate
		if (ans == "Y" || ans == "y")
		{
			//C:\\MyProgramms\\FTPLoader.exe s1.mta - sa.ru Alex 456258 % 1
			stringstream ss;
			TCHAR patch[MAX_PATH + 40];
			string login;
			string password;
			TCHAR Patch[MAX_PATH];
			int k = 0;
			GetCurrentDirectory(sizeof(Patch), Patch);
			//Pars and reforming patch for windows
			for (size_t i = 0; i < sizeof(Patch); i++)
			{

				if (Patch[i] == '\\')
				{
					patch[k] = '\\';
					patch[k + 1] = '\\';
					k++;
				}
				else
					patch[k] = Patch[i];
				k++;
			}
			cout << "Программа находится по пути: " << Patch << " Это местоположение можно будет изменить в будущем, еще раз запустив программу." << endl;
			cout << "Введите ваш логин от FTP:";
			cin >> login;
			cout << "Введите ваш пароль:";
			cin >> password;
			ss << patch << "\\\\FTPLoader.exe" << " s1.mta-sa.ru " << login << " " << password << " %1";
			_tsetlocale(LC_ALL, _T("Russain"));
			TCHAR *szTestString = new TCHAR[45];
			_tcscpy(szTestString, _T(ss.str().c_str()));
			// Ключ который будем создавать
			_TCHAR szPath[] = _T("*\\shell\\Отправить на сервер\\");

			HKEY hKey;

			// Создаем ключ в ветке HKEY_CLASSES_ROOT
			if (RegCreateKeyEx(HKEY_CLASSES_ROOT, szPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS) {
				cout << _T("При создании ключа произошла ошибка1") << endl;
				return 1;
			}
			if (RegCloseKey(hKey) != ERROR_SUCCESS) {
				cout << _T("При закрытии ключа произошла ошибка2") << endl;
				return 3;
			};
			// Создаем ключ в ветке HKEY_CLASSES_ROOT
			if (RegCreateKeyEx(HKEY_CLASSES_ROOT, _T("*\\shell\\Отправить на сервер\\command\\"), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS) {
				cout << _T("При создании ключа произошла ошибка cmd") << endl;
				return 1;
			}

			// Пишем тестовую строку в созданный ключ в default
			if (RegSetValue(hKey, NULL, REG_SZ, szTestString, sizeof(szTestString)) != ERROR_SUCCESS) {
				cout << _T("При записи строки произошла ошибка") << endl;
				return 2;
			}
			// Закрываем описатель ключа
			if (RegCloseKey(hKey) != ERROR_SUCCESS) {
				cout << _T("При закрытии ключа произошла ошибка") << endl;
				return 3;
			};

			ss.str("");
			ss.clear();
			//TCHAR *szTestString = new TCHAR[45];
			TCHAR Patch2[MAX_PATH];
			TCHAR patch2[MAX_PATH];
			k = 0;
			GetCurrentDirectory(sizeof(Patch2), Patch2);
			//Pars and reforming patch for windows
			for (size_t i = 0; i < sizeof(Patch2); i++)
			{

				if (Patch2[i] == '\\')
				{
					patch2[k] = '\\';
				}
				else
					patch2[k] = Patch2[i];
				k++;
			}
			ss << patch2;
			TCHAR szTestString2[MAX_PATH];
			_tcscpy(szTestString2, _T(ss.str().c_str()));

			if (RegOpenKey(
				HKEY_CLASSES_ROOT,
				szPath,
				&hKey
			) != ERROR_SUCCESS)
			{
				cout << "Cannot open Reg Key" << endl;
				system("pause");
			}
			if (RegSetValueEx(hKey, _T("Patch"), 0, REG_SZ, (BYTE*)szTestString2, sizeof(szTestString2)) != ERROR_SUCCESS) {
				cout << _T("При записи строки произошла ошибка") << endl;
				return 2;
			}

			delete szTestString;


		}
		else
			cout << "Вы отказались от автоматической настройки." << endl;

		system("pause");
		return 0;
	}

	ifstream S;
	_TCHAR szPath1[] = _T("*\\shell\\Отправить на сервер\\");
	_tsetlocale(LC_ALL, _T("Russain"));
	_TCHAR szBuf1[MAX_PATH];
	DWORD dwBufLen1 = MAX_PATH;
	HKEY hKey;
	/*RegOpenKey(
	hKey,
	_T("*\\shell\\Отправить на сервер\\"),
	&hKey
	);*/
	RegOpenKeyEx(HKEY_CLASSES_ROOT, "*\\shell\\Отправить на сервер\\", NULL, KEY_QUERY_VALUE, &hKey);
	if (RegQueryValueEx(hKey, "Patch", NULL, NULL, (LPBYTE)&szBuf1, &dwBufLen1) != ERROR_SUCCESS)
		//if (RegGetValue(HKEY_CLASSES_ROOT, szPath1, _T("Patch"), RRF_RT_REG_SZ, NULL, (BYTE*)szBuf1, &dwBufLen1) != ERROR_SUCCESS)
	{
		cout << _T("При чтении строки произошла ошибка") << endl;
		system("pause");
		return 4;
	}
	stringstream q;
	q << szBuf1 << "/conf.temp";
	S.open(q.str());
	if (S.is_open())
	{

		stringstream s, p;
		S.close();
		system("taskkill /f /im Uppdater.exe");
		system("cls");
		s << "DEL / Q / F " << szBuf1 << "\\Uppdater.exe";
		p << "DEL / Q / F " << szBuf1 << "\\conf.temp";
		system(s.str().c_str());
		system(p.str().c_str());
		cout << "Успешно обновились. Повторите отправку файла." << endl;
		system("pause");
		return 0;
	}
	S.close();

	Ftp ftp;
	char* ip;
	char* login;
	char* pass;
	std::stringstream SS;
	std::stringstream LL;

	//	std::cout << "Параметров передано:" << argc << "\n";
	ip = argv[1];
	login = argv[2];
	pass = argv[3];
	for (int i = 4; i < argc; i++)
	{
		//	std::cout << "Праметр №" << i << " = " << argv[i] << std::endl;
		if (i + 1 < argc)
		{
			SS << argv[i] << "_";
			LL << argv[i] << " ";
		}
		else
		{
			SS << argv[i] << "";
			LL << argv[i] << "";
		}
	}
	//std::string s = SS.str();
	std::string s = SS.str();
	std::string fileName;
	int SleepTime = 1000;
	int i;
	for (i = s.length(); (s[i] != '\\') && (s[i] != '/') && i >= 0; --i)
		;
	while (s[++i] != '\0')
		fileName += s[i];
	SS.str("");
	SS.clear();
	SS << login << "/" << RusToEng(fileName);
	//std::cout << SS.str() << std::endl;
	TCHAR Patch[MAX_PATH];
	TCHAR patch[MAX_PATH + 40];
	int k = 0;
	//GetCurrentDirectory(sizeof(Patch), Patch);
	////Pars and reforming patch for windows
	//for (size_t i = 0; i < sizeof(Patch); i++)
	//{

	//	if (Patch[i] == '\\')
	//	{
	//		patch[k] = '/';
	//	}
	//	else
	//		patch[k] = Patch[i];
	//	k++;
	//}
	if (ftp.connectToHost(ip, login, pass))
		std::cout << "Успешно подключились к серверу и авторизовались" << std::endl;
	else
	{
		cout << "Не верный логин или пароль. Не удалось покдлючиться к FTP." << endl;
		system("pause");
	}
	//std::cout << SS.str() << std::endl;
	stringstream *t = new stringstream();
	//HKEY hKey;
	*t << szBuf1 << "/conf.temp";
	if (!FtpGetFile(
		ftp.ftp_session,
		"settings.cfg",
		t->str().c_str(),
		true,
		FILE_ATTRIBUTE_NORMAL,
		FTP_TRANSFER_TYPE_BINARY,
		NULL
	))
	{
		system("DEL /Q /F conf.temp");
		FtpGetFile(
				ftp.ftp_session,
				"/xampp/htdocs/FTP/settings.cfg",
				t->str().c_str(),
				true,
				FILE_ATTRIBUTE_NORMAL,
				FTP_TRANSFER_TYPE_BINARY,
				NULL
			);
	}
	ifstream f;
	t->str("");
	t->clear();
	*t << szBuf1 << "/conf.temp";
	f.open(t->str());
	string temp;
	f >> temp;
	f.close();
	t->str("");
	t->clear();
	*t << szBuf1 << "\\Uppdater.exe";
	if (temp != version)
	{	
		cout << "Вышла новая версия программы. Программа автоматически обновится." << endl;
		Sleep(1000);
		if(!FtpGetFile(
			ftp.ftp_session,
			"Uppdater.exe",
			t->str().c_str(),
			true,
			FILE_ATTRIBUTE_NORMAL,
			FTP_TRANSFER_TYPE_BINARY,
			NULL
		))
		{

			system("DEL /Q /F Uppdater.exe ");
			FtpGetFile(
				ftp.ftp_session,
				"/xampp/htdocs/FTP/Uppdater.exe",
				t->str().c_str(),
				true,
				FILE_ATTRIBUTE_NORMAL,
				FTP_TRANSFER_TYPE_BINARY,
				NULL
			);
		}
		ftp.close();
		stringstream up;
		up << szBuf1 << "/Uppdater.exe " << login << " " << pass << " " << szBuf1;
		system(up.str().c_str());
		return 0;
	}
	else
	{
		stringstream up1;
		up1 << "DEL /Q /F " << szBuf1 << "\\conf.temp";
		system(up1.str().c_str());
	}
	cout << "Начата загрузка файла." << endl;
	if (ftp.put(LL.str().c_str(), SS.str().c_str()))
		std::cout << "Передача файла успешна.\n";
	else
	{
		int Error = GetLastError();
		std::cout << "Error №" << Error << std::endl;
		if (Error == 2)
			std::cout << "В названии файла есть пробел, исправьте ошибку и попробуйте еще раз.\n";
		ftp.close();
		return system("pause");

	}
	ftp.close();
	SS.str("");
	SS.clear();
	SS << "http://" << ip << "/FTP/" << login << "/" << RusToEng(fileName);
	CString source = SS.str().c_str(); //в эту переменную нужно записать текст, который в дальнейшем поместится в буфер обмена
					//запись текста в буфер обмена
	if (OpenClipboard(NULL))//открываем буфер обмена
	{
		HGLOBAL hgBuffer;
		char* chBuffer;
		EmptyClipboard(); //очищаем буфер
		hgBuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength() + 1);//выделяем память
		chBuffer = (char*)GlobalLock(hgBuffer); //блокируем память
		strcpy(chBuffer, LPCSTR(source));
		GlobalUnlock(hgBuffer);//разблокируем память
		SetClipboardData(CF_TEXT, hgBuffer);//помещаем текст в буфер обмена
		CloseClipboard(); //закрываем буфер обмена
	}
	Sleep(500);
	//system("pause");
	return 0;
}