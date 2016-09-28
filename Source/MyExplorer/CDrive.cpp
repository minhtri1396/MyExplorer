#include "stdafx.h"
#include "CDrive.h"

wchar_t *CDrive::LABEL()
{
	return &m_lpwLabel[0];
}

wchar_t *CDrive::NAME()
{
	return &m_lpwName[0];
}

wchar_t *CDrive::TYPE()
{
	return &m_lpwType[0];
}

CCapacity CDrive::FreeSpace()
{
	return m_freeSpace;
}

CCapacity CDrive::TotalSpace()
{
	return m_totalSpace;
}

CDrive::CDrive()
{
	m_lpwLabel = NULL;
	m_lpwName = NULL;
	m_lpwType = NULL;
}

CDrive::CDrive(const CDrive &_other)
{
	m_lpwLabel = _wcsdup(_other.m_lpwLabel);
	m_lpwName = _wcsdup(_other.m_lpwName);
	m_lpwType = _wcsdup(_other.m_lpwType);
	m_freeSpace = _other.m_freeSpace;
	m_totalSpace = _other.m_totalSpace;
}

void CDrive::Init(wchar_t *_lpwLabel,wchar_t *_lpwName, wchar_t *_lpwType, const CCapacity &_freeSpace, const CCapacity &_totalSpace)
{
	if (m_lpwLabel != 0)
	{
		delete[]m_lpwLabel;
	}
	if (m_lpwName != 0)
	{
		delete[]m_lpwName;
	}
	if (m_lpwType != 0)
	{
		delete[]m_lpwType;
	}
	m_lpwLabel = wcsdup(_lpwLabel);
	m_lpwName = wcsdup(_lpwName);
	m_lpwType = wcsdup(_lpwType);
	m_freeSpace = _freeSpace;
	m_totalSpace = _totalSpace;
}

CDrive::~CDrive()
{
	delete[]m_lpwLabel;
	delete[]m_lpwName;
	delete[]m_lpwType;
	m_lpwLabel = NULL;
	m_lpwName = NULL;
	m_lpwType = NULL;
}
