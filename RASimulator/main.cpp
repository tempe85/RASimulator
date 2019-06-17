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
			for (list<Unit*>::iterator it = FL.TheWorkArea[i].OverFlow.begin(); it != FL.TheWorkArea[i].OverFlow.end(); ++it)
			{
				cout << (*it)->UnitName << ",";

			}
		//	cout << FL.TheWorkArea[i].OverFlow.size();
		}
		else
		{
			cout << 0;
		}
		cout << endl << endl;
		j = 0;
		i++;
	} while (i < FL.TheWorkArea.size());


	cout << endl << "Completed Units: " << FL.CompletedUnitCounter << endl;
	cout << "Work Minutes completed: " << FL.WorkTimeFinished << endl;
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
			//first going to see if current spot is free and overflow for previous area has a unit
			if (!OverFlowIsEmpty(FL, i)) // if the overflow for the area is not empty
			{
				if (FL.TheWorkArea[i].Stations[j] == nullptr) //if current area is empty
				{
					FL.TheWorkArea[i].Stations[j] = FL.TheWorkArea[i].OverFlow.front();
					FL.TheWorkArea[i].Stations[j]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i].AreaName, *(FL.TheWorkArea[i].Stations[0]), FL, FL.TheWorkArea[i].BuildT[0]);
					RemoveFirstOverFlowUnit(FL, i);
				}
			}
			//at the first station in an area
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

void SimulateFlowHelper(FlowLine &FL, ifstream & ReadUnitFile)
{
	while (FL.WorkDay > 0)
	{
		cout << "Time left in day: " << FL.WorkDay << endl;
		SimulateFlowLine(FL, ReadUnitFile);
		PrintFlowLine(FL);
		system("pause");
		system("cls");

	}
	//PrintFlowLine(FL);
	//system("pause");

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
	FL.TheWorkArea[i].Stations[j]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[0].AreaName, *(FL.TheWorkArea[i].Stations[j]), FL, FL.TheWorkArea[i].BuildT[0]); //BuildT is zero because FB BuildT is size 1

	return;
}
FlowLine SimulateFlowLine(FlowLine &FL, ifstream & ReadUnitFile)
{
	int i = FL.TheWorkArea.size() - 1;
	int j = FL.TheWorkArea[i].Stations.size() - 1;
	Unit TempUnit;
	string TempLine;

	//We're starting at the end of the flowline
	do 
	{ 
		//If the Station is already NULL we must skip it
		if (FL.TheWorkArea[i].Stations[j] == nullptr)
		{
			MoveInFlowline(FL, i, j, FL.WorkDay);
		}
		else //Don't skip
		{
			if (CheckIfFinished(FL.TheWorkArea[i].Stations[j]->TimeLeft) == true)
			{
				//check if it's in FB
				if (FL.TheWorkArea[i].AreaName == "FB")
				{
					//check if first UA spot is open
					if (FL.TheWorkArea[i + 1].Stations[0] == nullptr)
					{
						FL.TheWorkArea[i + 1].Stations[0] = FL.TheWorkArea[i].Stations[j];
						FL.TheWorkArea[i].Stations[j] = nullptr;
					}
					else // UA spot is not open
					{
						//Move to UA overflow
						MoveToOverFlow(FL, i, j);
					}
					AddUnitToFlow(FL, ReadUnitFile, i, j);
					FL.TheWorkArea[i + 1].Stations[0]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i + 1].AreaName, *(FL.TheWorkArea[i + 1].Stations[0]), FL, FL.TheWorkArea[i + 1].BuildT[0]);
					MoveInFlowline(FL, i, j, FL.WorkDay);
				}
				//check if at the end of flowline
				else if (i == FL.TheWorkArea.size() - 1 && j == FL.TheWorkArea[i].Stations.size() - 1) //end of flowline
				{
					//pop unit out of the Flow Line as it is completed, first memory must be freed
					delete FL.TheWorkArea[i].Stations[j];
					FL.TheWorkArea[i].Stations[j] = nullptr;
					MoveInFlowline(FL, i, j, FL.WorkDay);
					FL.CompletedUnitCounter += 1;

				}
				else //not end of flowline but is finished, we must see if it can be moved to a new spot
				{
					//first check if it's at the end of an area
					if (j == FL.TheWorkArea[i].Stations.size() - 1)
					{
						//needs to be moved to a new area, only if a spot is available
						if (FL.TheWorkArea[i + 1].Stations[0] == nullptr) //there is a free spot
						{
							FL.TheWorkArea[i + 1].Stations[0] = FL.TheWorkArea[i].Stations[j]; //Moving pointer to the next area spot
							FL.TheWorkArea[i].Stations[j] = nullptr; //Current spot is now free
							//Need Timeleft for a unit reset

							//FL.TheWorkArea[i + 1].Stations[0]->TimeLeft = FL.TheWorkArea[i + 1].BuildT[0];

							FL.TheWorkArea[i + 1].Stations[0]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i + 1].AreaName, *(FL.TheWorkArea[i + 1].Stations[0]), FL, FL.TheWorkArea[i + 1].BuildT[0]);
							//double CalculateTimeLeft(string AreaName, Unit MOT, FlowLine FL, double BuildTime)
							//still need to change i and j
							MoveInFlowline(FL, i, j, FL.WorkDay);

						}
						else //Next spot is not free
						{
							//Move to Next area overflow
							FL.TheWorkArea[i + 1].OverFlow.push_back(FL.TheWorkArea[i].Stations[j]);
							FL.TheWorkArea[i].Stations[j] = nullptr;

							//Need to add to downtime
							//FL.TheWorkArea[i].downtime += FL.WorkTime; //No need with overflow at this time
							//still need to change i and j
							MoveInFlowline(FL, i, j, FL.WorkDay);
						}
					}
					else if (FL.TheWorkArea[i].AreaName == "Build") //The build exception!
					{
						//it can skip over a unit if possible. Build is the only place where this can happen
						if (FL.TheWorkArea[i + 1].Stations[0] == nullptr)
						{
							FL.TheWorkArea[i + 1].Stations[0] = FL.TheWorkArea[i].Stations[j];
							FL.TheWorkArea[i].Stations[j] = nullptr;
						}
						else //Test is not open, so move to test overflow
						{
							MoveToOverFlow(FL, i, j);
						}
					}
					else //Not at the end of an area, can't be moved to overflow! (unless in build line!)
					{

						//next spot is FL.TheWorkArea[i].Stations[j+1];
						//Check if next spot in the work area is free
						if (FL.TheWorkArea[i].Stations[j + 1] == nullptr) //next spot is free
						{
							//move pointer
							FL.TheWorkArea[i].Stations[j + 1] = FL.TheWorkArea[i].Stations[j];
							FL.TheWorkArea[i].Stations[j] = nullptr; //free current spot
							//Need to reset time left
							//FL.TheWorkArea[i].Stations[j + 1]->TimeLeft = FL.TheWorkArea[i].BuildT[j + 1];
							FL.TheWorkArea[i].Stations[j + 1]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i].AreaName, *(FL.TheWorkArea[i].Stations[j + 1]), FL, FL.TheWorkArea[i].BuildT[j + 1]);
							MoveInFlowline(FL, i, j, FL.WorkDay);
						}
						else //next station is not free
						{
							//Need to add to area downtime
							FL.TheWorkArea[i].downtime += FL.WorkTime;
							MoveInFlowline(FL, i, j, FL.WorkDay);
						}
					}
				}
			}
			else //not finished, timeleft > 0
			{
				FL.TheWorkArea[i].Stations[j]->TimeLeft -= FL.WorkTime; //worktime is the incremental change
				//Add to work time completed
				FL.WorkTimeFinished += FL.WorkTime;
				//Need to adjust in case TimeLeft is less than 0
				if (FL.TheWorkArea[i].Stations[j]->TimeLeft < 0)
				{
					FL.WorkTimeFinished += FL.TheWorkArea[i].Stations[j]->TimeLeft;
				}

				//Not finished but is in the build line, check if any spot in build line is free (starting at the end of build), also can't be at the end of the build line
				if (FL.TheWorkArea[i].AreaName == "Build" && j < FL.TheWorkArea[i].Stations.size() - 1)
				{
					for (int k = FL.TheWorkArea[i].Stations.size() - 1; k > j; k--)
					{
						if (FL.TheWorkArea[i].Stations[k] == nullptr)
						{
							FL.TheWorkArea[i].Stations[k] = FL.TheWorkArea[i].Stations[j];
							FL.TheWorkArea[i].Stations[j] = nullptr;
							k = j; //break out of loop
						}
					}
				}

				//Moving to next spot in line, as long as we're not at the start
				MoveInFlowline(FL, i, j, FL.WorkDay);

			}
		} 
	
	}while (i > -1 || j != 0); //negative one to allow loop to run when i = 0 and j = 0 which is the first unit in the Flowline


	//FL.WorkDay -= FL.WorkTime;

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
	srand(std::time(nullptr));


	CreateFlowLine(FL);

	TestFlow.TheWorkArea = FL;


	CreateUnitList(TestFlow);
	FillFlowLine(TestFlow, TestUnit, ReadUnitFile);
//	PrintFlowLine(TestFlow);
	SimulateFlowHelper(TestFlow, ReadUnitFile);


	ReadUnitFile.close();

	int random_variable = rand() % 100 + 1;
	system("pause");

	return 0;
}
