#pragma once
#include <string>

extern char BoxFTYP[4];

extern char BoxFREE[4];
extern char BoxMDAT[4];

extern char BoxMOOV[4];
extern char BoxMVHD[4];

extern char BoxTRAK[4];
extern char BoxTKHD[4];

extern char BoxEDTS[4];
extern char BoxELST[4];

extern char BoxMDIA[4];
extern char BoxMDHD[4];
extern char BoxHDLR[4];

extern char BoxMINF[4];
extern char BoxVMHD[4];
extern char BoxSMHD[4];
extern char BoxDINF[4];
extern char BoxDREF[4];
extern char BoxSTBL[4];

extern char BoxSTSD[4];
extern char BoxSTTS[4];
extern char BoxSTSS[4];
extern char BoxSTSC[4];
extern char BoxSTSZ[4];
extern char BoxSTCO[4];
extern char BoxCO64[4];

extern char BoxUDTA[4];
extern char BoxMETA[4];

class AtomMVHD;
class AtomTKHD;
/*
 * ftyp
 * moov
 *  mvhd
 *	trak
 *		tkhd
 *		mdia
 *          --
 *  trak
 *      tkhd
 *      mdia
 *          mdhd
 *          hdlr
 *          minf
 *              smhd
 *              dinf
 *                  dref
 *                      url
 *                  stbl
 *  udta
 * mdat
 * free
*/
class MP4Demuxer
{
public:
	MP4Demuxer();
	~MP4Demuxer();

	int OpenFile(const std::string& fileName);

    int GetOneFrame(uint8_t** data, uint32_t* sz, uint32_t* ts, bool* isKeyFrame, bool* isVideo);

    int GetNextFrame(uint8_t** data, uint32_t* sz, uint32_t* ts, bool* isKeyFrame);

	int GetNextAudioFrame(uint8_t** data, uint32_t* sz, uint32_t* ts);
    /*
     * @param ts unit millisecond
    */
    int Seek(uint32_t ts);

    void WriteTag(FILE* fPtr);
private:
	int Parse();

private:
	FILE* m_fileHandle = nullptr;
    AtomMVHD* mvhd = nullptr;
	AtomTKHD* tkhd[2] = { nullptr };
	int trackID = 0;

    unsigned char* vps = nullptr;
    uint16_t vpsLen = 0;
    unsigned char* sps = nullptr;
    uint16_t spsLen = 0;
    unsigned char* pps = nullptr;
    uint16_t ppsLen = 0;

    uint8_t* mediaData = nullptr;
    uint32_t mediaSize = 0;
};

