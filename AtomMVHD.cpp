#include "AtomMVHD.h"
#ifdef _WIN32
#include <WinSock2.h>
#else

#endif 

AtomMVHD::AtomMVHD(unsigned char v, FILE* f)
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
	fread(&Rate, sizeof(uint32_t), 1, f);
	Rate = ntohl(Rate);
	fread(&Volume, sizeof(uint16_t), 1, f);
	Rate = ntohs(Volume);
	fread(Reserved1, 1, sizeof(Reserved1), f);
	fread(Matrix, 1, sizeof(Matrix), f);
	fread(Predefined, 1, sizeof(Predefined), f);
	fread(&NextTrackId, sizeof(uint32_t), 1, f);
	NextTrackId = ntohl(NextTrackId);
}

AtomMVHD::~AtomMVHD()
{

}
