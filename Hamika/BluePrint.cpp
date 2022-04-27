#include "BluePrint.h"

#define MapCodeNONE 0
#define MapCodeRANDOM 'R'
#define MapCodeMANUAL 'M'

const unsigned OLDKIRW = 63;
const unsigned OLDKIRH = 53;

struct TranslateData
{
	Type::ID
		Old,
		New;
};
TranslateData translate[] = {
	{0,-123},
	{1,},
	{2,},
	{3,0},
	{4,1},
	{5,32},
	{6,14},
	{7,15},
	{8,29},
	{9,31},
	{10,30},
	{11,28},
	{12,20},
	{13,21},
	{14,19},
	{15,22},
	{16,18},
	{17,17},
	{18,},
	{19,},
	{20,},
	{21,},
	{22,},
	{23,},
	{24,},
	{25,},
	{26,16},
	{27,27},
	{28,13},
	{29,},
	{30,},
	{31,},
	{32,},
	{33,},
	{34,},
	{35,},
	{36,},
	{37,25},
	{38,23},
	{39,},
	{40,},
	{41,},
	{42,},
	{43,},
	{44,24},
	{45,26},
	{46,},
	{47,},
	{48,},
	{49,},
	{50,},
	{51,},
	{52,},
	{53,4},
	{54,},
	{55,},
	{56,},
	{57,},
	{58,},
	{59,},
	{60,2},
	{61,},
	{62,},
	{63,},
	{64,},
	{65,},
	{66,},
	{67,},
	{68,},
	{69,},
	{70,},
	{71,},
	{72,},
	{73,},
	{74,6},
	{75,3},
	{76,7},
	{77,10},
	{78,8},
	{79,12},
	{80,9},
	{81,5},
	{82,11},
	{83,},
	{84,},
	{85,},
	{86,},
	{87,},
	{88,},
	{89,},
};


void BluePrint::SetName(const std::string &new_name)
{
	name = new_name;
}
void BluePrint::SetAim(int new_aim)
{
	aim = new_aim;
}
void BluePrint::SetSpeed(Type::Speed new_speed)
{
	speed = new_speed;
}

Type::Speed BluePrint::GetSpeed() const
{
	return speed;
}
int BluePrint::GetAim() const
{
	return aim;
}
std::string BluePrint::GetName() const
{
	return name;
}

