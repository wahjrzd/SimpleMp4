#pragma once
#include <stdint.h>
#include <stdio.h>

class AtomMDHD;

struct SampleInfo
{
    bool isKeyFrame;
    uint32_t SampleSize;
    uint32_t TimeStamp;
    uint32_t Position;
    SampleInfo()
    {
        isKeyFrame = false;
        SampleSize = 0;
        TimeStamp = 0;
        Position = 0;
    }
};

struct STSCInfo
{
    uint32_t chunkIndex;
    uint32_t sampleCount;
};

class AtomTKHD
{
public:
	AtomTKHD(unsigned char v, FILE* f);
	~AtomTKHD();

    uint64_t CreationTime;//创建时间（相对于UTC时间1904-01-01零点的秒数）
    uint64_t ModificationTime;
    uint32_t TrackId; //唯一标识该Track的一个非零值
    uint32_t Reserved1;
    uint64_t Duration;//该Track的播放时长，需要和时间戳进行一起计算，然后得到播放的时间坐标。
    uint32_t Reserved2[2];
    uint16_t Layer;
    uint16_t AlternateGroup;
    uint16_t Volume;//播放此track的音量，1.0为正常音量
    uint16_t Reserved3;
    int32_t Matrix[9];
    uint32_t Width; //如果该Track为Video Track，则表示图像的宽度（16.16浮点表示）
    uint32_t Height;

    bool isVideo = true;

    AtomMDHD* mdhd = nullptr;

    uint32_t curSampleIndex = 0;
    uint32_t TotalSampleCount = 0;//如果是视频表示有多少帧
    SampleInfo* samples = nullptr;

    STSCInfo* stsc = nullptr;
    uint32_t count1 = 0;//stsc中获取到chunk数量
};

