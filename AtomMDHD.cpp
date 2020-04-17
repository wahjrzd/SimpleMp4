#include "AtomMDHD.h"
#ifdef _WIN32
#include <WinSock2.h>
#else

#endif 

AtomMDHD::AtomMDHD(unsigned char v, FILE* f)
{
	if (v == 0)
	{
		fread(&CreationTime, sizeof(uint32_t), 1, f);
		CreationTime = ntohl(CreationTime);
		fread(&ModificationTime, sizeof(uint32_t), 1, f);
		ModificationTime = ntohl(ModificationTime);
		fread(&TimeScale, sizeof(uint32_t), 1, f);
		TimeScale = ntohl(TimeScale);
		fread(&Duration, sizeof(uint32_t), 1, f);
		Duration = ntohl(Duration);
	}
	else
	{

	}
	fread(Language, 4, 1, f);
}

AtomMDHD::~AtomMDHD()
{

}
