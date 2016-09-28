#include "CCapacity.h"

#ifndef CCONVERT
#define CCONVERT

class CConvert
{
public:
	static double ByteToKB(__int64 bytes);
	static double ByteToMB(__int64 bytes);
	static double ByteToGB(__int64 bytes);
	static double ByteToTB(__int64 bytes);
	static CCapacity ConvertByte(__int64 bytes);
};

#endif
