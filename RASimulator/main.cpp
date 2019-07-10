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

void PrintFailCheckData(FlowLine FL, int i, int j)
{
	if (get<0>(FL.TheWorkArea[i].Stations[j]->UnitFailCheckVariable) == true)
	{
		cout << "(T)";
	}
	else
	{
		cout << "(F)";
	}
	if (get<1>(FL.TheWorkArea[i].Stations[j]->UnitFailCheckVariable) == true)
	{
		cout << "(T)";
	}
	else
	{
		cout << "(F)";
	}
	cout << "(" << get<2>(FL.TheWorkArea[i].Stations[j]->UnitFailCheckVariable) << ")";
}
void PrintFlowLine(FlowLine FL)
{
	int j = 0;
	int i = 0;
	int overflowcounter = 0;
	do
	{
		cout << "************************************" << endl;
		cout << FL.TheWorkArea[i].AreaName << " Units:";

		do
		{
			//check if null
			if (!(FL.TheWorkArea[i].Stations[j] == nullptr))
			{
				cout << FL.TheWorkArea[i].Stations[j]->UnitName;
				//PrintFailCheckData(FL, i, j);
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
		cout << "Area Downtime: " << FL.TheWorkArea[i].downtime << endl;
		cout << "Area Downtime %: " << FL.TheWorkArea[i].downtime / FL.WorkDayCounter << endl;
		cout << "Total Area Rework time: " << FL.ReworkTimeByArea[FL.TheWorkArea[i].AreaName];
		cout << endl;
		cout << "Area Overflow: " << '(' << FL.TheWorkArea[i].OverFlow.size() <<  "):";
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
		cout << endl << "************************************" << endl;
		cout << endl;

		j = 0;
		overflowcounter += FL.TheWorkArea[i].OverFlow.size();
		i++;
	} while (i < FL.TheWorkArea.size());

	cout << endl << "Current Rework Tracker: ";
	for (list<Unit*>::iterator it = FL.ReWork.begin(); it != FL.ReWork.end(); ++it)
	{
		cout << (*it)->UnitName << "(" << (*it)->TotalUnitTroubleShootTime << ")" << ",";
	}
	cout << endl << "Completed Units: Count:" << FL.CompletedUnitCounter << "  Unit Type: ";
	//for (vector<Unit*>::iterator it = FL.CompletedUnits.begin(); it != FL.CompletedUnits.end(); ++it)
	//{
	//	cout << (*it)->UnitName << ",";
	//}
	cout << FL.CompletedUnits;
	//PPU
	int ppu = 0;
	for (vector<WorkArea>::iterator it = FL.TheWorkArea.begin(); it != FL.TheWorkArea.end(); ++it)
	{
		ppu += it->Stations.size();
	}

	//cout << endl << ppu;
	//cout << endl << FL.WorkDayCounter;
	cout << endl << "PPU (people per unit): " << (double)FL.CompletedUnits.size() / (double)ppu;
	cout << endl << "Kits for UA: "<< FL.TheWorkArea[1].OverFlow.size() << " Non-UA overflow total size: " << overflowcounter - FL.TheWorkArea[1].OverFlow.size();
	cout << endl << "Value added minutes completed: " << FL.TimeValueAdded;
	cout << endl << "No value added minutes completed: " << FL.TimeNoValueAdded;
	cout << endl << "Unit rework time: " << FL.TimeUnitRework;
	cout << endl << "Total work minutes completed: " << FL.TimeValueAdded + FL.TimeNoValueAdded << endl;
}
double CalculateTimeLeft(string AreaName, Unit MOT, FlowLine FL, double BuildTime)
{
	double TimeLeft;
	TimeLeft = BuildTime * MOT.BuildTimeMap[AreaName];

	return TimeLeft;
}

string GenerateUnitList(FlowLine & FL)
{
	//Second is the type of list, first is the counter
	//4,4,4,4
	if (FL.userInputGenerator == 1)
	{
		if (FL.unitGeneratorCounter > 15)
		{
			//reset unitCounter
			FL.unitGeneratorCounter = 0;
		}
		if (FL.unitGeneratorCounter <= 3)
		{
			return "SD1";
		}
		else if (FL.unitGeneratorCounter > 3 && FL.unitGeneratorCounter <= 7)
		{
			if (FL.UnitMode == 1)
			{
				return "SD2";
			}
			return "DD1";
		}
		else if (FL.unitGeneratorCounter > 7 && FL.unitGeneratorCounter <= 11)
		{
			if (FL.UnitMode == 1)
			{
				return "SD1";
			}
			return "SD2";
		}
		else
		{
			if (FL.UnitMode == 1)
			{
				return "SD2";
			}
			return "DD2";
		}
	}
	else if (FL.userInputGenerator == 2) //1,1,1,1
	{

		if (FL.unitGeneratorCounter > 3)
		{
			//reset unitCounter
			FL.unitGeneratorCounter = 0;
		}
		if (FL.unitGeneratorCounter == 0)
		{
			return "SD1";
		}
		else if (FL.unitGeneratorCounter == 1)
		{
			if (FL.UnitMode == 1)
			{
				return "SD2";
			}
			return "DD1";
		}
		else if (FL.unitGeneratorCounter == 2)
		{
			if (FL.UnitMode == 1)
			{
				return "SD1";
			}
			return "SD2";
		}
		else if (FL.unitGeneratorCounter == 3)
		{
			if (FL.UnitMode == 1)
			{
				return "SD2";
			}
			return "DD2";
		}
	}
	else // (TypeOfList == 3) //random list, add exception later
	{
		int random_number = rand() % 100 + 1;
		if (random_number <= 25)
		{
			return "SD1";
		}
		else if (random_number > 25 && random_number <= 50)
		{
			if (FL.UnitMode == 1)
			{
				return "SD1";
			}
			return "DD1";
		}
		else if (random_number > 50 && random_number >= 75)
		{
			return "SD2";
		}
		else //random_number between 76 and 100
		{
			if (FL.UnitMode == 1)
			{
				return "SD2";
			}
			return "DD2";
		}
	}

}

FlowLine FillFlowLine(FlowLine &FL, Unit TestUnit, ifstream & ReadUnitFile)
{
	string line;
	Unit *UnitPointer;
	int i = 0;
	//FL.AreaOrder = CreateAreaOrderString(FL);

	while (i < FL.TheWorkArea.size())
	{
		for (int j = 0; j < FL.TheWorkArea[i].Stations.size(); j++)
		{
			line = GenerateUnitList(FL);
			if (FL.userInputGenerator == 1 || FL.userInputGenerator == 2) //after generation, increment counter unless using the random generator
			{
				FL.unitGeneratorCounter++;
			}
			//getline(ReadUnitFile, line); use this if you want to fill flow from file
			TestUnit = CheckUnitType(line, FL);
			//Assign unit a unique ID number
			TestUnit.IDnum = FL.AssignUnitID;
			FL.AssignUnitID++;

			FL.TheWorkArea[i].Stations[j] = new Unit;
			*(FL.TheWorkArea[i].Stations[j]) = TestUnit;
			//We Need to keep track of where the unit started on the flowline
			FL.TheWorkArea[i].Stations[j]->AreaStart = FL.TheWorkArea[i].AreaName;

			string TempString;

			//Set completed area map for unit
			//if (FL.TheWorkArea[i].Stations[j]->AreaStart != "FB")
			//{
			//	for (int k = 0; k < FL.AreaOrder.size(); k++)
			//	{
			//		if (FL.TheWorkArea[i].AreaName == FL.AreaOrder[k])
			//		{
			//			k = FL.AreaOrder.size(); //breaking out of loop
			//		}
			//		else
			//		{
			//			FL.TheWorkArea[i].Stations[j]->WorkAreasCompleted[FL.AreaOrder[k]] = true;
			//		}
			//	}
			//}
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
					FL.TheWorkArea[i].Stations[j]->TimeLeft -= ((FL.TheWorkArea[i].Stations[j]->TimeLeft) / (FL.TheWorkArea[i].Stations.size() - 1))* j;
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
						//getline(ReadUnitFile, line);

						line = GenerateUnitList(FL);
						if (FL.userInputGenerator == 1 || FL.userInputGenerator == 2) //after generation, increment counter unless using the random generator
						{
							FL.unitGeneratorCounter++;
						}
						//TestUnit = CheckUnitType(line, FL);
						//Assign Unit ID number

						UnitPointer = new Unit;
						*(UnitPointer) = CheckUnitType(line, FL);
						UnitPointer->WorkAreasCompleted["FB"] = true;
						//Assign Unit ID number
						UnitPointer->IDnum = FL.AssignUnitID;
						FL.AssignUnitID++;
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

void MoveInFlowline(FlowLine &FL, int &i, int &j, double & WorkdayTime)
{
	if (i == 0 && j == 0) //we're at the start of the line, set i to -1 in order to break out of do loop
	{
		i = -1;
		j = 0;
		WorkdayTime -= FL.WorkTime;
		FL.WorkDayCounter += FL.WorkTime;

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
	//Reset unit fail checks
	UnitFailCheckReset(*(FL.TheWorkArea[i].Stations[j]));
	if (FL.TheWorkArea[i + 1].OverFlow.size() < FL.TheWorkArea[i + 1].MaxOverFlowSize)
	{
		FL.TheWorkArea[i + 1].OverFlow.push_back(FL.TheWorkArea[i].Stations[j]);
		FL.TheWorkArea[i].Stations[j] = nullptr;
	}
	else
	{
		//FL.TheWorkArea[i].Stations[j]->TotalUnitDownTime++;
	}

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

void SimulateFlowHelper(FlowLine &FL, ifstream & ReadUnitFile, int continueSim, int const ListSimulator)
{
	int CheckAreaDown;
	int unitCounter = 0;
	bool checkAreaDown = false;
	Unit TestUnit;


	if (continueSim == 0)
	{
		FillFlowLine(FL, TestUnit, ReadUnitFile);
	}
	while (FL.WorkDay > 0)
	{
		CheckAreaDown = FL.WorkDay;
		if (ListSimulator == 2 || ListSimulator == 3)
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
		if (ListSimulator == 2 || ListSimulator == 3)
		{
			PrintFlowLine(FL);
			if (ListSimulator == 2)
			{
				system("pause");
				system("cls");
			}
		}
	}
	if (ListSimulator == 1)
	{
		PrintFlowLine(FL);
		system("pause");
	}
	return;
}

void SimulateFlowHelperTester(FlowLine &FL, ifstream & ReadUnitFile)
{
	for (vector<WorkArea>::iterator it = FL.TheWorkArea.begin(); it != FL.TheWorkArea.end(); ++it)
	{
		it->MaxOverFlowSize = 1000;
	}
	FL.UnitMode = 1;
	FL.userInputGenerator = 1;
	int CheckAreaDown;
	int unitCounter = 0;
	bool checkAreaDown = false;
	Unit TestUnit;

	FillFlowLine(FL, TestUnit, ReadUnitFile);
	while (FL.WorkDay > 0)
	{
		CheckAreaDown = FL.WorkDay;

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
	}
	return;
}

void AddUnitToFlow(FlowLine &FL, ifstream & ReadUnitFile, int i, int j)
{
	Unit TempUnit;
	string TempLine;
	TempLine = GenerateUnitList(FL);
	if (FL.userInputGenerator == 1 || FL.userInputGenerator == 2)
	{
		FL.unitGeneratorCounter++;
	}
	//getline(ReadUnitFile, TempLine); use this if you want to generate units from a file
	TempUnit = CheckUnitType(TempLine, FL);

	//Assign unit a unique ID number
	TempUnit.IDnum = FL.AssignUnitID;
	FL.AssignUnitID++;


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
				else //ULT
				{
					if (j = FL.TheWorkArea[i].Stations.size()) //last test station in ULT
					{
						bool ULTEmpty = false;
						for (int k = 0; k < FL.TheWorkArea[i].Stations.size(); k++)
						{
							if (FL.TheWorkArea[i].Stations[k] != nullptr)
							{
								ULTEmpty = true;
							}
						}
						if (ULTEmpty == true)
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
					//(*it1)->TotalUnitDownTime += FL.WorkTime;
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
			cout << endl << FL.TheWorkArea[i].AreaName << " area Down! (" << FL.WorkDay << ")" << endl;
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
void UnitTSHelper(FlowLine & FL)
{
	int n = 0;
	for (list<Unit*>::iterator it = FL.ReWork.begin(); it != FL.ReWork.end();) //iterating backwards because we may pop things from the list
	{
		if ((*it)->TotalUnitTroubleShootTime > 0)
		{
			(*it)->TotalUnitTroubleShootTime--;
			++it;
		}
		else //TotalUnitTroubleShootTime == 0
		{
			n = FL.OrderOfWorkAreas[(*it)->LastAreaBeforeTS];

			if (FL.TheWorkArea[n].Stations[0] == nullptr)
			{
				cout << "Unit " << (*it)->UnitName << " is moving from TS to " << FL.TheWorkArea[n].AreaName << endl;
				FL.TheWorkArea[n].Stations[0] = (*it);
				FL.TheWorkArea[n].Stations[0]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[n].AreaName, *(FL.TheWorkArea[n].Stations[0]), FL, FL.TheWorkArea[n].BuildT[0]);
				it = FL.ReWork.erase(it); //removing unit from rework list

			}
			//move to overflow if you can
			else if (FL.TheWorkArea[n].OverFlow.size() < FL.TheWorkArea[n].MaxOverFlowSize)
			{
				cout << "Unit " << (*it)->UnitName << " is moving from TS to " << FL.TheWorkArea[n].AreaName << " overflow." << endl;
				FL.TheWorkArea[n].OverFlow.push_back((*it));
				it = FL.ReWork.erase(it);
			}
			else
			{
				++it;
				//OVERFLOW DOWNTIME
			}
		}

		if (FL.ReWork.empty())
		{
			break;
		}
	}
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

	if (!FL.ReWork.empty()) //if the TS area is not empty, then we will check TS
	{
		UnitTSHelper(FL);
	}

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

void SimDataOutputs(FlowLine FL, ofstream & SimData)
{
	int totalstations = 0;
	int totaloverflow = 0;
	for (vector<WorkArea>::iterator it = FL.TheWorkArea.begin(); it != FL.TheWorkArea.end(); ++it)
	{
		totalstations += it->Stations.size();
	}
	for (vector<WorkArea>::iterator it = FL.TheWorkArea.begin(); it != FL.TheWorkArea.end(); ++it)
	{
		totaloverflow += it->OverFlow.size();
	}
	//SimData << "Units, People, UA OF, Non-UA OF, ULT OF\n";
	SimData << FL.CompletedUnits.size() << ",";
	SimData << totalstations << ",";
	SimData << FL.TheWorkArea[1].OverFlow.size() << ",";
	SimData << totaloverflow - FL.TheWorkArea[1].OverFlow.size() << ",";
	SimData << FL.TheWorkArea[4].OverFlow.size() << ",";
	SimData << "\n";
	

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

void ProgramInputsFromUser(FlowLine &FL, int &ListSimulator)
{
	int OverFlowType;
	cout << "Please select between \n";
	cout << "(1) Unlimited overflows\n";
	cout << "(2) Limited overflows \n";
	cin >> OverFlowType;
	if (OverFlowType == 1)
	{
		for (vector<WorkArea>::iterator it = FL.TheWorkArea.begin(); it != FL.TheWorkArea.end(); ++it)
		{
			it->MaxOverFlowSize = 1000;
		}
	}
	system("cls");

	cout << "Please select between \n";
	cout << "(1) Single Door mode\n";
	cout << "(2) Dual Door mode\n";
	cin >> FL.UnitMode;
	system("cls");
	cout << "Please select between \n(1) Simulate entire day, \n(2) Simulate by minute \n(3) Test Mode\n";
	cin >> ListSimulator;
	system("cls");
	cout << "Please select how you want units to be generated in the line: \n(1) Four of each unit \n(2) One of each unit \n(3) Random Units\n";
	cin >> FL.userInputGenerator;
	system("cls");

}

vector<string> CreateAreaOrderString(FlowLine & FL)
{
	vector<string> AreaOrder;
	for (int i = 0; i < FL.TheWorkArea.size(); i++)
	{
		AreaOrder.push_back(FL.TheWorkArea[i].AreaName);
	}

	return AreaOrder;
}
void CreateFlowLineAreaOrderMap(FlowLine & FL)
{
	int count = 0;
	for (vector<WorkArea>::iterator it = FL.TheWorkArea.begin(); it != FL.TheWorkArea.end(); ++it)
	{
		FL.OrderOfWorkAreas[(*it).AreaName] = count;
		count++;
	}
}
int main(void)
{
	FlowLine TestFlow;
	FlowLine *TFlow;
	vector<string> UnitList;
	ifstream ReadUnitFile("Units2.txt");
	ofstream UnitOutputs("Output.csv", fstream::app); //USE THIS if you don't want to overwrite 
	ofstream SimData("SimData.csv", fstream::app);
	//ofstream UnitOutputs("Output.csv");

	srand(std::time(nullptr)); //seed for rand



	CreateFlowLine(TestFlow.TheWorkArea);
    CreateFlowLineAreaOrderMap(TestFlow);
	CreateUnitList(TestFlow);

	//	PrintFlowLine(TestFlow);

	int continueSim = 0;
	int ListSimulator = 0;
	system("cls");
	do
	{
		ProgramInputsFromUser(TestFlow, ListSimulator);
		SimulateFlowHelper(TestFlow, ReadUnitFile, continueSim, ListSimulator);

		//system("cls");
		cout << "Please select between: \n";
		cout << "(0) End the simulation " << endl;
		cout << "(1) Sim 10 days" << endl;
		cout << "Otherwise press any other key to return to previous menus" << endl;
		cin >> continueSim;
		if (continueSim == 1)
		{
			TestFlow.WorkDay = 480 * 1000;
		}
		else
		{
			TestFlow.WorkDay = 480;
		}

		system("pause");
	} while (continueSim != 0);

	UnitTimeOutputs(TestFlow, UnitOutputs);

	CreateFlowLine(TestFlow.TheWorkArea);

	/*TFlow = new FlowLine;
	for (int i = 0; i < 10; i++)
	{
		CreateFlowLine(TFlow->TheWorkArea);
		CreateFlowLineAreaOrderMap(*(TFlow));
		CreateUnitList(*(TFlow));
		SimulateFlowHelperTester(*(TFlow), ReadUnitFile);
		SimDataOutputs(*(TFlow), SimData);
		PrintFlowLine(*(TFlow));
		delete TFlow;
		TFlow = nullptr;
		TFlow = new FlowLine;
	}*/

	SimData.close();
	UnitOutputs.close();
	ReadUnitFile.close();
	system("pause");

	return 0;
}