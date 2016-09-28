#include "CCapacity.h"

#ifndef CDRIVE
#define CDRIVE

class CDrive
{
private:
	wchar_t *m_lpwLabel;
	wchar_t *m_lpwName;
	wchar_t *m_lpwType;
	CCapacity m_freeSpace;
	CCapacity m_totalSpace;

public:
	wchar_t *LABEL();
	wchar_t *NAME();
	wchar_t *TYPE();
	CCapacity FreeSpace();
	CCapacity TotalSpace();

	CDrive();
	CDrive(const CDrive &_other);
	void Init(wchar_t *_lpwLabel, wchar_t *_lpwName, wchar_t *_lpwType,const CCapacity &_freeSpace, const CCapacity &_totalSpace);
	~CDrive();
};

#endif