#pragma once
#include <stdint.h>
#include <stdio.h>

class AtomMDHD
{
public:
	AtomMDHD(unsigned char v, FILE* f);
	~AtomMDHD();

    uint64_t   CreationTime;
    uint64_t   ModificationTime;
    uint32_t   TimeScale;
    uint64_t   Duration;
    char Language[4];
};

