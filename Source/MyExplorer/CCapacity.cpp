#include "stdafx.h"
#include "CCapacity.h"
#include <math.h>

wchar_t *CCapacity::TYPE()
{
	return m_type;
}

double CCapacity::SIZE()
{
	return m_size;
}

__int64 CCapacity::ROUNDSIZE()
{
	return floor(m_size + 0.5);
}

CCapacity::CCapacity()
{
	m_type[0] = 0;
	m_size = 0;
}

CCapacity::CCapacity(const CCapacity& other)
{
	wcscpy(m_type, other.m_type);
	m_size = other.m_size;
}

bool CCapacity::Init(wchar_t *_type, double _size)
{
	if (wcslen(_type) > 2)
	{
		return false;
	}
	wcscpy(m_type, _type);

	m_size = _size;

	return true;
}

void CCapacity::operator=(const CCapacity& other)
{
	wcscpy(m_type, other.m_type);
	m_size = other.m_size;
}
