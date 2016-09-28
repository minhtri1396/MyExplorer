#include <list>
#include "CFile.h"

#ifndef CEXPLORER
#define CEXPLORER

class CExplorer
{
private:
	static DWORD GetSize(const WIN32_FIND_DATA &fd);
	static wchar_t* GetType(const WIN32_FIND_DATA &fd);
	static wchar_t* GetDateModified(const WIN32_FIND_DATA &fd);
public:
	static std::list<CFile> GetListDirectory(wchar_t *path);
	static std::list<CFile> GetListDirectoryAndFile(wchar_t *path);
};

#endif