#include "AtomTKHD.h"
#ifdef _WIN32
#include <WinSock2.h>
#else

#endif 
#include "AtomMDHD.h"

AtomTKHD::AtomTKHD(unsigned char v, FILE* f)
{
	if (v == 0)
	{
		fread(&CreationTime, sizeof(uint32_t), 1, f);
		CreationTime = ntohl(CreationTime);
		fread(&ModificationTime, sizeof(uint32_t), 1, f);
		ModificationTime = ntohl(ModificationTime);
		fread(&TrackId, sizeof(uint32_t), 1, f);
		TrackId = ntohl(TrackId);
		fread(&Reserved1, sizeof(uint32_t), 1, f);
		Reserved1 = ntohl(Reserved1);
		fread(&Duration, sizeof(uint32_t), 1, f);
		Duration = ntohl(Duration);
	}
	else
	{

	}
	fread(Reserved2, 1, sizeof(Reserved2), f);
	fread(&Layer, sizeof(uint16_t), 1, f);
	Layer = ntohs(Layer);
	fread(&AlternateGroup, sizeof(uint16_t), 1, f);
	AlternateGroup = ntohs(AlternateGroup);
	fread(&Volume, sizeof(uint16_t), 1, f);
	Volume = ntohs(Volume);
	fread(&Reserved3, sizeof(uint16_t), 1, f);
	
	fread(Matrix, 1, 36, f);
	fread(&Width, sizeof(uint32_t), 1, f);
	Width = ntohs(Width & 0xFFFF);
	fread(&Height, sizeof(uint32_t), 1, f);
	Height = ntohs(Height & 0xFFFF);

	if (Width == 0 || Height == 0)
		isVideo = false;
}

AtomTKHD::~AtomTKHD()
{
	delete mdhd;
	delete[] samples;
	delete[] stsc;
}
