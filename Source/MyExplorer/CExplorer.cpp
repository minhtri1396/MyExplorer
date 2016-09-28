#include "stdafx.h"
#include "CExplorer.h"
#include "CConvert.h"
#include "CText.h"
#include <Windows.h>

#include <shlobj.h> //Shell object

//Dùng để sử dụng hàm StrCpy, StrNCat
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

DWORD CExplorer::GetSize(const WIN32_FIND_DATA &fd)
{
	return fd.nFileSizeLow;
}

#define ENTIRE_STRING NULL



wchar_t* CExplorer::GetType(const WIN32_FIND_DATA &fd)
{
	//Xác định vị trí phần mở rộng (Ex: .exe)
	int nDotPos = StrRStrI(fd.cFileName, ENTIRE_STRING, _T(".")) - fd.cFileName;
	int len = wcslen(fd.cFileName);
	if (nDotPos < 0 || nDotPos >= len) //tập tin không có dấu . (tức là không có phần mở rộng)
	{
		return L"Tập tin";
	}
	//Lấy tên phần mở rộng (phần sau dấu . cuối cùng)
	TCHAR *szExtension = new TCHAR[len - nDotPos + 1];
	for (int i = nDotPos; i < len; ++i) 
	{
		szExtension[i - nDotPos] = fd.cFileName[i];
	}
	szExtension[len - nDotPos] = NULL;
	//Xác định loại dữ liệu từ phần mở rộng
	if (!StrCmpI(szExtension, _T(".htm")) || !StrCmpI(szExtension, _T(".html")))
	{
		delete[]szExtension;
		return L"Web page";
	}

	TCHAR pszOut[256];
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 256;

	//Kiếm handle của phần mở rộng tương ứng trong registry
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, szExtension, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		delete[]szExtension;
		return L"Tập tin";
	}

	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE)pszOut, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return L"Tập tin";
	}
	RegCloseKey(hKey);

	//Kiếm mô tả về thông tin của phần mở rộng thông qua handle của key tương ứng trong registry
	wchar_t pszPath[1001];
	dwSize = 1000;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, pszOut, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return L"Tập tin";
	}
	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE)pszPath, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return L"Tập tin";
	}
	RegCloseKey(hKey);

	return pszPath;
}

wchar_t* CExplorer::GetDateModified(const WIN32_FIND_DATA &fd)
{
	//Convert to local time
	SYSTEMTIME stUTC, stLocal;
	FileTimeToSystemTime(&fd.ftLastWriteTime, &stUTC);

	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	wchar_t *lpwDateModified = new wchar_t[20]; //20/12/2016 20:50 PM/0 => 20 characters
	wsprintf(lpwDateModified, _T("%02d/%02d/%04d %02d:%02d %s"),
		stLocal.wDay, stLocal.wMonth, stLocal.wYear,
		(stLocal.wHour > 12) ? (stLocal.wHour - 12) : (stLocal.wHour),
		stLocal.wMinute,
		(stLocal.wHour > 12) ? (_T("PM")) : (_T("AM")));

	return lpwDateModified;
}

std::list<CFile> CExplorer::GetListDirectory(wchar_t *_path)
{
	std::list<CFile> directories;

	wchar_t *path, *pathAll;
	path = new wchar_t[wcslen(_path) + 2]; // 1 = '\0' + 1 = '\\' (nếu chưa có)
	pathAll = new wchar_t[wcslen(_path) + 3]; // 1 = '\0' + 1 = '\\' (nếu chưa có) + 1 = '*'

	wcscpy(path, _path);
	wcscpy(pathAll, _path);

	if (_path[wcslen(_path) - 1] != '\\')
	{
		wcscat(path, L"\\");
		wcscat(pathAll, L"\\");
	}
	wcscat(pathAll, L"*");

	WIN32_FIND_DATA fd;
	HANDLE hFile = FindFirstFileW(pathAll, &fd);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		BOOL bFound = 1;
		CFile file;
		wchar_t *folderPath, *dateModified;
		while (bFound)
		{
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& ((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN)
				&& (wcscmp(fd.cFileName, L".") != 0) && (wcscmp(fd.cFileName, L"..") != 0))
			{
				folderPath = new wchar_t[wcslen(path) + wcslen(fd.cFileName) + 2];
				wcscpy(folderPath, path);
				wcscat(folderPath, fd.cFileName);
				dateModified = GetDateModified(fd);
				file.Init(folderPath, fd.cFileName, dateModified, L"Thư mục", CConvert::ConvertByte(GetSize(fd)));
				directories.push_back(file);
				delete[]folderPath; delete[]dateModified;
			}

			bFound = FindNextFileW(hFile, &fd);
		}
	}

	return directories;
}

std::list<CFile> CExplorer::GetListDirectoryAndFile(wchar_t *_path)
{
	std::list<CFile> directories;

	wchar_t *path, *pathAll;
	path = new wchar_t[wcslen(_path) + 2]; // 1 = '\0' + 1 = '\\' (nếu chưa có)
	pathAll = new wchar_t[wcslen(_path) + 3]; // 1 = '\0' + 1 = '\\' (nếu chưa có) + 1 = '*'

	wcscpy(path, _path);
	wcscpy(pathAll, _path);

	if (_path[wcslen(_path) - 1] != '\\')
	{
		wcscat(path, L"\\");
		wcscat(pathAll, L"\\");
	}
	wcscat(pathAll, L"*");

	WIN32_FIND_DATA fd;
	HANDLE hFile = FindFirstFileW(pathAll, &fd);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		BOOL bFound = 1;
		CFile file;
		wchar_t *folderPath, *dateModified;
		while (bFound)
		{
			if (((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN)
				&& (wcscmp(fd.cFileName, L".") != 0) && (wcscmp(fd.cFileName, L"..") != 0))
			{
				folderPath = new wchar_t[wcslen(path) + wcslen(fd.cFileName) + 2];
				wcscpy(folderPath, path);
				wcscat(folderPath, fd.cFileName);
				dateModified = GetDateModified(fd);
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					file.Init(folderPath, fd.cFileName, dateModified, L"Thư mục", CConvert::ConvertByte(GetSize(fd)));
				}
				else
				{
					file.Init(folderPath, fd.cFileName, dateModified, GetType(fd), CConvert::ConvertByte(GetSize(fd)));
				}
				directories.push_back(file);
				delete[]folderPath; delete[]dateModified;
			}

			bFound = FindNextFileW(hFile, &fd);
		}
	}

	return directories;
}