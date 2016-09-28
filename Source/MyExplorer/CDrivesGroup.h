#include "CDrive.h"

#ifndef CDRIVESGROUP
#define CDRIVESGROUP

class CDrivesGroup
{
private:
	__int8 m_nDrive;
	wchar_t **m_pszLabelsGroup;
	CDrive **m_pDrive;

	__int64 GetTotalSize(wchar_t *label);
	__int64 GetFreeSize(wchar_t *label);
	wchar_t *GetVolumn(wchar_t *label);
	wchar_t *GetType(wchar_t *label);

	void SetNumberDrive(__int32 _markDrives);
	void SetLabelDrives(__int32 _markDrives);
	void SetDrives();
public:
	CDrivesGroup();
	CDrivesGroup(const CDrivesGroup &_other);
	~CDrivesGroup();
	__int8 Size();
	CDrive *operator[] (__int8 _iIndex);
};

#endif