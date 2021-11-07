#include "pch.h"
#include <random>
#include <numeric>
#include "IniFile.hpp"
#include "SKCModLoader.h"

using std::unordered_map;
using std::string;
using std::wstring;
using std::transform;

std::default_random_engine gen;

int sslaymode;
int ssnums[16];
char sslayouts[16][1026];

DataArray(short, word_69EC1F, 0x69EC1F, 1);
DataPointer(void*, dword_8FFE446, 0x8FFE446);
DataPointer(short, word_8FFFFAC, 0x8FFFFAC);

void LoadRandomStageMap(int ssnum)
{
	memset(Target_water_palette, 0, 0x600);
	memcpy(Plane_buffer, sslayouts[ssnum], 0x400);
	SS_start_angle = 0x4000;
	SS_start_x = 0;
	SS_start_y = 0;
	SS_perfect_count = *(unsigned short*)&sslayouts[ssnum][0x400];
}

void LoadBSStageMap()
{
	Kosinski_Decomp(SSCompressedLayoutPointers[1], RAM_start);
	memset(Target_water_palette, 0, 0x600);
	if (BS_special_stage_flag)
		Current_special_stage = 0;
	else
	{
		int ssnum = Current_special_stage;
		if (SK_alone_flag || SK_special_stage_flag)
			ssnum += 8;
		Blue_sphere_stage_number = ssnums[ssnum];
	}
	SS_start_angle = 0x4000;
	SS_start_x = 0x1000;
	SS_start_y = 0x300;
	SS_perfect_count = 0;
	word_8FFFFAC = 0;
	char* stgid = (char*)&Blue_sphere_stage_number;
	short* wordthing = word_69EC1F;
	for (int part = 0; part < 4; part++)
	{
		char v22 = *(stgid++);
		SS_perfect_count += (unsigned __int8)RAM_start[v22 & 0x7F];
		word_8FFFFAC += (unsigned __int8)RAM_start[(v22 & 0x7F) + 128];
		char* src = &RAM_start[(signed __int16)((v22 & 0x7F) << 8) + 256];
		short v23 = *(wordthing++);
		short v24 = *(wordthing++);
		short v25 = *(wordthing++);
		char* dst = &Plane_buffer[*(wordthing++)];
		for (int y = 0; y < 16; y++)
		{
			short off = v24;
			for (int x = 0; x < 16; x++)
			{
				*(dst++) = src[off];
				off += v23;
			}
			dst += 16;
			v24 += v25;
		}
	}
}

void LoadS3StageMap(int ssnum)
{
	memset(Target_water_palette, 0, 0x600);
	memcpy(Plane_buffer, SStageLayoutPtrs[ssnum], 0x400);
	unsigned short* src = (unsigned short*)((char*)SStageLayoutPtrs[ssnum] + 0x400);
	SS_start_angle = _byteswap_ushort(*(src++));
	SS_start_x = _byteswap_ushort(*(src++));
	SS_start_y = _byteswap_ushort(*(src++));
	SS_perfect_count = _byteswap_ushort(*(src++));
}

void LoadSKStageMap(int ssnum)
{
	Kosinski_Decomp(SSCompressedLayoutPointers[0], RAM_start);
	memset(Target_water_palette, 0, 0x600);
	memcpy(Plane_buffer, SSLayoutOffs_RAM[ssnum], 0x400);
	unsigned short* src = (unsigned short*)((char*)SSLayoutOffs_RAM[ssnum] + 0x400);
	SS_start_angle = _byteswap_ushort(*(src++));
	SS_start_x = _byteswap_ushort(*(src++));
	SS_start_y = _byteswap_ushort(*(src++));
	SS_perfect_count = _byteswap_ushort(*(src++));
}

void LoadSpecialStageMap_r()
{
	int ssnum;
	if (BS_special_stage_flag)
	{
		LoadBSStageMap();
		ssnum = ((char*)&Blue_sphere_stage_number)[2];
	}
	else
	{
		char emecnt = Emerald_count;
		char flag;
		if (SK_alone_flag)
			flag = 0;
		else
		{
			flag = SK_special_stage_flag;
			if (SK_special_stage_flag)
				emecnt = Super_emerald_count;
		}
		char curss;
		if (Debug_cheat_flag && Ctrl_1.Held & 0x40)
			curss = (char)Sound_test_sound;
		else
		{
			curss = Current_special_stage;
			if (emecnt < 7u)
			{
				int i = 5;
				flag *= 2;
				while (flag != Collected_emeralds_array[curss])
				{
					if (++curss >= 7u)
					{
						curss = 0;
						if (--i == -1)
						{
							curss = 7;
							break;
						}
					}
				}
			}
		}
		Current_special_stage = curss & 7;
		ssnum = Current_special_stage;
		if (SK_alone_flag || SK_special_stage_flag)
			ssnum += 8;
		switch (sslaymode)
		{
		case 1:
			int laynum = ssnums[ssnum];
			if (laynum & 8)
				LoadSKStageMap(laynum & 7);
			else
				LoadS3StageMap(laynum);
			break;
		case 2:
			LoadBSStageMap();
			break;
		case 3:
			LoadRandomStageMap(ssnum);
			break;
		}
	}
	AReg a1 = { *(void**)((char*)&SS_Pal_Map_Ptrs[1] + 16 * (ssnum & 7)) };
	if (ssnum & 8)
		a1.Byte += 304;
	dword_8FFE446 = a1.Unknown;
	memcpy(&Target_palette[56], a1.Word, 16);
	memcpy(&Target_palette[40], &a1.Word[16], 6);
}

unordered_map<string, int> ssmodes = {
	{ "off", 0 },
	{ "orig", 1 },
	{ "bs", 2 },
	{ "rand", 3 }
};

char ssprobtbl[0x20] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1,
	2,
	3, 3, 3,
	4, 4, 4,
	5
};

extern "C"
{
	__declspec(dllexport) void Init(const wchar_t* path, const HelperFunctions& helperFunctions)
	{
		const IniFile* settings = new IniFile(wstring(path) + L"\\config.ini");
		unsigned int seed = settings->getInt("", "seed");
		if (seed == 0)
			seed = std::random_device()();
		gen.seed(seed);
		helperFunctions.PrintDebug("Seed: %u\n", seed);
		string ssrandstr = settings->getString("", "sslayrand", "off");
		transform(ssrandstr.cbegin(), ssrandstr.cend(), ssrandstr.begin(), tolower);
		helperFunctions.PrintDebug("SS Layout: %s\n", ssrandstr.c_str());
		sslaymode = ssmodes[ssrandstr];
		if (sslaymode != 0)
		{
			WriteJump(LoadSpecialStageMap, LoadSpecialStageMap_r);
			switch (sslaymode)
			{
			case 1:
				std::iota(ssnums, &ssnums[16], 0);
				std::shuffle(ssnums, &ssnums[16], gen);
				for (int i = 0; i < 16; i++)
					helperFunctions.PrintDebug("SS%d Layout: %d\n", i + 1, ssnums[i] + 1);
				break;
			case 2:
				for (int i = 0; i < 16; i++)
				{
					do
					{
						ssnums[i] = gen() & 0x7F7F7F7F;
					}
					while (std::find(ssnums, &ssnums[i], ssnums[i]) != &ssnums[i]);
					helperFunctions.PrintDebug("SS%d Layout: %d\n", i + 1, ssnums[i]);
				}
				break;
			case 3:
				std::uniform_int_distribution<> ssdist(0, 0x1F);
				for (int i = 0; i < 16; i++)
				{
					short rings = 0;
					for (int j = 1; j < 1024; j++)
					{
						sslayouts[i][j] = ssprobtbl[ssdist(gen)];
						if (sslayouts[i][j] == 4)
							++rings;
					}
					*(unsigned short*)&sslayouts[i][0x400] = rings;
				}
				break;
			}
		}
		delete settings;
	}

	__declspec(dllexport) ModInfo SKCModInfo = { ModLoaderVer };
}