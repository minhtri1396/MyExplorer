#ifndef CCAPACITY
#define CCAPACITY

class CCapacity
{
private:
	wchar_t m_type[3]; //B, KB, MB, GB or TB
	double m_size;
public:
	wchar_t *TYPE();
	double SIZE();
	__int64 ROUNDSIZE();

	CCapacity();
	CCapacity(const CCapacity& other);
	bool Init(wchar_t *_type, double _size);
	void operator=(const CCapacity& other);
};

#endif