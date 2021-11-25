#include "pch.h"
#include <random>
#include <numeric>
#include "IniFile.hpp"
#include "SKCModLoader.h"
#include "MidiInterface.h"
#include "Trampoline.h"

using std::unordered_map;
using std::vector;
using std::string;
using std::wstring;
using std::transform;

std::default_random_engine gen;

int sslaymode;
int ssnums[16];
char sslayouts[16][1026];
Trampoline* LoadSongTramp = nullptr;
Trampoline* MonitorTramp = nullptr;
int MusicMap[MusicID_SKCredits + 1];
unordered_map<uint16_t, uint16_t> LevelMap;
char MonitorMap[10];
char SFXMap[0xA9];

const int MusicIDs[] = {
	MusicID_AngelIsland1,
	MusicID_AngelIsland2,
	MusicID_Hydrocity1,
	MusicID_Hydrocity2,
	MusicID_MarbleGarden1,
	MusicID_MarbleGarden2,
	MusicID_CarnivalNight1,
	MusicID_CarnivalNight2,
	MusicID_FlyingBattery1,
	MusicID_FlyingBattery2,
	MusicID_IceCap1,
	MusicID_IceCap2,
	MusicID_LaunchBase1,
	MusicID_LaunchBase2,
	MusicID_MushroomHill1,
	MusicID_MushroomHill2,
	MusicID_Sandopolis1,
	MusicID_Sandopolis2,
	MusicID_LavaReef1,
	MusicID_LavaReef2,
	MusicID_SkySanctuary,
	MusicID_DeathEgg1,
	MusicID_DeathEgg2,
	MusicID_SKMidboss,
	MusicID_Boss,
	MusicID_Doomsday,
	MusicID_MagneticOrbs,
	MusicID_SpecialStage,
	MusicID_SlotMachine,
	MusicID_GumballMachine,
	MusicID_S3Knuckles,
	MusicID_AzureLake,
	MusicID_BalloonPark,
	MusicID_DesertPalace,
	MusicID_ChromeGadget,
	MusicID_EndlessMine,
	MusicID_S3Invincibility,
	MusicID_CompetitionMenu,
	MusicID_S3Midboss,
	MusicID_LevelSelect,
	MusicID_FinalBoss,
	MusicID_S3Credits,
	MusicID_SKKnuckles,
	MusicID_SKInvincibility,
	MusicID_SKCredits,
};

const int JingleIDs[] = {
	MusicID_S3Title,
	MusicID_GameOver,
	MusicID_Continue,
	MusicID_ActClear,
	MusicID_ChaosEmerald,
	MusicID_Drowning,
	MusicID_S3AllClear,
	MusicID_SKTitle,
	MusicID_SKAllClear,
};

const int OneUpIDs[] = {
	MusicID_S31Up,
	MusicID_SK1Up,
};

vector<const char*> MusicNames = {
	"MIDI",
	"AngelIsland1",
	"AngelIsland2",
	"Hydrocity1",
	"Hydrocity2",
	"MarbleGarden1",
	"MarbleGarden2",
	"CarnivalNight1",
	"CarnivalNight2",
	"FlyingBattery1",
	"FlyingBattery2",
	"IceCap1",
	"IceCap2",
	"LaunchBase1",
	"LaunchBase2",
	"MushroomHill1",
	"MushroomHill2",
	"Sandopolis1",
	"Sandopolis2",
	"LavaReef1",
	"LavaReef2",
	"SkySanctuary",
	"DeathEgg1",
	"DeathEgg2",
	"SKMidboss",
	"Boss",
	"Doomsday",
	"MagneticOrbs",
	"SpecialStage",
	"SlotMachine",
	"GumballMachine",
	"S3Knuckles",
	"AzureLake",
	"BalloonPark",
	"DesertPalace",
	"ChromeGadget",
	"EndlessMine",
	"S3Invincibility",
	"CompetitionMenu",
	"S3Midboss",
	"LevelSelect",
	"FinalBoss",
	"S3Credits",
	"SKKnuckles",
	"SKInvincibility",
	"SKCredits",
	"S3CCredits",
	"S3Continue",
	"SKCredits0525",
	"GreenGrove1",
	"GreenGrove2",
	"RustyRuin1",
	"RustyRuin2",
	"VolcanoValley2",
	"VolcanoValley1",
	"SpringStadium1",
	"SpringStadium2",
	"DiamondDust1",
	"DiamondDust2",
	"GeneGadget1",
	"GeneGadget2",
	"PanicPuppet2",
	"FinalFight",
	"S3DSpecialStage",
	"PanicPuppet1",
	"S3DBoss2",
	"S3DBoss1",
	"S3DCredits",
	"S3DInvincibility",
	"S3DMenu",
	"S4E1Boss",
	"GreenHill",
	"Labyrinth",
	"Marble",
	"StarLight",
	"SpringYard",
	"ScrapBrain",
	"S1Invincibility",
	"S1SpecialStage",
	"S1Boss",
	"FinalZone",
	"S1Credits",
	"CasinoNight2P",
	"EmeraldHill",
	"Metropolis",
	"CasinoNight",
	"MysticCave",
	"MysticCave2P",
	"AquaticRuin",
	"S2DeathEgg",
	"S2SpecialStage",
	"S2Options",
	"S2FinalBoss",
	"ChemicalPlant",
	"S2Boss",
	"SkyChase",
	"OilOcean",
	"WingFortress",
	"EmeraldHill2P",
	"S22PResults",
	"S2SuperSonic",
	"HillTop",
	"S2Invincibility",
	"S2HiddenPalace",
	"S2Credits",
	"CasinoNight2PBeta",
	"EmeraldHillBeta",
	"MetropolisBeta",
	"CasinoNightBeta",
	"MysticCaveBeta",
	"MysticCave2PBeta",
	"AquaticRuinBeta",
	"S2DeathEggBeta",
	"S2SpecialStageBeta",
	"S2OptionsBeta",
	"S2FinalBossBeta",
	"ChemicalPlantBeta",
	"S2BossBeta",
	"SkyChaseBeta",
	"OilOceanBeta",
	"WingFortressBeta",
	"EmeraldHill2PBeta",
	"S22PResultsBeta",
	"S2SuperSonicBeta",
	"HillTopBeta",
	"S3DSpecialStageBeta",
	"ProtoAngelIsland1",
	"ProtoAngelIsland2",
	"ProtoHydrocity1",
	"ProtoHydrocity2",
	"ProtoCarnivalNight1",
	"ProtoCarnivalNight2",
	"ProtoFlyingBattery1",
	"ProtoFlyingBattery2",
	"ProtoIceCap1",
	"ProtoIceCap2",
	"ProtoLaunchBase1",
	"ProtoLaunchBase2",
	"ProtoMushroomHill1",
	"ProtoMushroomHill2",
	"ProtoLavaReef1",
	"ProtoLavaReef2",
	"ProtoSkySanctuary",
	"ProtoDoomsday",
	"ProtoSpecialStage",
	"ProtoKnuckles",
	"ProtoBalloonPark",
	"ProtoDesertPalace",
	"ProtoChromeGadget",
	"ProtoContinue",
	"ProtoCompetitionMenu",
	"ProtoLevelSelect",
	"ProtoCredits",
	"ProtoUnused",
	"CarnivalNight1PC",
	"CarnivalNight2PC",
	"IceCap1PC",
	"IceCap2PC",
	"LaunchBase1PC",
	"LaunchBase2PC",
	"KnucklesPC",
	"CompetitionMenuPC",
	"UnusedPC",
	"CreditsPC",
	"S3InvincibilityPC",
};

vector<const char*> JingleNames = {
	"MIDI",
	"S3Title",
	"GameOver",
	"Continue",
	"ActClear",
	"ChaosEmerald",
	"Drowning",
	"S3AllClear",
	"SKTitle",
	"SKAllClear",
	"SKTitle0525",
	"SKAllClear0525",
	"S3DEnding",
	"S3DIntro",
	"S1Title",
	"S1Ending",
	"S1ActClear",
	"S1GameOver",
	"S1Continue",
	"S1Drowning",
	"S1ChaosEmerald",
	"S2Ending",
	"S2Title",
	"ProtoActClear",
};

vector<const char*> OneUpNames = {
	"MIDI",
	"S31Up",
	"SK1Up",
	"S11Up",
};

const char* const MusicOptions[] = {
	"AngelIsland1Track",
	"AngelIsland2Track",
	"Hydrocity1Track",
	"Hydrocity2Track",
	"MarbleGarden1Track",
	"MarbleGarden2Track",
	"CarnivalNight1Track",
	"CarnivalNight2Track",
	"FlyingBattery1Track",
	"FlyingBattery2Track",
	"IceCap1Track",
	"IceCap2Track",
	"LaunchBase1Track",
	"LaunchBase2Track",
	"MushroomHill1Track",
	"MushroomHill2Track",
	"Sandopolis1Track",
	"Sandopolis2Track",
	"LavaReef1Track",
	"LavaReef2Track",
	"SkySanctuaryTrack",
	"DeathEgg1Track",
	"DeathEgg2Track",
	"MidbossTrack",
	"BossTrack",
	"DoomsdayTrack",
	"MagneticOrbsTrack",
	"SpecialStageTrack",
	"SlotMachineTrack",
	"GumballMachineTrack",
	"KnucklesTrack",
	"AzureLakeTrack",
	"BalloonParkTrack",
	"DesertPalaceTrack",
	"ChromeGadgetTrack",
	"EndlessMineTrack",
	"ContinueTrack",
	"InvincibilityTrack",
	"CompetitionMenuTrack",
	"UnusedTrack",
	"LevelSelectTrack",
	"FinalBossTrack",
	"CreditsTrack",
	"KnucklesTrack",
	"HiddenPalaceTrack",
	"SuperSonicTrack",
	"EndingTrack",
	"DataSelectTrack",
	"BlueSphereTitleTrack",
	"BlueSphereDifficultyTrack",
	"TimeAttackRecordsTrack",
	"KnucklesBossTrack"
};

const char* const JingleOptions[] = {
	"TitleScreenTrack",
	"GameOverTrack",
	"ActClearTrack",
	"ChaosEmeraldTrack",
	"DrowningTrack",
	"AllClearTrack",
	"SpecialStageResultTrack",
	"BlueSphereResultTrack"
};

const uint16_t LevelList_S3K[] = {
	angel_island_zone_act_1,
	hydrocity_zone_act_1,
	marble_garden_zone_act_1,
	carnival_night_zone_act_1,
	icecap_zone_act_1,
	launch_base_zone_act_1,
	mushroom_hill_zone_act_1,
	flying_battery_zone_act_1,
	sandopolis_zone_act_1,
	lava_reef_zone_act_1,
	hidden_palace_zone,
	sky_sanctuary_zone_act_1,
	death_egg_zone_act_1,
	doomsday_zone_act_1
};

const uint16_t LevelList_S3[] = {
	angel_island_zone_act_1,
	hydrocity_zone_act_1,
	marble_garden_zone_act_1,
	carnival_night_zone_act_1,
	icecap_zone_act_1,
	launch_base_zone_act_1
};

const uint16_t LevelList_SK[] = {
	mushroom_hill_zone_act_1,
	flying_battery_zone_act_1,
	sandopolis_zone_act_1,
	lava_reef_zone_act_1,
	hidden_palace_zone,
	sky_sanctuary_zone_act_1,
	death_egg_zone_act_1,
	doomsday_zone_act_1
};

const uint16_t NextLevelList[] = {
	hydrocity_zone_act_1,
	marble_garden_zone_act_1,
	carnival_night_zone_act_1,
	icecap_zone_act_1,
	launch_base_zone_act_1,
	mushroom_hill_zone_act_1,
	flying_battery_zone_act_1,
	sandopolis_zone_act_1,
	lava_reef_zone_act_1,
	hidden_palace_zone,
	sky_sanctuary_zone_act_1,
	death_egg_zone_act_1,
	doomsday_zone_act_1,
	ending_zone_act_2
};

const unordered_map<uint16_t, const char*> LevelNames = {
	{ angel_island_zone_act_1, "Angel Island Zone" },
	{ hydrocity_zone_act_1, "Hydro City Zone" },
	{ marble_garden_zone_act_1, "Marble Garden Zone" },
	{ carnival_night_zone_act_1, "Carnival Night Zone" },
	{ icecap_zone_act_1, "Ice Cap Zone" },
	{ launch_base_zone_act_1, "Launch Base Zone" },
	{ mushroom_hill_zone_act_1, "Mushroom Hill Zone" },
	{ flying_battery_zone_act_1, "Flying Battery Zone" },
	{ sandopolis_zone_act_1, "Sandopolis Zone" },
	{ lava_reef_zone_act_1, "Lava Reef Zone" },
	{ hidden_palace_zone, "Hidden Palace Zone" },
	{ sky_sanctuary_zone_act_1, "Sky Sanctuary Zone (Sonic)" },
	{ death_egg_zone_act_1, "Death Egg Zone" },
	{ doomsday_zone_act_1, "The Doomsday Zone" },
	{ ending_zone_act_2, "Sonic Ending" },
	{ sky_sanctuary_zone_act_2, "Sky Sanctuary Zone (Knuckles)" }
};

const char*const MonitorNames[] = {
	"Blank",
	"1Up",
	"Eggman",
	"Rings",
	"Speed Shoes",
	"Fire Shield",
	"Lightning Shield",
	"Bubble Shield",
	"Invincibility",
	"Super Sonic"
};

DataArray(short, word_69EC1F, 0x69EC1F, 1);
DataPointer(void*, dword_8FFE446, 0x8FFE446);
DataPointer(short, word_8FFFFAC, 0x8FFFFAC);
DataPointer(uint16_t, Apparent_zone_and_act, 0x8FFEE4E);
DataPointer(uint16_t, Restart_level_flag, 0x8FFFE02);
DataPointer(uint8_t, Last_star_post_hit, 0x8FFFE2A);
DataPointer(uint8_t, Special_bonus_entry_flag, 0x8FFFE48);
VoidFunc(Refresh_PlaneFull, 0x6ADFAB);

void StartNewLevel_r()
{
	uint16_t lev = reg_d0.UWord;
	if (LevelMap.find(lev) != LevelMap.cend())
		lev = LevelMap.at(lev);
	if (lev == UINT16_MAX)
	{
		Game_mode = GameModeID_S3Credits;
	}
	else
	{
		Current_zone_and_act = lev;
		Apparent_zone_and_act = lev;
		Restart_level_flag = 1;
		Last_star_post_hit = 0;
		Special_bonus_entry_flag = 0;
	}
}

void LoadMHZ1()
{
	reg_d0.UWord = mushroom_hill_zone_act_1;
	StartNewLevel_r();
}

void FixAIZIntro()
{
	reg_d0.Long = 0;
	Refresh_PlaneFull();
}

void MonitorSwap()
{
	reg_a0.Object->air_left = MonitorMap[reg_a0.Object->air_left];
	((decltype(MonitorSwap)*)MonitorTramp->Target())();
}

char SFXSwap()
{
	return SFXMap[reg_d0.UByte - 0x33];
}

int LoadSong_r(int song)
{
	return ((decltype(LoadSong_r)*)LoadSongTramp->Target())(MusicMap[song - 1] + 1);
}

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
		{
			int laynum = ssnums[ssnum];
			if (laynum & 8)
				LoadSKStageMap(laynum & 7);
			else
				LoadS3StageMap(laynum);
		}
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

unordered_map<string, int> levmodes = {
	{ "off", 0 },
	{ "orig", 1 },
	{ "mix", 2 }
};

unordered_map<string, int> musmodes = {
	{ "off", 0 },
	{ "swap", 1 },
	{ "rand", 2 }
};

unordered_map<string, int> ssmodes = {
	{ "off", 0 },
	{ "orig", 1 },
	{ "bs", 2 },
	{ "rand", 3 }
};

char startlevelcode[] = { 0x66, 0xB8, 0xFF, 0xFF, 0x66, 0xA3, 0x10, 0xFE, 0xFF, 0x08, 0x66, 0xA3, 0x4E, 0xEE, 0xFF, 0x08 };

char ssprobtbl[0x20] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1,
	2,
	3, 3, 3,
	4, 4, 4,
	5
};

template <typename T, size_t N>
void CopyShuffle(const T (&src)[N], T *dst)
{
	std::copy(src, src + N, dst);
	std::shuffle(dst, dst + N, gen);
}

#define PrintSpoiler if (spoilers) helperFunctions.PrintDebug
#define PathCat(s) wcscpy_s(pathbuf, path); wcscat_s(pathbuf, s)
extern "C"
{
	__declspec(dllexport) void Init(const wchar_t* path, const HelperFunctions& helperFunctions)
	{
		wchar_t pathbuf[MAX_PATH];
		PathCat(L"\\config.ini");
		const IniFile* settings = new IniFile(pathbuf);
		bool spoilers = settings->getBool("", "spoil");
		unsigned int seed = settings->getInt("", "seed");
		if (seed == 0)
			seed = std::random_device()();
		std::seed_seq seq = { seed };
		gen.seed(seq);
		PrintSpoiler("Seed: %u\n", seed);
		string levrandstr = settings->getString("", "levrand", "off");
		transform(levrandstr.cbegin(), levrandstr.cend(), levrandstr.begin(), tolower);
		if (levmodes.find(levrandstr) == levmodes.cend())
			levrandstr = "off";
		PrintSpoiler("Levels: %s\n", levrandstr.c_str());
		int levmode = levmodes[levrandstr];
		if (levmode)
		{
			uint16_t* levelorder = new uint16_t[LengthOfArray(LevelList_S3K) + 1];
			switch (levmode)
			{
			case 1:
				CopyShuffle(LevelList_S3, levelorder);
				CopyShuffle(LevelList_SK, levelorder + LengthOfArray(LevelList_S3));
				break;
			case 2:
				CopyShuffle(LevelList_S3K, levelorder);
				break;
			}
			levelorder[LengthOfArray(LevelList_S3K)] = gen() & 1 ? sky_sanctuary_zone_act_2 : ending_zone_act_2;
			PrintSpoiler("%s\n", LevelNames.at(levelorder[0]));
			for (size_t i = 1; i <= LengthOfArray(LevelList_S3K); ++i)
			{
				PrintSpoiler("%s\n", LevelNames.at(levelorder[i]));
				for (size_t j = 0; j < LengthOfArray(LevelList_S3K); ++j)
					if (LevelList_S3K[j] == levelorder[i - 1])
					{
						if (GameSelection == 2 && i == LengthOfArray(LevelList_S3))
							LevelMap[NextLevelList[j]] = UINT16_MAX;
						else
						{
							LevelMap[NextLevelList[j]] = levelorder[i];
							if (j == LengthOfArray(LevelList_S3K) - 1)
								LevelMap[sky_sanctuary_zone_act_2] = levelorder[i];
						}
						break;
					}
			}
			WriteJump((void*)0x6F2A3C, StartNewLevel_r);
			WriteJump((void*)0x75B403, (void*)0x75B418); // always go to DDZ after DEZ Boss
			WriteData((uint16_t*)0x7C209F, (uint16_t)sky_sanctuary_zone_act_1); // go to SSZ1 instead of SSZ2
			WriteCall((void*)0x7DD204, LoadMHZ1);
			WriteData((uint16_t*)0x6DE677, levelorder[LengthOfArray(LevelList_S3)]); // starting zone for S&K
			*(uint16_t*)&startlevelcode[2] = levelorder[0];
			WriteData((char*)0x6E27B3, startlevelcode);
			WriteData((char*)0x6E2FB4, startlevelcode);
			WriteData((char*)0x6EACFE, startlevelcode);
			WriteData((char*)0x6EB33D, startlevelcode);
			WriteData((decltype(FixAIZIntro)**)0x6B21B7, FixAIZIntro);
			delete[] levelorder;
		}
		string ssrandstr = settings->getString("", "sslayrand", "off");
		transform(ssrandstr.cbegin(), ssrandstr.cend(), ssrandstr.begin(), tolower);
		if (ssmodes.find(ssrandstr) == ssmodes.cend())
			ssrandstr = "off";
		PrintSpoiler("SS Layout: %s\n", ssrandstr.c_str());
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
					PrintSpoiler("SS%d -> SS%d\n", i + 1, ssnums[i] + 1);
				break;
			case 2:
				for (int i = 0; i < 16; i++)
				{
					do
					{
						ssnums[i] = gen() & 0x7F7F7F7F;
					}
					while (std::find(ssnums, &ssnums[i], ssnums[i]) != &ssnums[i]);
					PrintSpoiler("SS%d -> Stage %d\n", i + 1, ssnums[i]);
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
		if (settings->getBool("", "monrand"))
		{
			std::iota(MonitorMap, &MonitorMap[10], 0);
			std::shuffle(MonitorMap, &MonitorMap[10], gen);
			MonitorTramp = new Trampoline(0x799BB0, 0x799BB6, MonitorSwap);
			if (spoilers)
			{
				helperFunctions.PrintDebug("Monitors:\n");
				for (size_t i = 0; i < 10; ++i)
					helperFunctions.PrintDebug("%s -> %s\n", MonitorNames[i], MonitorNames[MonitorMap[i]]);
			}
		}
		if (settings->getBool("", "sfxrand"))
		{
			std::iota(SFXMap, &SFXMap[LengthOfArray(SFXMap)], 0x33);
			std::swap(SFXMap[0], SFXMap[1]);
			std::shuffle(&SFXMap[1], &SFXMap[0x89], gen);
			std::swap(SFXMap[0], SFXMap[1]);
			std::shuffle(&SFXMap[0x89], &SFXMap[LengthOfArray(SFXMap)], gen);
			WriteCall((void*)0x40A9A9, SFXSwap);
			WriteData((char*)0x40AA8E, ((char*)0x82C878)[SFXMap[0x78]]);
			if (spoilers)
			{
				helperFunctions.PrintDebug("SFX:\n");
				for (size_t i = 0; i < LengthOfArray(SFXMap); ++i)
					helperFunctions.PrintDebug("%.2X -> %.2X\n", i + 0x33, (unsigned char)SFXMap[i]);
			}
		}
		string musrandstr = settings->getString("", "musrand", "off");
		transform(musrandstr.cbegin(), musrandstr.cend(), musrandstr.begin(), tolower);
		if (musmodes.find(musrandstr) == musmodes.cend())
			musrandstr = "off";
		PrintSpoiler("Music: %s\n", musrandstr.c_str());
		int musmode = musmodes[musrandstr];
		PathCat(L"\\Music\\Music.ini");
		DeleteFile(pathbuf);
		switch (musmode)
		{
		case 1:
		{
			LoadSongTramp = new Trampoline(0x40B5EB, 0x40B5F0, LoadSong_r);
			int* tmp = new int[LengthOfArray(MusicIDs)];
			memcpy(tmp, MusicIDs, SizeOfArray(MusicIDs));
			std::shuffle(tmp, tmp + LengthOfArray(MusicIDs), gen);
			for (size_t i = 0; i < LengthOfArray(MusicIDs); ++i)
				MusicMap[MusicIDs[i]] = tmp[i];
			tmp = new int[LengthOfArray(JingleIDs)];
			memcpy(tmp, JingleIDs, SizeOfArray(JingleIDs));
			std::shuffle(tmp, tmp + LengthOfArray(JingleIDs), gen);
			for (size_t i = 0; i < LengthOfArray(JingleIDs); ++i)
				MusicMap[JingleIDs[i]] = tmp[i];
			tmp = new int[LengthOfArray(OneUpIDs)];
			memcpy(tmp, OneUpIDs, SizeOfArray(OneUpIDs));
			std::shuffle(tmp, tmp + LengthOfArray(OneUpIDs), gen);
			for (size_t i = 0; i < LengthOfArray(OneUpIDs); ++i)
				MusicMap[OneUpIDs[i]] = tmp[i];
		}
		break;
		case 2:
			PathCat(L"\\Music\\Music.txt");
			FILE* f = _wfopen(pathbuf, L"r");
			if (f)
			{
				while (!feof(f))
				{
					char line[1024];
					if (!fgets(line, sizeof(line), f))
						break;
					if (strnlen(line, sizeof(line)) == 0)
						continue;
					MusicNames.push_back(_strdup(line));
				}
				fclose(f);
			}
			PathCat(L"\\Music\\Jingle.txt");
			f = _wfopen(pathbuf, L"r");
			if (f)
			{
				while (!feof(f))
				{
					char line[1024];
					if (!fgets(line, sizeof(line), f))
						break;
					if (strnlen(line, sizeof(line)) == 0)
						continue;
					JingleNames.push_back(_strdup(line));
				}
				fclose(f);
			}
			PathCat(L"\\Music\\1Up.txt");
			f = _wfopen(pathbuf, L"r");
			if (f)
			{
				while (!feof(f))
				{
					char line[1024];
					if (!fgets(line, sizeof(line), f))
						break;
					if (strnlen(line, sizeof(line)) == 0)
						continue;
					OneUpNames.push_back(_strdup(line));
				}
				fclose(f);
			}
			PathCat(L"\\Music\\Music.ini");
			IniFile* muscfg = new IniFile(pathbuf);
			IniGroup* grp = muscfg->createGroup("All");
			std::uniform_int_distribution<> musdist(0, MusicNames.size() - 1);
			for (auto i : MusicOptions)
				grp->setString(i, MusicNames[musdist(gen)]);
			musdist = std::uniform_int_distribution<>(0, JingleNames.size() - 1);
			for (auto i : JingleOptions)
				grp->setString(i, JingleNames[musdist(gen)]);
			musdist = std::uniform_int_distribution<>(0, OneUpNames.size() - 1);
			grp->setString("OneUpTrack", OneUpNames[musdist(gen)]);
			muscfg->save(pathbuf);
			delete muscfg;
			break;
		}
		delete settings;
	}

	__declspec(dllexport) ModInfo SKCModInfo = { ModLoaderVer };
}