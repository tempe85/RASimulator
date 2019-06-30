﻿#include "WorkArea.h"

//Fill flow line with units
Unit CheckUnitType(string line, FlowLine FL)
{
	Unit DummyUnit;
	int i = 0;
	while (i < FL.UnitList.size())
	{
		if (line == FL.UnitList[i].first)
		{
			return FL.UnitList[i].second;
		}
		i++;
	}

	return DummyUnit; //nothing was found
}
string CheckAreaName(string line, FlowLine FL, int i)
{
	line = FL.TheWorkArea[i].AreaName;

	return line;
}

void PrintFlowLine(FlowLine FL)
{
	int j = 0;
	int i = 0;
	do
	{
		cout << FL.TheWorkArea[i].AreaName << " Units:";

		do 
		{
			//check if null
			if (!(FL.TheWorkArea[i].Stations[j] == nullptr))
			{
				cout << FL.TheWorkArea[i].Stations[j]->UnitName;
				cout << "(" << FL.TheWorkArea[i].Stations[j]->TimeLeft << "),";
			}
			else //empty
			{
				cout << "EMPTY" << ",";
			}
			j++;
		} while (j < FL.TheWorkArea[i].Stations.size() - 1);
		if (FL.TheWorkArea[i].Stations.size() > 1)
		{
			if (!(FL.TheWorkArea[i].Stations[j] == nullptr))
			{
					cout << FL.TheWorkArea[i].Stations[j]->UnitName;
					cout << "(" << FL.TheWorkArea[i].Stations[j]->TimeLeft << "),";
			}
			else //empty
			{
				cout << "EMPTY" << ",";
			}
		}
		cout << endl;
		cout << "Area Downtime: " <<FL.TheWorkArea[i].downtime;
		cout << endl;
		cout << "Area Overflow: ";
		if (!FL.TheWorkArea[i].OverFlow.empty())
		{
			//cout << FL.TheWorkArea.OverFLow;
			for (list<Unit*>::iterator it = FL.TheWorkArea[i].OverFlow.begin(); it != FL.TheWorkArea[i].OverFlow.end(); ++it)
			{
				cout << (*it)->UnitName;
				cout << '(' << (*it)->TotalUnitDownTime << "),";

			}
			//cout << FL.TheWorkArea[i].OverFlow.size();
		}
		else
		{
			cout << 0;
		}
		cout << endl;
		
		j = 0;
		i++;
	} while (i < FL.TheWorkArea.size());

	cout << endl << "Current Rework Tracker: ";
	for (vector<Unit*>::iterator it = FL.ReWork.begin(); it != FL.ReWork.end(); ++it)
	{
		cout << (*it)->UnitName << ",";
	}
	cout << endl << "Completed Units: Count:" << FL.CompletedUnitCounter << "  Unit Type: ";
	//for (vector<Unit*>::iterator it = FL.CompletedUnits.begin(); it != FL.CompletedUnits.end(); ++it)
	//{
	//	cout << (*it)->UnitName << ",";
	//}
	cout << FL.CompletedUnits; 
	cout << endl << "Value added minutes completed: " << FL.TimeValueAdded;
	cout << endl << "No value added minutes completed: " << FL.TimeNoValueAdded << endl;
}
double CalculateTimeLeft(string AreaName, Unit MOT, FlowLine FL, double BuildTime)
{
	double TimeLeft;
	TimeLeft = BuildTime * MOT.BuildTimeMap[AreaName];

	return TimeLeft;
}

FlowLine FillFlowLine(FlowLine &FL, Unit TestUnit, ifstream & ReadUnitFile)
{
	string line;
	Unit *UnitPointer;
	int i = 0;
	while (i < FL.TheWorkArea.size())
	{
		for (int j = 0; j < FL.TheWorkArea[i].Stations.size(); j++)
		{
			getline(ReadUnitFile, line);
			TestUnit = CheckUnitType(line, FL);
			FL.TheWorkArea[i].Stations[j] = new Unit;
			*(FL.TheWorkArea[i].Stations[j]) = TestUnit;
			//We Need to keep track of where the unit started on the flowline
			FL.TheWorkArea[i].Stations[j]->AreaStart = FL.TheWorkArea[i].AreaName;
			//There needs to be a build and FB exception
			if (FL.TheWorkArea[i].AreaName == "Build")
			{
				if (j == FL.TheWorkArea[i].Stations.size() - 1) //at station 6
				{
					FL.TheWorkArea[i].Stations[j]->TimeLeft = 5; //station 6 will only be a lookback area.
				}
				//Need to account for station 6 in build
				else
				{
					FL.TheWorkArea[i].Stations[j]->TimeLeft = (FL.TheWorkArea[i].BuildT[0] * TestUnit.BuildTimeMap[FL.TheWorkArea[i].AreaName]);
					FL.TheWorkArea[i].Stations[j]->TimeLeft -= ((FL.TheWorkArea[i].Stations[j]->TimeLeft) / (FL.TheWorkArea[i].Stations.size()-1))* j;
				}
			}
			else if (FL.TheWorkArea[i].AreaName == "FB")
			{
				FL.TheWorkArea[i].Stations[j]->TimeLeft = (FL.TheWorkArea[i].BuildT[0] * TestUnit.BuildTimeMap[FL.TheWorkArea[i].AreaName]); 
				FL.TheWorkArea[i].Stations[j]->TimeLeft -= ((FL.TheWorkArea[i].Stations[j]->TimeLeft) / ((FL.TheWorkArea[i].Stations.size()))* j);
			}
			else if (FL.TheWorkArea[i].AreaName == "UA") //UA needs kits available at the start, only the first time, overflow should have units, ~15
			{
				FL.TheWorkArea[i].Stations[j]->TimeLeft = (FL.TheWorkArea[i].BuildT[0] * TestUnit.BuildTimeMap[FL.TheWorkArea[i].AreaName]);
				FL.TheWorkArea[i].Stations[j]->TimeLeft -= ((FL.TheWorkArea[i].Stations[j]->TimeLeft) / (FL.TheWorkArea[i].Stations.size()))* j;
				if (FL.TheWorkArea[i].OverFlow.empty())
				{
					for (int k = 0; k < 15; k++) //this loop determines overflow starting size for UA
					{
						getline(ReadUnitFile, line);
						//TestUnit = CheckUnitType(line, FL);
						UnitPointer = new Unit;
						*(UnitPointer) = CheckUnitType(line, FL);
						FL.TheWorkArea[i].OverFlow.push_back(UnitPointer);

					}
				}

				//FL.TheWorkArea[i].Stations[j]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i].AreaName, TestUnit, FL, FL.TheWorkArea[i].BuildT[j]);
			
			}
			else //if it is not build or FB
			{
				FL.TheWorkArea[i].Stations[j]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i].AreaName, TestUnit, FL, FL.TheWorkArea[i].BuildT[j]);
			}
		}

		i++;
	}

	return FL;
}

void ReadInUnitList(vector<string> &UnitList)
{
	ifstream ReadUnitFile("Units.txt");
	string line;
	while (!ReadUnitFile.eof())
	{
		getline(ReadUnitFile, line);
		UnitList.push_back(line);

	}
	return;
}


bool CheckIfFinished(double TimeLeft)
{
	if (TimeLeft <= 0)
	{
		return true;
	}
	return false;
}

void MoveInFlowline (FlowLine &FL, int &i, int &j, double & WorkdayTime)
{
	if (i == 0 && j == 0) //we're at the start of the line, set i to -1 in order to break out of do loop
	{
		i = -1;
		j = 0;
		WorkdayTime -= FL.WorkTime;

		return;
	}

	else
	{
		//check if we are at the first area
		if (i == 0)
		{
			j--; //move back one station
		}
		else if (j == 0)
		{
			i--;
			j = FL.TheWorkArea[i].Stations.size() - 1;
		}
		else //need to move back station in an area
		{
			j--;
		}
	}

}

void MoveToOverFlow(FlowLine &FL, int i, int j)
{

	FL.TheWorkArea[i + 1].OverFlow.push_back(FL.TheWorkArea[i].Stations[j]);
	FL.TheWorkArea[i].Stations[j] = nullptr;

	return;
}

bool OverFlowIsEmpty(FlowLine &FL, int i)
{
	return FL.TheWorkArea[i].OverFlow.empty();
}

void RemoveFirstOverFlowUnit(FlowLine &FL, int i)
{
	FL.TheWorkArea[i].OverFlow.pop_front();

	return;
}

void SimulateFlowHelper(FlowLine &FL, ifstream & ReadUnitFile, int k)
{
	int CheckAreaDown;
	bool checkAreaDown = false;
	while (FL.WorkDay > 0)
	{
		CheckAreaDown = FL.WorkDay;
		if (k == 2 || k == 3)
		{
			cout << "Time left in day: " << FL.WorkDay << endl;
		}
		//SimulateFlowLine(FL, ReadUnitFile);
		//cout << CheckAreaDown % 30;
		if (CheckAreaDown % 30 == 0) //checking every thirty 30 minutes for area going down
		{
			FL.CheckAreaDown = true;
		}
		else
		{
			FL.CheckAreaDown = false;
		}
		SimulateFlowLine2(FL, ReadUnitFile);
		if (k == 2 || k == 3)
		{
			PrintFlowLine(FL);
			if (k == 2)
			{
				system("pause");
				system("cls");
			}
		}
	}
	if (k == 1)
	{
		PrintFlowLine(FL);
		system("pause");
	}
	return;
}

void AddUnitToFlow(FlowLine &FL, ifstream & ReadUnitFile, int i, int j)
{
	Unit TempUnit;
	string TempLine;
	getline(ReadUnitFile, TempLine);
	TempUnit = CheckUnitType(TempLine, FL);
	FL.TheWorkArea[i].Stations[j] = new Unit;
	*(FL.TheWorkArea[i].Stations[j]) = TempUnit;
	//Keeping track of where a unit started
	FL.TheWorkArea[i].Stations[j]->AreaStart = FL.TheWorkArea[i].AreaName;
	FL.TheWorkArea[i].Stations[j]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[0].AreaName, *(FL.TheWorkArea[i].Stations[j]), FL, FL.TheWorkArea[i].BuildT[0]); //BuildT is zero because FB BuildT is size 1
	//Check if rework needed
	UnitReworkCheck(FL.TheWorkArea[i], *(FL.TheWorkArea[i].Stations[j]), FL.TheWorkArea[i].Stations[j]->TimeLeft);


	return;
}

void CountAreaDowntime(FlowLine &FL)
{

	for (int i = 0; i <= FL.TheWorkArea.size() - 1; i++)
	{
		for (int j = 0; j <= FL.TheWorkArea[i].Stations.size() - 1; j++)
		{
			if (FL.TheWorkArea[i].Stations[j] == nullptr)
			{
				if (FL.TheWorkArea[i].AreaName != "ULT")
				{
					FL.TheWorkArea[i].downtime += FL.WorkTime;
				}
				else
				{
					if (j = 1) //last test station in ULT, must change if you add more test stations to ULT!!
					{
						if (FL.TheWorkArea[i].Stations[0] == nullptr && FL.TheWorkArea[i].Stations[1] == nullptr)
						{
							FL.TheWorkArea[i].downtime += FL.WorkTime;
						}
					}
				}
			}
		}
	}
}

void CalculateUnitDownTime(FlowLine FL)
{
	for (vector<WorkArea>::iterator it = FL.TheWorkArea.begin(); it != FL.TheWorkArea.end(); ++it)
	{
		//First check if Area has overflow
		if ((*it).OverFlow.empty())
		{
			//Container is 0, so skip
		}
		else
		{
			for (list<Unit*>::iterator it1 = it->OverFlow.begin(); it1 != it->OverFlow.end(); ++it1)
			{
				if (it->AreaName != "UA") //not counting FB downtime for now
				{
					(*it1)->TotalUnitDownTime += FL.WorkTime;
				}
			}
		}
	}
}

bool AreaDownHelper(FlowLine &FL, int & i, int & j)
{
	if (FL.TheWorkArea[i].AreaDown == true && FL.TheWorkArea[i].AreaDownTimer > 0)
	{
		//cout << endl << FL.TheWorkArea[i].AreaName << "Area down for " << FL.TheWorkArea[i].AreaDownTimer << " more minutes" << endl;
		if (FL.TheWorkArea[i].AreaDownTimer == 30)
		{
			cout << endl << FL.TheWorkArea[i].AreaName << " area Down! (" << FL.WorkDay<< ")" <<endl;
		}
		FL.TheWorkArea[i].AreaDownTimer -= FL.WorkTime;
		if (FL.TheWorkArea[i].AreaDownTimer <= 0) //station back up
		{
			cout << FL.TheWorkArea[i].AreaName << " back up! (" << FL.WorkDay << ")" << endl;
			FL.TheWorkArea[i].AreaDown == false;
		}
		return true;
	}
	return false;
}

FlowLine SimulateFlowLine2(FlowLine &FL, ifstream & ReadUnitFile)
{
	int i = FL.TheWorkArea.size() - 1;
	int j = FL.TheWorkArea[i].Stations.size() - 1;
	int Random_ULT;
	int Random_CLT;
	int Random_INIT;
	int Random_AVS;
	//Unit TempUnit;
	//string TempLine;
	if (FL.CheckAreaDown == true)
	{
		for (vector<WorkArea>::iterator it = FL.TheWorkArea.begin(); it != FL.TheWorkArea.end(); ++it)
		{
			if (it->CheckIfAreaDown())
			{
				it->AreaDown = true;
				it->AreaDownTimer = 30;
			};
			
		}
	}
	do
	{
		while (FL.TheWorkArea[i].AreaName == "Pkg")
		{
			SimulatePkg(FL, i, j);
		}
		while (FL.TheWorkArea[i].AreaName == "Rec" ||
			FL.TheWorkArea[i].AreaName == "Prep" ||
			FL.TheWorkArea[i].AreaName == "Doors" ||
			FL.TheWorkArea[i].AreaName == "Ins" ||
			FL.TheWorkArea[i].AreaName == "Settings"
			)
		{
			SimulateBasicArea(FL, i, j);
		}
		while (FL.TheWorkArea[i].AreaName == "UA")
		{
			SimulateUA(FL, i, j);
		}
		while (FL.TheWorkArea[i].AreaName == "INIT")
		{
			if (AreaDownHelper(FL, i, j) == true)
			{
				j = 0;
				OverFlowManager(FL, i, j);
			}
			else
			{
				SimulateBasicArea(FL, i, j);
			}
		}
		while (FL.TheWorkArea[i].AreaName == "CLT")
		{
			if (AreaDownHelper(FL, i, j) == true)
			{
				j = 0;
				OverFlowManager(FL, i, j);
			}
			else
			{
				SimulateCLT(FL, i, j);
			}
		}
		while (FL.TheWorkArea[i].AreaName == "Build")
		{
			SimulateBuild(FL, i, j);
		}
		while (FL.TheWorkArea[i].AreaName == "ULT")
		{
			if (AreaDownHelper(FL, i, j) == true)
			{
				j = 0;
				OverFlowManager(FL, i, j);
			}
			else
			{
				SimulateULT(FL, i, j);
			}
		}
		while (FL.TheWorkArea[i].AreaName == "AVS")
		{
			if (AreaDownHelper(FL, i, j) == true)
			{
				j = 0;
				OverFlowManager(FL, i, j);
			}
			else
			{
				SimulateAVS(FL, i, j);
			}
		}
		while (i == 0 && FL.TheWorkArea[i].AreaName == "FB") // when i is -1, this will break code
		{
			SimulateFormboard(FL, i, j, ReadUnitFile);
		}
		

	} while (i > -1 || j != 0);
	//Count downtime
	CountAreaDowntime(FL);
	CalculateUnitDownTime(FL);

	return FL;
}

void UnitTimeOutputs(FlowLine const FL, ofstream & UnitOutputs)
{
	UnitOutputs << "Unit Type, Start Area, Value Added Time, No Value added Time, Down Time\n";
	for (int i = 0; i < FL.CompletedUnits.size(); i++)
	{
		UnitOutputs << FL.CompletedUnits[i]->UnitName << ",";
		UnitOutputs << FL.CompletedUnits[i]->AreaStart << ",";
		UnitOutputs << FL.CompletedUnits[i]->TotalUnitValueTime << ",";
		UnitOutputs << FL.CompletedUnits[i]->TotalUnitNoValueTime << ",";
		UnitOutputs << FL.CompletedUnits[i]->TotalUnitDownTime << ",";
		UnitOutputs << "\n";
	}
}

void CreateUnitList(FlowLine &FL)
{

	SD1 Unit_SD1;
	SD2 Unit_SD2;
	DD1 Unit_DD1;
	DD2 Unit_DD2;
	
	FL.UnitList.push_back({ "SD1", Unit_SD1 });
	FL.UnitList.push_back({ "SD2", Unit_SD2 });
	FL.UnitList.push_back({ "DD1", Unit_DD1 });
	FL.UnitList.push_back({ "DD2", Unit_DD2 });


	return;
}
int main (void)
{
	FlowLine TestFlow;
	Unit TestUnit;
	vector<WorkArea> FL;
	vector<string> UnitList;
	ifstream ReadUnitFile("Units2.txt");
	//ofstream UnitOutputs("Output.csv", fstream::app); USE THIS if you don't want to overwrite 
	ofstream UnitOutputs("Output.csv"); 
	int k;
	srand(std::time(nullptr)); //seed for rand



	CreateFlowLine(FL);

	TestFlow.TheWorkArea = FL;

	CreateUnitList(TestFlow);
	FillFlowLine(TestFlow, TestUnit, ReadUnitFile);
//	PrintFlowLine(TestFlow);
	cout << "Please select between (1) Simulate entire day, (2) Simulate by minute (3) Test Mode";
	cin >> k;
	system("cls");
	SimulateFlowHelper(TestFlow, ReadUnitFile, k);
	UnitTimeOutputs(TestFlow, UnitOutputs);


	UnitOutputs.close();
	ReadUnitFile.close();

	//system("pause");

	return 0;
}
