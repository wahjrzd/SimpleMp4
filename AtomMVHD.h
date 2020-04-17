#pragma once
#include <stdint.h>
#include <stdio.h>

class AtomMVHD
{
public:
    AtomMVHD(unsigned char v, FILE* f);
    ~AtomMVHD();

    uint64_t CreationTime;//����ʱ�䣨�����UTCʱ��1904-01-01����������
    uint64_t ModificationTime;//�޸�ʱ�䣨�����UTCʱ��1904-01-01����������
    uint32_t TimeScale;//
    uint64_t Duration;//��ӰƬ�Ĳ���ʱ������ֵ����time scale�ֶμ����Եõ���ӰƬ����ʱ����λ��s
    uint32_t Rate;//�Ƽ���������
    uint16_t Volume;//��Rate���ƣ�[8.8] ��ʽ��1.0��0x0100����ʾ�������
    uint8_t Reserved1[10];//
    uint32_t Matrix[9];//
    uint8_t Predefined[24];//
    int32_t NextTrackId;//��һ��Trackʹ�õ�id�ţ�ͨ����ֵ��ȥ1�����жϵ�ǰ�ļ���Track����
};

