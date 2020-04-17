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

    uint64_t CreationTime;//����ʱ�䣨�����UTCʱ��1904-01-01����������
    uint64_t ModificationTime;
    uint32_t TrackId; //Ψһ��ʶ��Track��һ������ֵ
    uint32_t Reserved1;
    uint64_t Duration;//��Track�Ĳ���ʱ������Ҫ��ʱ�������һ����㣬Ȼ��õ����ŵ�ʱ�����ꡣ
    uint32_t Reserved2[2];
    uint16_t Layer;
    uint16_t AlternateGroup;
    uint16_t Volume;//���Ŵ�track��������1.0Ϊ��������
    uint16_t Reserved3;
    int32_t Matrix[9];
    uint32_t Width; //�����TrackΪVideo Track�����ʾͼ��Ŀ�ȣ�16.16�����ʾ��
    uint32_t Height;

    bool isVideo = true;

    AtomMDHD* mdhd = nullptr;

    uint32_t curSampleIndex = 0;
    uint32_t TotalSampleCount = 0;//�������Ƶ��ʾ�ж���֡
    SampleInfo* samples = nullptr;

    STSCInfo* stsc = nullptr;
    uint32_t count1 = 0;//stsc�л�ȡ��chunk����
};

