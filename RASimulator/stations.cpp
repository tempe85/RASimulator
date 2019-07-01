#include "WorkArea.h"

FB::FB()
{
	AreaName = "FB";
	Stations = vector<Unit*>(6);
	BuildT = { 120 };
	ValueAdded = true;
	ReworkCoefficient = 5;
}
UA::UA()
{
	AreaName = "UA";
	Stations = vector<Unit*>(5);
	//BuildT = {20,20,20,20,20};
	BuildT = { 60 };
	ValueAdded = true;
	ReworkCoefficient = 7;

}

INIT::INIT()
{
	AreaName = "INIT";
	Stations = vector<Unit*>(1);
	BuildT = { 10 };
	FailCoefficient = 1;
	ReworkCoefficient = 2;
	AreaDownCoefficient = 22;
	ValueAdded = false;
}
AVS::AVS()
{
	AreaName = "AVS";
	Stations = vector<Unit*>(1);
	BuildT = { 10 };
	FailCoefficient = 2;
	ReworkCoefficient = 8;
	AreaDownCoefficient = 22;
	ValueAdded = false;
}
ULTest::ULTest()
{
	AreaName = "ULT";
	Stations = vector<Unit*>(2);
	BuildT = {30, 30};
	//vector < pair<int, int>> vect = { {10, 10}, {10, 10}, {30,30} }; this is an alternative
	FailCoefficient = 2;
	AreaDownCoefficient = 22;
	ReworkCoefficient = 5;
	ValueAdded = false;
}


Receiving::Receiving()
{
	AreaName = "Rec";
	Stations = vector<Unit*>(1);
	BuildT = {20};
	ReworkCoefficient = 1;
	ValueAdded = true;
}

Prep::Prep()
{
	AreaName = "Prep";
	Stations = vector<Unit*>(1);
	BuildT = {20};
	ReworkCoefficient = 1;
	ValueAdded = true;
}

Build::Build()
{
	AreaName = "Build";
	Stations = vector<Unit*>(5);
	BuildT = { 100 };
	ReworkCoefficient = 5;
	ValueAdded = true;
}

CLTest::CLTest()
{
	AreaName = "CLT";
	Stations = vector<Unit*>(1);
	BuildT = {20};
	FailCoefficient = 2;
	ReworkCoefficient = 8;
	AreaDownCoefficient = 22;
	ValueAdded = false;
}

Settings::Settings()
{
	AreaName = "Settings";
	Stations = vector<Unit*>(1);
	BuildT = { 20 };
	ReworkCoefficient = 5;
	ValueAdded = false;
}
Doors::Doors()
{
	AreaName = "Doors";
	Stations = vector<Unit*>(1);
	BuildT = {20};
	ReworkCoefficient = 1;
	ValueAdded = true;
}

Inspection::Inspection()
{
	AreaName = "Ins";
	Stations = vector<Unit*>(1);
	BuildT = {20};
	ReworkCoefficient = 8;
	ValueAdded = false;
}

Packaging::Packaging()
{
	AreaName = "Pkg";
	Stations = vector<Unit*>(1);
	BuildT = {20};
	ValueAdded = true;
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
	int random_number = rand() % 100 + 1;
	if (random_number <= curArea.FailCoefficient)
	{
		return true;
	}
	return false;
}

void UnitReworkCheck(WorkArea curArea, Unit curUnit, double & unitBuildTime)
{
	int random_number = rand() % 100 + 1;
	if (random_number <= curArea.ReworkCoefficient)
	{
		cout << endl << curUnit.UnitName << " is being reworked in " << curArea.AreaName;
		random_number = rand() % 100 + 1;
		//short rework
		if (random_number <= 33)
		{
			cout << " for " << unitBuildTime * .2 << " extra minutes!";
			unitBuildTime += unitBuildTime * .2;
		}
		//medium rework
		else if (random_number > 33 && random_number < 66)
		{
			cout << " for " << unitBuildTime * .5 << " extra minutes!";
			unitBuildTime += unitBuildTime * .5;
		}
		//long rework
		else
		{
			cout << " for " << unitBuildTime * 1.0 << " extra minutes!";
			unitBuildTime += unitBuildTime * 1.0;
		}

		cout << endl;
	}

	return;
}
void PrintUnitFail(Unit FailedUnit, WorkArea curArea)
{
	cout << endl <<  FailedUnit.UnitName << " has been sent to rework from " << curArea.AreaName << endl;

}
void SendUnitToTS(FlowLine &FL, const int i, const int j, const bool IsOverFlow)
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