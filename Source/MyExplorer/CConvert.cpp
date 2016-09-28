#include "stdafx.h"
#include "CConvert.h"
#include "CText.h"

extern __int64 MinKB = 1024;
extern __int64 MinMB = 1048576;
extern __int64 MinGB = 1073741824;

double CConvert::ByteToKB(__int64 bytes)
{
	return bytes / 1024.0;
}

double CConvert::ByteToMB(__int64 bytes)
{
	return ByteToKB(bytes) / 1024;
}

double CConvert::ByteToGB(__int64 bytes)
{
	return ByteToMB(bytes) / 1024;
}

double CConvert::ByteToTB(__int64 bytes)
{
	return ByteToGB(bytes) / 1024;
}

CCapacity CConvert::ConvertByte(__int64 bytes)
{
	CCapacity spaceCapacity;
	if (bytes > MinGB)
	{
		double resTmp = ByteToGB(bytes);
		resTmp < 1024 ? spaceCapacity.Init(L"GB", resTmp) : spaceCapacity.Init(L"TB", resTmp / 1024);
	}
	else if (bytes > MinMB)
	{
		spaceCapacity.Init(L"MB", ByteToMB(bytes));
	}
	else if (bytes > MinKB)
	{
		spaceCapacity.Init(L"KB", ByteToKB(bytes));
	}
	else
	{
		spaceCapacity.Init(L"B", bytes);
	}
	return spaceCapacity;
}
