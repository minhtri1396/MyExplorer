#include "CCapacity.h"

#ifndef CFILE
#define CFILE

class CFile
{
private:
	wchar_t *m_path;
	wchar_t *m_name;
	wchar_t m_dateModified[20]; //20/12/2016 20:50 PM/0 => 20 characters
	wchar_t *m_type;
	CCapacity m_capacity;
public:
	wchar_t *PATH();
	wchar_t *NAME();
	wchar_t *DATEMODIFIED();
	wchar_t *TYPE();
	CCapacity CAPACITY();
	CFile();
	CFile(const CFile &other);
	~CFile();
	void Init(wchar_t *_path, wchar_t *_name, wchar_t *_dateModified, wchar_t *_type, const CCapacity &_capacity);
};

#endif