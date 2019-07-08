#include "WorkArea.h"

FB::FB()
{
	AreaName = "FB";
	Stations = vector<Unit*>(6);
	BuildT = { 120 };
	ValueAdded = true;
	ReworkCoefficient = 5;
	MaxOverFlowSize = 0;
}
UA::UA()
{
	AreaName = "UA";
	Stations = vector<Unit*>(4);
	BuildT = {20,20,20,20,20};
	//BuildT = { 60 };
	ValueAdded = true;
	ReworkCoefficient = 7;
	MaxOverFlowSize = 20;

}

INIT::INIT()
{
	AreaName = "INIT";
	Stations = vector<Unit*>(1);
	BuildT = { 10 };
	FailCoefficient = 10;
	ReworkCoefficient = 2;
	AreaDownCoefficient = 11;
	ValueAdded = false;
	MaxOverFlowSize = 1;
}
AVS::AVS()
{
	AreaName = "AVS";
	Stations = vector<Unit*>(1);
	BuildT = { 10 };
	FailCoefficient = 20;
	ReworkCoefficient = 8;
	AreaDownCoefficient = 22;
	ValueAdded = false;
	MaxOverFlowSize = 1;
}
ULTest::ULTest()
{
	AreaName = "ULT";
	Stations = vector<Unit*>(1);
	BuildT = {30};
	//vector < pair<int, int>> vect = { {10, 10}, {10, 10}, {30,30} }; this is an alternative
	FailCoefficient = 50;
	AreaDownCoefficient = 22;
	ReworkCoefficient = 5;
	ValueAdded = false;
	MaxOverFlowSize = 1;
}


Receiving::Receiving()
{
	AreaName = "Rec";
	Stations = vector<Unit*>(1);
	BuildT = {20};
	ReworkCoefficient = 1;
	ValueAdded = true;
	MaxOverFlowSize = 3;
}

Prep::Prep()
{
	AreaName = "Prep";
	Stations = vector<Unit*>(1);
	BuildT = {20};
	ReworkCoefficient = 1;
	ValueAdded = true;
	MaxOverFlowSize = 1;
}

Build::Build()
{
	AreaName = "Build";
	Stations = vector<Unit*>(6);
	BuildT = { 90 };
	ReworkCoefficient = 5;
	ValueAdded = true;
	MaxOverFlowSize = 2;
}

CLTest::CLTest()
{
	AreaName = "CLT";
	Stations = vector<Unit*>(1);
	BuildT = {20};
	FailCoefficient = 20;
	ReworkCoefficient = 8;
	AreaDownCoefficient = 22;
	ValueAdded = false;
	MaxOverFlowSize = 1;
}

Settings::Settings()
{
	AreaName = "Settings";
	Stations = vector<Unit*>(1);
	BuildT = { 20 };
	ReworkCoefficient = 5;
	ValueAdded = false;
	MaxOverFlowSize = 1;
}
Doors::Doors()
{
	AreaName = "Doors";
	Stations = vector<Unit*>(1);
	BuildT = {20};
	ReworkCoefficient = 1;
	ValueAdded = true;
	MaxOverFlowSize = 2;
}

Inspection::Inspection()
{
	AreaName = "Ins";
	Stations = vector<Unit*>(1);
	BuildT = {20};
	ReworkCoefficient = 8;
	ValueAdded = false;
	MaxOverFlowSize = 1;
}

Packaging::Packaging()
{
	AreaName = "Pkg";
	Stations = vector<Unit*>(1);
	BuildT = {20};
	ValueAdded = true;
	MaxOverFlowSize = 3;
}


//NONPOINTER FORMAT:
//Packaging::Packaging()
//{
//	AreaName = "Packaging";
//	Stations = *(new vector<Unit>(3);
//	BuildTime = 30;
//}

// FAILING UNITS
void UnitFailCheckHelper(WorkArea const curArea, Unit & curUnit)
{
	if (get<0>(curUnit.UnitFailCheckVariable) == false) //if we haven't checked a unit yet
	{
		UnitFailCheck(curArea, curUnit);
		get<0>(curUnit.UnitFailCheckVariable) = true; //Setting to true to show we have checked the unit
	}
	if (get<1>(curUnit.UnitFailCheckVariable) == true) //If we have set the unit to 'failed'
	{
		get<2>(curUnit.UnitFailCheckVariable)--; //Decrement the unit fail counter
	}
}
void UnitFailCheck(WorkArea const curArea, Unit & curUnit)
{
	int random_number = rand() % 1000 + 1;
	if (random_number <= curArea.FailCoefficient)
	{
		get<1>(curUnit.UnitFailCheckVariable) = true; //The unit failing has been set to true
		//determine when a unit leaves
		int random_number2 = rand() % (int)curUnit.TimeLeft; //time which unit fails will be random during timeleft 
		get<2>(curUnit.UnitFailCheckVariable) = random_number2;
		return;
	}
	return;
}

void UnitFailCheckReset(Unit & curUnit)
{
	get<0>(curUnit.UnitFailCheckVariable) = false;
	get<1>(curUnit.UnitFailCheckVariable) = false;
	get<2>(curUnit.UnitFailCheckVariable) = 0;
}

void UnitReworkCheck(WorkArea curArea, Unit  & curUnit, double & unitBuildTime)
{
	int random_number = rand() % 100 + 1;
	if (random_number <= curArea.ReworkCoefficient)
	{
		cout << endl << curUnit.UnitName << " is being reworked in " << curArea.AreaName;
		random_number = rand() % 100 + 1;
		//short rework
		if (random_number <= 50) //50% odds short rework
		{
			cout << " for " << unitBuildTime * .2 << " extra minutes!";
			curUnit.TotalUnitReworkTime = unitBuildTime * .2;
			unitBuildTime += unitBuildTime * .2;
		}
		//medium rework
		else if (random_number > 50 && random_number <= 90) //40% odds medium rework
		{
			cout << " for " << unitBuildTime * .5 << " extra minutes!";
			curUnit.TotalUnitReworkTime = unitBuildTime * .5;
			unitBuildTime += unitBuildTime * .5;
		}
		//long rework
		else //10% odds long rework
		{
			cout << " for " << unitBuildTime * 1.0 << " extra minutes!";
			curUnit.TotalUnitReworkTime = unitBuildTime * 1.0;
			unitBuildTime += unitBuildTime * 1.0;
		}

		cout << endl;
	}

	return;
}

void PrintUnitFail(Unit FailedUnit, WorkArea curArea)
{
	cout << endl <<  FailedUnit.UnitName << " has been sent to troubleshoot from " << curArea.AreaName << endl;

}
void SendUnitToTS(FlowLine &FL, const int i, const int j)
{
	UnitFailCheckReset(*(FL.TheWorkArea[i].Stations[j]));
	PrintUnitFail(*(FL.TheWorkArea[i].Stations[j]), FL.TheWorkArea[i]);
	//determine Troubleshoot time
	int random_number = rand() % 100 + 1;
	if (random_number <= 33)
	{
		FL.TheWorkArea[i].Stations[j]->TotalUnitTroubleShootTime = 60;
	}
	else if (random_number >= 66)
	{
		FL.TheWorkArea[i].Stations[j]->TotalUnitTroubleShootTime = 180;
	}
	else
	{
		FL.TheWorkArea[i].Stations[j]->TotalUnitTroubleShootTime = 120;
	}
	FL.ReWork.push_back(FL.TheWorkArea[i].Stations[j]);
	FL.TheWorkArea[i].Stations[j] = nullptr; //unit leaves to rework
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