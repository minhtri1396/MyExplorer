#include "stdafx.h"
#include "CText.h"
#include <math.h>
#include <Windows.h>

wchar_t *CText::NumberToWCS(double number, short nDecimalPlaces)
{
	LPWSTR lpwTemp = new WCHAR[20];
	LPWSTR lpwStr = new WCHAR[20];
	lpwStr[0] = 0;
	if (number < 0) wcscat(lpwStr, L"-");
	//Get integer place (lpwTemp saves integer place of number now)
	double integer = floor(number);
	if (integer != 0) {
		INT numberOfDigit = (INT)log10(integer) + 1;
		lpwTemp[numberOfDigit] = 0;
		WCHAR wcDigit[2];
		while (numberOfDigit > 0)
		{
			INT digit = INT(integer - 10 * floor(integer / 10));
			_itow(digit, wcDigit, 10);
			lpwTemp[--numberOfDigit] = wcDigit[0];
			integer /= 10;
		}
	}
	else
	{
		lpwTemp[0] = '0', lpwTemp[1] = 0;
	}
	wcscat(lpwStr, lpwTemp);
	//Get decimal place (lpwTemp saves decimal place of number now)
	integer = floor(number);
	double decimal = (number - integer) * pow(10, nDecimalPlaces); //get n decimal places
	if (decimal != 0) //it has decimal place
	{
		wcscat(lpwStr, L"."); //decimal point
		wcscat(lpwStr, _itow(decimal, lpwTemp, 10));
	}
	delete[]lpwTemp;

	return lpwStr;
}