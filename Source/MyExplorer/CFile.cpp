#include "stdafx.h"
#include "CFile.h"

wchar_t *CFile::PATH()
{
	return m_path;
}

wchar_t *CFile::NAME()
{
	return &m_name[0];
}

wchar_t *CFile::DATEMODIFIED()
{
	return m_dateModified;
}

wchar_t *CFile::TYPE()
{
	return &m_type[0];
}

CCapacity CFile::CAPACITY()
{
	return m_capacity;
}

CFile::CFile()
{
	m_path = m_name = m_type = 0;
	m_dateModified[0] = 0;
}

CFile::CFile(const CFile &other)
{
	m_path = wcsdup(other.m_path);
	m_name = wcsdup(other.m_name);
	m_type = wcsdup(other.m_type);
	wcscpy(m_dateModified, other.m_dateModified);
	m_capacity = other.m_capacity;
}

CFile::~CFile()
{
	delete[]m_path;
	delete[]m_name;
	delete[]m_type;
}

void CFile::Init(wchar_t *_path, wchar_t *_name, wchar_t *_dateModified, wchar_t *_type, const CCapacity &_capacity)
{
	if (m_path != 0)
	{
		delete[]m_path;
	}
	if (m_name != 0)
	{
		delete[]m_name;
	}
	if (m_type != 0)
	{
		delete[]m_type;
	}
	m_path = wcsdup(_path);
	m_name = wcsdup(_name);
	m_type = wcsdup(_type);
	wcscpy(m_dateModified, _dateModified);
	m_capacity = _capacity;
}
