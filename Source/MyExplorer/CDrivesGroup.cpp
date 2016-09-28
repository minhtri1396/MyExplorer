#include "stdafx.h"
#include "CDrivesGroup.h"
#include "CText.h"
#include "CConvert.h"
#include <shellapi.h>
#include <Windows.h>


__int64 CDrivesGroup::GetTotalSize(wchar_t *label)
{
	wchar_t drive[4]; // Ex: A:\ + '\0' = 4
	wcscpy(drive, label);
	wcscat(drive, L":\\");

	__int64 nSize;
	if (GetDriveType(drive) == DRIVE_CDROM)
	{
		nSize = 0;
	}
	else
	{
		SHGetDiskFreeSpaceEx(drive, NULL, (PULARGE_INTEGER)&nSize, NULL);
	}

	return nSize;
}

__int64 CDrivesGroup::GetFreeSize(wchar_t *label)
{
	wchar_t drive[4]; // Ex: A:\ + '\0' = 4
	wcscpy(drive, label);
	wcscat(drive, L":\\");

	__int64 nFreeSpace;
	if (GetDriveType(drive) == DRIVE_CDROM)
	{
		nFreeSpace = 0;
	}
	else
	{
		GetDiskFreeSpaceEx(drive, NULL, NULL, (PULARGE_INTEGER)&nFreeSpace);
	}

	return nFreeSpace;
}

wchar_t *CDrivesGroup::GetVolumn(wchar_t *label)
{
	wchar_t drive[4]; // Ex: A:\ + '\0' = 4
	wcscpy(drive, label);
	wcscat(drive, L":\\");

	wchar_t *volumne = new wchar_t[51];
	volumne[0] = 0;
	GetVolumeInformation(drive, volumne, 50, NULL, NULL, NULL, NULL, 0);

	if (wcslen(volumne) == 0) //not have label => Local Disk
	{
		if (GetDriveType(drive) == DRIVE_CDROM)
		{
			wcscpy(volumne, L"CD Rom");
		}
		else
		{
			wcscpy(volumne, L"Local Disk");
		}
	}

	return volumne;
}

wchar_t *CDrivesGroup::GetType(wchar_t *label)
{
	__int8 iType = GetDriveType(label);
	switch (iType)
	{
	case DRIVE_FIXED:
		return wcsdup(L"Local Disk");
	case DRIVE_REMOVABLE:
		return wcsdup(L"Removable Disk");
	case DRIVE_CDROM:
		return wcsdup(L"CD Rom");
	default:
		return wcsdup(L"Unknown");
	}	
}

void CDrivesGroup::SetNumberDrive(__int32 _markDrives)
{
	m_nDrive = 0;
	while (_markDrives)
	{
		if (_markDrives & 0x00000001)
		{
			++m_nDrive;
		}
		_markDrives >>= 1;
	}
}

void CDrivesGroup::SetLabelDrives(__int32 _markDrives)
{
	__int8 nextLabel = 0;
	wchar_t wch[2];
	wch[1] = 0; //NULL

	__int8 nLabel = 0;
	m_pszLabelsGroup = new wchar_t *[m_nDrive];

	while (_markDrives)
	{
		if (_markDrives & 0x00000001)
		{
			m_pszLabelsGroup[nLabel] = new wchar_t [2]; //Ex: C + '\0' = 2
			wch[0] = 'A' + nextLabel;

			wcscpy(m_pszLabelsGroup[nLabel], wch);
			++nLabel;
		}
		_markDrives >>= 1;
		++nextLabel;
	}
}

void CDrivesGroup::SetDrives()
{
	__int32 markDrives = GetLogicalDrives();

	//Get number of drive
	SetNumberDrive(markDrives);
	//Get drive label (A, B, C, ...)
	SetLabelDrives(markDrives);
	//Get volumne
	wchar_t *type, *volumn, label[4];
	CCapacity freeSpace, totalSpace;
	m_pDrive = new CDrive *[m_nDrive];
	for (__int8 i = 0; i < m_nDrive; ++i)
	{
		volumn = GetVolumn(m_pszLabelsGroup[i]);
		wcscat(volumn, L" (");
		wcscat(volumn, m_pszLabelsGroup[i]);
		wcscat(volumn, L":)");

		wcscpy(label, m_pszLabelsGroup[i]);
		wcscat(label, L":\\");

		type = GetType(label);
		
		//Get size of spaces
		freeSpace = CConvert::ConvertByte(GetFreeSize(m_pszLabelsGroup[i]));
		totalSpace = CConvert::ConvertByte(GetTotalSize(m_pszLabelsGroup[i]));
		//Store drive informtion
		m_pDrive[i] = new CDrive();
		m_pDrive[i]->Init(label, volumn, type, freeSpace, totalSpace);
		delete[]volumn; delete[]type;
	}
}

CDrivesGroup::CDrivesGroup()
{
	SetDrives();
}

CDrivesGroup::CDrivesGroup(const CDrivesGroup &_other)
{
	m_nDrive = _other.m_nDrive;

	m_pDrive = new CDrive *[m_nDrive];
	for (__int8 i = 0; i < m_nDrive; ++i)
	{
		m_pDrive[i] = new CDrive(*_other.m_pDrive[i]);
	}
}

CDrivesGroup::~CDrivesGroup()
{

	for (__int8 i = 0; i < m_nDrive; ++i)
	{
		delete[]m_pszLabelsGroup[i];
	}
	delete[]m_pszLabelsGroup;

	if (m_pDrive)
	{
		for (__int8 i = 0; i < m_nDrive; ++i)
		{
			delete m_pDrive[i];
		}

		delete[]m_pDrive;
	}
}

__int8 CDrivesGroup::Size()
{
	return m_nDrive;
}

CDrive *CDrivesGroup::operator[] (__int8 _iIndex)
{
	if (_iIndex < 0 || _iIndex > m_nDrive)
	{
		return NULL;
	}

	return m_pDrive[_iIndex];
}
