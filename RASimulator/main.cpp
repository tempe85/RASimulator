#include "WorkArea.h"

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
	cout << endl << "Work Minutes completed: " << FL.WorkTimeFinished << endl;
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
			if (FL.TheWorkArea[i].AreaName == "Build" || FL.TheWorkArea[i].AreaName == "FB")
			{
				FL.TheWorkArea[i].Stations[j]->TimeLeft = (FL.TheWorkArea[i].BuildT[0] * TestUnit.BuildTimeMap[FL.TheWorkArea[i].AreaName]);
				FL.TheWorkArea[i].Stations[j]->TimeLeft -= ((FL.TheWorkArea[i].Stations[j]->TimeLeft) / ((FL.TheWorkArea[i].Stations.size()))* j);
			}
			else if (FL.TheWorkArea[i].AreaName == "UA" && FL.TheWorkArea[i].OverFlow.empty()) //UA needs kits available at the start, only the first time, overflow should have units, ~15
			{
				FL.TheWorkArea[i].Stations[j]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i].AreaName, TestUnit, FL, FL.TheWorkArea[i].BuildT[j]);
				for (int k = 0; k < 15; k++) //this loop determines overflow starting size for UA
				{
					getline(ReadUnitFile, line);
					//TestUnit = CheckUnitType(line, FL);
					UnitPointer = new Unit;
					*(UnitPointer) = CheckUnitType(line, FL);
					FL.TheWorkArea[i].OverFlow.push_back(UnitPointer);

				}
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
	while (FL.WorkDay > 0)
	{
		if (k == 2)
		{
			cout << "Time left in day: " << FL.WorkDay << endl;
		}
		//SimulateFlowLine(FL, ReadUnitFile);
		SimulateFlowLine2(FL, ReadUnitFile);
		if (k == 2)
		{
			PrintFlowLine(FL);
			system("pause");
			system("cls");
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
		//First check all units currently on the line with timeleft == 0
		//for (vector<Unit*>::iterator it2 = it->Stations.begin(); it2 != it->Stations.end(); ++it2)
		//{
		//	if (*it2 == nullptr)
		//	{
		//		//skip
		//	}
		//	else
		//	{
		//		if ((*it2)->TimeLeft <= 0)
		//		{
		//			(*it2)->TotalUnitDownTime += FL.WorkTime;
		//		}
		//	}
		//}
	}
}
FlowLine SimulateFlowLine2(FlowLine &FL, ifstream & ReadUnitFile)
{
	int i = FL.TheWorkArea.size() - 1;
	int j = FL.TheWorkArea[i].Stations.size() - 1;
	//Unit TempUnit;
	//string TempLine;
	do
	{
		while (FL.TheWorkArea[i].AreaName == "Pkg")
		{
			SimulatePkg(FL, i, j);
		}
		while (FL.TheWorkArea[i].AreaName == "UA" ||
			FL.TheWorkArea[i].AreaName == "INIT" ||
			FL.TheWorkArea[i].AreaName == "Rec" ||
			FL.TheWorkArea[i].AreaName == "Prep" ||
			FL.TheWorkArea[i].AreaName == "Doors" ||
			FL.TheWorkArea[i].AreaName == "Ins" ||
			FL.TheWorkArea[i].AreaName == "Settings"
			)
		{
			SimulateBasicArea(FL, i, j);
		}
		while (FL.TheWorkArea[i].AreaName == "CLT")
		{
			SimulateCLT(FL, i, j);
		}
		while (FL.TheWorkArea[i].AreaName == "Build")
		{
			SimulateBuild(FL, i, j);
		}
		while (FL.TheWorkArea[i].AreaName == "ULT")
		{
			SimulateULT(FL, i, j);
		}
		while (FL.TheWorkArea[i].AreaName == "AVS")
		{
			SimulateAVS(FL, i, j);
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
	int k;
	srand(std::time(nullptr)); //seed for rand


	CreateFlowLine(FL);

	TestFlow.TheWorkArea = FL;


	CreateUnitList(TestFlow);
	FillFlowLine(TestFlow, TestUnit, ReadUnitFile);
//	PrintFlowLine(TestFlow);
	cout << "Please select between (1) Simulate entire day and (2) Simulate by minute";
	cin >> k;
	system("cls");
	SimulateFlowHelper(TestFlow, ReadUnitFile, k);


	ReadUnitFile.close();

	int random_variable = rand() % 100 + 1;
	system("pause");

	return 0;
}
