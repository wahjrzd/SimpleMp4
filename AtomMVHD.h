#pragma once
#include <stdint.h>
#include <stdio.h>

class AtomMVHD
{
public:
    AtomMVHD(unsigned char v, FILE* f);
    ~AtomMVHD();

    uint64_t CreationTime;//创建时间（相对于UTC时间1904-01-01零点的秒数）
    uint64_t ModificationTime;//修改时间（相对于UTC时间1904-01-01零点的秒数）
    uint32_t TimeScale;//
    uint64_t Duration;//该影片的播放时长，该值除以time scale字段即可以得到该影片的总时长单位秒s
    uint32_t Rate;//推荐播放速率
    uint16_t Volume;//与Rate类似，[8.8] 格式，1.0（0x0100）表示最大音量
    uint8_t Reserved1[10];//
    uint32_t Matrix[9];//
    uint8_t Predefined[24];//
    int32_t NextTrackId;//下一个Track使用的id号，通过该值减去1可以判断当前文件的Track数量
};

