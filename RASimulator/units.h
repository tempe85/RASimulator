#include "Header.h"

class Unit
{
public:
	string UnitName = "TempUnitName";
	unordered_map<string, double> BuildTimeMap;
	double TimeLeft = 0;
	Unit() {};
};

class SD1 : public Unit
{
public:
	SD1()
	{
		UnitName = "SD1";
		BuildTimeMap = {
			{ "FB", 1.5},
			{ "UA", 1.0 },
			{ "INIT", .5 },
			{ "AVS", 1.0 },
			{ "ULT", 1.2 },
			{ "Rec", 1.0 },
			{ "Prep", 1.0 },
			{ "Build", 2.0 },
			{ "CLT", 2.0 },
			{ "Doors", 1.0 },
			{ "Ins", 1.0 },
			{ "Pkg", 1.0 },
		};
	}
};

class SD2 : public Unit
{
public:
	SD2()
	{
		UnitName = "SD2";
		BuildTimeMap = {
			{ "FB", .5},
			{ "UA", 1.0 },
			{ "INIT", 1.0 },
			{ "AVS", 1.0 },
			{ "ULT", .7 },
			{ "Rec", 1.0 },
			{ "Prep", 1.0 },
			{ "Build", .5 },
			{ "CLT", 1.0 },
			{ "Doors", 1.0 },
			{ "Ins", .8 },
			{ "Pkg", 1.0 },
		};
	}
};

class DD1 : public Unit
{
public:
	DD1()
	{
		UnitName = "DD1";
		BuildTimeMap = {
			{"FB", 1.0},
			{ "UA", 1.0 },
			{ "INIT", 1.0 },
			{ "AVS", 1.0 },
			{ "ULT", 1.0 },
			{ "Rec", .4 },
			{ "Prep", 1.5 },
			{ "Build", 1.3 },
			{ "CLT", 1.0 },
			{ "Doors", 1.8 },
			{ "Ins", 1.0 },
			{ "Pkg", 1.0 },
		};
	}
};

class DD2 : public Unit
{
public:
	DD2()
	{
		UnitName = "DD2";
		BuildTimeMap = {
			{"FB", 1.2},
			{ "UA", 1.0 },
			{ "INIT", 1.0 },
			{ "AVS", 1.0 },
			{ "ULT", 1.0 },
			{ "Rec", .4 },
			{ "Prep", 1.3 },
			{ "Build", 1.0 },
			{ "CLT", 2.0 },
			{ "Doors", 1.8 },
			{ "Ins", 1.0 },
			{ "Pkg", 1.0 },
		};
		
	}
};