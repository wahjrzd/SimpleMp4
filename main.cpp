// main.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "MP4Demuxer.h"

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif // _WIN32

int main()
{
#if defined(_WIN32)&& defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	MP4Demuxer mp4;
	mp4.OpenFile("1.mp4");
	FILE* out_pf = nullptr;
#ifdef _WIN32
	fopen_s(&out_pf, "out.h264", "wb");
#else
	out_pf = fopen("out.h264", "wb");
#endif // _WIN32

	uint8_t* data;
	uint32_t sz;
	uint32_t pts;
	bool keyFrame;
	while (mp4.GetNextFrame(&data, &sz, &pts, &keyFrame) == 0)
	{
		fwrite(data, 1, sz, out_pf);
		//std::cout << "datasize:" << sz << ",pts" << pts << std::endl;
	}
	fclose(out_pf);
	//std::string line;
	//std::getline(std::cin, line);
	return 0;
}