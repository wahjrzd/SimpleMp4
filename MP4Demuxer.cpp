#include "MP4Demuxer.h"
#include <iostream>
#ifdef _WIN32
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif 
#include "AtomMVHD.h"
#include "AtomTKHD.h"
#include "AtomMDHD.h"

char BoxFTYP[4] = { 'f','t','y','p' };
char BoxFREE[4] = { 'f','r','e','e' };
char BoxMDAT[4] = { 'm','d','a','t' };

char BoxMOOV[4] = { 'm','o','o','v' };
char BoxMVHD[4] = { 'm','v','h','d' };

char BoxTRAK[4] = { 't','r','a','k' };
char BoxTKHD[4] = { 't','k','h','d' };

char BoxEDTS[4] = { 'e','d','t','s' };
char BoxELST[4] = { 'e','l','s','t' };

char BoxMDIA[4] = { 'm','d','i','a' };
char BoxMDHD[4] = { 'm','d','h','d' };
char BoxHDLR[4] = { 'h','d','l','r' };

char BoxMINF[4] = { 'm','i','n','f' };
char BoxVMHD[4] = { 'v','m','h','d' };
char BoxSMHD[4] = { 's','m','h','d' };
char BoxDINF[4] = { 'd','i','n','f' };
char BoxDREF[4] = { 'd','r','e','f' };
char BoxSTBL[4] = { 's','t','b','l' };

char BoxSTSD[4] = { 's','t','s','d' };
char BoxSTTS[4] = { 's','t','t','s' };
char BoxSTSS[4] = { 's','t','s','s' };
char BoxSTSC[4] = { 's','t','s','c' };
char BoxSTSZ[4] = { 's','t','s','z' };
char BoxSTCO[4] = { 's','t','c','o' };
char BoxCO64[4] = { 'c','o','6','4' };

char BoxUDTA[4] = { 'u','d','t','a' };
char BoxMETA[4] = { 'm','e','t','a' };

MP4Demuxer::MP4Demuxer()
{

}

MP4Demuxer::~MP4Demuxer()
{
	if (m_fileHandle)
		fclose(m_fileHandle);
	delete mvhd;
	delete tkhd[0];
	delete tkhd[1];
	delete[] sps;
	delete[] pps;
	delete[] mediaData;
}

int MP4Demuxer::OpenFile(const std::string& fileName)
{
#ifdef _WIN32
	fopen_s(&m_fileHandle, fileName.c_str(), "rb");
#else
	m_fileHandle = fopen(fileName.c_str(), "rb");
#endif
	if (!m_fileHandle)
		return 1;
	//检测文件头
	unsigned char boxHeader[8];
	fread(boxHeader, 1, 8, m_fileHandle);
	if (memcmp(boxHeader + 4, BoxFTYP, 4) != 0)
	{
		std::cout << "not a mp4 file" << std::endl;
		return 1;
	}
	int boxLength = (boxHeader[0] << 24) | (boxHeader[1] << 16) | (boxHeader[2] << 8) | boxHeader[3];
	fseek(m_fileHandle, boxLength - 8, SEEK_CUR);
	/*char* boxBody = new char[boxLength - 8];
	fread(boxBody, 1, boxLength - 8, m_fileHandle);
	int v = (boxBody[4] << 24) | (boxBody[5] << 16) | (boxBody[6] << 8) | boxBody[7];
	std::cout << "Major Brand:" << std::string(boxBody, 4) << ", Version:" << v << std::endl;
	delete[] boxBody;*/
	return Parse();
}

int MP4Demuxer::Parse()
{
	while (!feof(m_fileHandle))
	{
		unsigned char boxHeader[8];
		if (fread(boxHeader, 1, 8, m_fileHandle) != 8)
			break;
		int boxDataSize = ((boxHeader[0] << 24) | (boxHeader[1] << 16) | (boxHeader[2] << 8) | boxHeader[3]) - 8;
		if (boxDataSize >= 8 && boxDataSize <= INT32_MAX)
		{
			//TODO
		}
		else
		{
			//TODO
		}

		if (memcmp(boxHeader + 4, BoxFREE, 4) == 0)//不用处理
		{
			std::cout << "find free" << std::endl;
			fseek(m_fileHandle, boxDataSize, SEEK_CUR);
		}
		else if (memcmp(boxHeader + 4, BoxMDAT, 4) == 0)//媒体数据暂时跳过
		{
			std::cout << "find mdat" << std::endl;
			fseek(m_fileHandle, boxDataSize, SEEK_CUR);
		}
		else if (memcmp(boxHeader + 4, BoxMOOV, 4) == 0)
		{
			std::cout << "find moov" << std::endl;
			Parse();
		}
		else if (memcmp(boxHeader + 4, BoxMVHD, 4) == 0)//movie header
		{
			unsigned char fullBox[4];
			fread(fullBox, 1, 4, m_fileHandle);
			mvhd = new AtomMVHD(fullBox[0], m_fileHandle);
			std::cout << "find mvhd" << std::endl;
		}
		else if (memcmp(boxHeader + 4, BoxTRAK, 4) == 0)//track
		{
			std::cout << "find trak" << std::endl;
			Parse();
		}
		else if (memcmp(boxHeader + 4, BoxTKHD, 4) == 0)
		{
			unsigned char fullBox[4];
			fread(fullBox, 1, 4, m_fileHandle);
			tkhd[trackID++] = new AtomTKHD(fullBox[0], m_fileHandle);
			std::cout << "find tkhd" << std::endl;
		}
		else if (memcmp(boxHeader + 4, BoxMDIA, 4) == 0)//media
		{
			std::cout << "find mdia" << std::endl;
			Parse();
		}
		else if (memcmp(boxHeader + 4, BoxMDHD, 4) == 0)//mdhd:解析track时间参数,音视频都有
		{
			unsigned char fullBox[4];
			fread(fullBox, 1, 4, m_fileHandle);
			tkhd[trackID - 1]->mdhd = new AtomMDHD(fullBox[0], m_fileHandle);
			std::cout << "find mdhd" << std::endl;
		}
		else if (memcmp(boxHeader + 4, BoxVMHD, 4) == 0)//vmhd 标记这是视频
		{
			std::cout << "find vmhd" << std::endl;
			fseek(m_fileHandle, boxDataSize, SEEK_CUR);
		}
		else if (memcmp(boxHeader + 4, BoxSMHD, 4) == 0)//smhd 标记这是音频
		{
			std::cout << "find smhd" << std::endl;
			fseek(m_fileHandle, boxDataSize, SEEK_CUR);
		}
		else if (memcmp(boxHeader + 4, BoxMINF, 4) == 0)
		{
			std::cout << "find minf" << std::endl;
			Parse();
		}
		else if (memcmp(boxHeader + 4, BoxSTBL, 4) == 0)//Sample Table
		{
			std::cout << "find stbl" << std::endl;
			Parse();
		}
		else if (memcmp(boxHeader + 4, BoxSTSD, 4) == 0)//Sample Description
		{
			//stsd 解析sps和pps  
			std::cout << "find stsd" << std::endl;
			fseek(m_fileHandle, 8, SEEK_CUR);//跳个8字节 4(fullbox)+4(descount)
			fread(boxHeader, 1, 8, m_fileHandle);
			boxDataSize = ((boxHeader[0] << 24) | (boxHeader[1] << 16) | (boxHeader[2] << 8) | boxHeader[3]) - 8;
			
			if (memcmp(boxHeader + 4, "avc1", 4) == 0)//avc
			{
				fseek(m_fileHandle, 78, SEEK_CUR);
				Parse();
			}
			else if (memcmp(boxHeader + 4, "hev1", 4) == 0)
				fseek(m_fileHandle, boxDataSize, SEEK_CUR);
			else if (memcmp(boxHeader + 4, "mp4a", 4) == 0)//aac
				fseek(m_fileHandle, boxDataSize, SEEK_CUR);
			else if (memcmp(boxHeader + 4, "alaw", 4) == 0)//g711
				fseek(m_fileHandle, boxDataSize, SEEK_CUR);
			else
			{
				std::cout << std::string((char*)boxHeader + 4, 4) << std::endl;
				fseek(m_fileHandle, boxDataSize, SEEK_CUR);
			}
		}
		else if (memcmp(boxHeader + 4, "avcC", 4) == 0)//sps pps???
		{
			std::cout << "find avcC" << std::endl;
			unsigned char* data = new unsigned char[boxDataSize];
			fread(data, 1, boxDataSize, m_fileHandle);
			////sps
			//auto spsLen = ntohs(*((u_short*)(data + 6)));
			//sps = new unsigned char[spsLen];
			//memcpy(sps, data + 8, spsLen);
			////pps
			//auto ppsLen = ntohs(*((u_short*)(data + 8 + spsLen + 1)));
			//pps = new unsigned char[ppsLen];
			//memcpy(pps, data + 8 + spsLen + 3, spsLen);
			delete[] data;
		}
		else if (memcmp(boxHeader + 4, BoxSTTS, 4) == 0)//Timestamp table
		{
			std::cout << "find stts" << std::endl;
			fseek(m_fileHandle, 4, SEEK_CUR);//跳过4字节的fullbox
			uint32_t entries = 0;
			fread(&entries, 1, 4, m_fileHandle);
			entries = ntohl(entries);
			struct STTS 
			{
				uint32_t SampleCount;
				uint32_t SampleDelta;
			};
			STTS* stts = new STTS[entries];
			for (size_t i = 0; i < entries; i++)
			{
				fread(&stts[i].SampleCount, 1, 4, m_fileHandle);
				stts[i].SampleCount = ntohl(stts[i].SampleCount);
				tkhd[trackID - 1]->TotalSampleCount += stts[i].SampleCount;
				fread(&stts[i].SampleDelta, 1, 4, m_fileHandle);
				stts[i].SampleDelta = ntohl(stts[i].SampleDelta);
			}
			tkhd[trackID - 1]->samples = new SampleInfo[tkhd[trackID - 1]->TotalSampleCount];
			int ps = tkhd[trackID - 1]->mdhd->TimeScale / 1000;
			int k = 0;
			for (size_t i = 0; i < entries; i++)
			{
				for (size_t j = 0; j < stts[i].SampleCount; j++)
				{
					if (k == 0)
						tkhd[trackID - 1]->samples[k].TimeStamp = stts[i].SampleDelta / ps;
					else
						tkhd[trackID - 1]->samples[k].TimeStamp = tkhd[trackID - 1]->samples[k - 1].TimeStamp + stts[i].SampleDelta / ps;
					
					k++;
				}
			}
			delete[] stts;
		}
		else if (memcmp(boxHeader + 4, BoxSTSZ, 4) == 0)//Sample size table
		{
			std::cout << "find stsz" << std::endl;
			fseek(m_fileHandle, 4, SEEK_CUR);//跳过4字节的fullbox
			uint32_t sampleSize;
			uint32_t sampleCount;
			fread(&sampleSize, 1, 4, m_fileHandle);
			sampleSize = ntohl(sampleSize);
			fread(&sampleCount, 1, 4, m_fileHandle);
			sampleCount = ntohl(sampleCount);
			if (sampleSize == 0)
			{
				for (size_t i = 0; i < sampleCount; i++)
				{
					fread(&tkhd[trackID - 1]->samples[i].SampleSize, 1, 4, m_fileHandle);
					tkhd[trackID - 1]->samples[i].SampleSize = ntohl(tkhd[trackID - 1]->samples[i].SampleSize);
				}
			}
			else
			{
				for (size_t i = 0; i < sampleCount; i++)
					tkhd[trackID - 1]->samples[i].SampleSize = sampleSize;
			}
		}
		else if (memcmp(boxHeader + 4, BoxSTSS, 4) == 0)//确定media中的关键帧
		{
			std::cout << "find stss" << std::endl;
			fseek(m_fileHandle, 4, SEEK_CUR);//跳过4字节的fullbox
			uint32_t entries;
			uint32_t index;
			fread(&entries, 1, 4, m_fileHandle);
			entries = ntohl(entries);
			
			for (size_t i = 0; i < entries; i++)
			{
				fread(&index, 1, 4, m_fileHandle);
				index = ntohl(index);
				tkhd[trackID - 1]->samples[index - 1].isKeyFrame = true;
			}
		}
		else if (memcmp(boxHeader + 4, BoxSTSC, 4) == 0)//描述了sample与chunk的映射关系，查看这张表就可以找到包含指定sample的thunk，从而找到这个sample
		{
			std::cout << "find stsc" << std::endl;
			fseek(m_fileHandle, 4, SEEK_CUR);//跳过4字节的fullbox
			uint32_t entries;
			fread(&entries, 1, 4, m_fileHandle);
			entries = ntohl(entries);
		
			tkhd[trackID - 1]->stsc = new STSCInfo[tkhd[trackID - 1]->TotalSampleCount];
			
			uint32_t count = 0;
			uint32_t last_samples_in_chunk = 0;
			for (size_t i = 0; i < entries; i++)
			{
				uint32_t firstChunk;
				fread(&firstChunk, 1, 4, m_fileHandle);
				firstChunk = ntohl(firstChunk);
				uint32_t samples_per_chunk;
				fread(&samples_per_chunk, 1, 4, m_fileHandle);
				samples_per_chunk = ntohl(samples_per_chunk);
				uint32_t sample_description_index;
				fread(&sample_description_index, 1, 4, m_fileHandle);
				sample_description_index = ntohl(sample_description_index);

				while (count + 1 < firstChunk)
				{
					tkhd[trackID - 1]->stsc[count].chunkIndex = count + 1;
					tkhd[trackID - 1]->stsc[count].sampleCount = last_samples_in_chunk;
					count++;
				}
				
				tkhd[trackID - 1]->stsc[count].chunkIndex = firstChunk;
				tkhd[trackID - 1]->stsc[count].sampleCount = samples_per_chunk;
				count++;
				last_samples_in_chunk = samples_per_chunk;
			}
			tkhd[trackID - 1]->count1 = count;
		}
		else if (memcmp(boxHeader + 4, BoxSTCO, 4) == 0)//Chunk offsets Table
		{
			std::cout << "find stco" << std::endl;
			fseek(m_fileHandle, 4, SEEK_CUR);//跳过4字节的fullbox
		
			uint32_t entryCount;
			uint32_t offset;
			fread(&entryCount, 1, 4, m_fileHandle);
			entryCount = ntohl(entryCount);
			auto count1 = tkhd[trackID - 1]->count1;
			while (count1 < entryCount)
			{
				tkhd[trackID - 1]->stsc[count1].chunkIndex = count1 + 1;
				tkhd[trackID - 1]->stsc[count1].sampleCount = tkhd[trackID - 1]->stsc[count1 - 1].sampleCount;
				count1++;
			}
			size_t k = 0;
			for (size_t i = 0; i < entryCount; i++)
			{
				fread(&offset, 1, 4, m_fileHandle);
				offset = ntohl(offset);
				uint32_t lastOffset = 0;
				while (tkhd[trackID - 1]->stsc[i].sampleCount)
				{
					tkhd[trackID - 1]->samples[k].Position = offset + lastOffset;
					lastOffset += tkhd[trackID - 1]->samples[k].SampleSize;
					tkhd[trackID - 1]->stsc[i].sampleCount--;
					k++;
				}
			}
			delete[] tkhd[trackID - 1]->stsc;
			tkhd[trackID - 1]->stsc = nullptr;
		}
		else if (memcmp(boxHeader + 4, BoxCO64, 4) == 0)
		{
			std::cout << "find co64" << std::endl;
			fseek(m_fileHandle, boxDataSize, SEEK_CUR);
		}
		else
		{
			std::cout << "ignore:" << std::string((char*)boxHeader + 4, 4) << std::endl;
			fseek(m_fileHandle, boxDataSize, SEEK_CUR);
		}
	}
	
	return 0;
}

unsigned char startCode[] = { 0x00,0x00,0x00,0x01 };

int MP4Demuxer::GetNextFrame(uint8_t** data, uint32_t* sz, uint32_t* ts, bool* isKeyFrame)
{
	auto videoTrack = tkhd[0]->isVideo ? tkhd[0] : tkhd[1];
	if (videoTrack->curSampleIndex >= videoTrack->TotalSampleCount)
		return -1;
	fseek(m_fileHandle, videoTrack->samples[videoTrack->curSampleIndex].Position, SEEK_SET);
	if (!mediaData)
	{
		mediaData = new uint8_t[videoTrack->samples[videoTrack->curSampleIndex].SampleSize];
		mediaSize = videoTrack->samples[videoTrack->curSampleIndex].SampleSize;
	}
	else if (mediaSize < videoTrack->samples[videoTrack->curSampleIndex].SampleSize)
	{
		std::cout << "buf too small" << std::endl;
		delete[] mediaData;
		mediaData = new uint8_t[videoTrack->samples[videoTrack->curSampleIndex].SampleSize];
		mediaSize = videoTrack->samples[videoTrack->curSampleIndex].SampleSize;
	}
	int i = 0;
	uint32_t naluSize;
	while (i < videoTrack->samples[videoTrack->curSampleIndex].SampleSize)
	{
		fread(&naluSize, 1, 4, m_fileHandle);
		naluSize = ntohl(naluSize);
		memcpy(mediaData + i, startCode, 4);
		i += 4;
		fread(mediaData + i, 1, naluSize, m_fileHandle);
		i += naluSize;
	}
	if (i != videoTrack->samples[videoTrack->curSampleIndex].SampleSize)
		std::cout << "abc" << std::endl;
	*ts = videoTrack->samples[videoTrack->curSampleIndex].TimeStamp;
	*isKeyFrame = videoTrack->samples[videoTrack->curSampleIndex].isKeyFrame;
	*data = mediaData;
	*sz = i;
	videoTrack->curSampleIndex++;
	return 0;
}

int MP4Demuxer::Seek(uint32_t ts)
{
	auto duration = tkhd[0]->mdhd->Duration / (tkhd[0]->mdhd->TimeScale / 1000);
	if (ts > duration)
		return 1;

	return 0;
}