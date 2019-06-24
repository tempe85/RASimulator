#include "WorkArea.h"

FB::FB()
{
	AreaName = "FB";
	Stations = vector<Unit*>(5);
	BuildT = { 120 };
}
UA::UA()
{
	AreaName = "UA";
	Stations = vector<Unit*>(5);
	BuildT = {20,20,20,20,20};

}

INIT::INIT()
{
	AreaName = "INIT";
	Stations = vector<Unit*>(1);
	BuildT = { 10 };
	FailCoefficient = 1;
	AreaDownCoefficient = 22;
}
AVS::AVS()
{
	AreaName = "AVS";
	Stations = vector<Unit*>(1);
	BuildT = { 10 };
	FailCoefficient = 2;
	ReworkCoefficient = 5;
	AreaDownCoefficient = 22;
}
ULTest::ULTest()
{
	AreaName = "ULT";
	Stations = vector<Unit*>(2);
	BuildT = {30, 30};
	//vector < pair<int, int>> vect = { {10, 10}, {10, 10}, {30,30} }; this is an alternative
	FailCoefficient = 2;
	AreaDownCoefficient = 22;
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
	Stations = vector<Unit*>(1);
	BuildT = {20};
	FailCoefficient = 2;
	AreaDownCoefficient = 22;
}

Settings::Settings()
{
	AreaName = "Settings";
	Stations = vector<Unit*>(1);
	BuildT = { 20 };

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
	Stations = vector<Unit*>(1);
	BuildT = {20};
}


//NONPOINTER FORMAT:
//Packaging::Packaging()
//{
//	AreaName = "Packaging";
//	Stations = *(new vector<Unit>(3);
//	BuildTime = 30;
//}

// FAILING UNITS
bool UnitFailCheck(WorkArea curArea)
{
	int random_variable = rand() % 100 + 1;
	if (random_variable <= curArea.FailCoefficient)
	{
		return true;
	}
	return false;
}
void PrintUnitFail(Unit FailedUnit, WorkArea curArea)
{
	cout << endl <<  FailedUnit.UnitName << " has been sent to rework from " << curArea.AreaName << endl;

}
void SendUnitToRework(FlowLine &FL, const int i, const int j, const bool IsOverFlow)
{
	if (IsOverFlow == false)
	{
		PrintUnitFail(*(FL.TheWorkArea[i].Stations[j]), FL.TheWorkArea[i]);
		FL.ReWork.push_back(FL.TheWorkArea[i].Stations[j]);
		FL.TheWorkArea[i].Stations[j] = nullptr; //unit leaves to rework
	}
	else
	{
		PrintUnitFail(*(FL.TheWorkArea[i].OverFlow.front()), FL.TheWorkArea[i]);
		FL.ReWork.push_back(FL.TheWorkArea[i].OverFlow.front());
		RemoveFirstOverFlowUnit(FL, i);
	}
}

bool WorkArea::CheckIfAreaDown()
{
	int random_variable = rand() % 1000 + 1;
	if (this->AreaDownCoefficient >= random_variable)
	{
		return true;
	}
	else
	{
		return false;
	}
}