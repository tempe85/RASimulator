#include "WorkArea.h"

FB::FB()
{
	AreaName = "FB";
	Stations = vector<Unit*>(6);
	BuildT = { 120 };
}
UA::UA()
{
	AreaName = "UA";
	Stations = vector<Unit*>(5);
	BuildT = {10,10,10,10,10};
}

INIT::INIT()
{
	AreaName = "INIT";
	Stations = vector<Unit*>(1);
	BuildT = { 10 };
}
AVS::AVS()
{
	AreaName = "AVS";
	Stations = vector<Unit*>(1);
	BuildT = { 10 };
}
ULTest::ULTest()
{
	AreaName = "ULT";
	Stations = vector<Unit*>(1);
	BuildT = {30};
	//vector < pair<int, int>> vect = { {10, 10}, {10, 10}, {30,30} }; this is an alternative
}


Receiving::Receiving()
{
	AreaName = "Rec";
	Stations = vector<Unit*>(1);
	BuildT = {20};
}

Prep::Prep()
{
	AreaName = "Prep";
	Stations = vector<Unit*>(1);
	BuildT = {20};
}

Build::Build()
{
	AreaName = "Build";
	Stations = vector<Unit*>(5);
	BuildT = { 100 };
}

CLTest::CLTest()
{
	AreaName = "CLT";
	Stations = vector<Unit*>(2);
	BuildT = {30,20};
}

Doors::Doors()
{
	AreaName = "Doors";
	Stations = vector<Unit*>(1);
	BuildT = {20};
}

Inspection::Inspection()
{
	AreaName = "Ins";
	Stations = vector<Unit*>(1);
	BuildT = {20};
}

Packaging::Packaging()
{
	AreaName = "Pkg";
	Stations = vector<Unit*>(3);
	BuildT = {10,10,10};
}


//NONPOINTER FORMAT:
//Packaging::Packaging()
//{
//	AreaName = "Packaging";
//	Stations = *(new vector<Unit>(3);
//	BuildTime = 30;
//}