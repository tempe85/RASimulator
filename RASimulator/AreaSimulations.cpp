#include "WorkArea.h"

void SimulateFormboard(FlowLine &FL, int &i, int &j, ifstream & ReadUnitFile)
{
	if (CheckIfFinished(FL.TheWorkArea[i].Stations[j]->TimeLeft) == true)
	{
		//check if first UA spot is open
		if (FL.TheWorkArea[i + 1].Stations[0] == nullptr)
		{
			if (UnitFailCheck(FL.TheWorkArea[i + 1]) == false)
			{
				
				FL.TheWorkArea[i + 1].Stations[0] = FL.TheWorkArea[i].Stations[j];
				FL.TheWorkArea[i].Stations[j] = nullptr;
				FL.TheWorkArea[i + 1].Stations[0]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i + 1].AreaName, *(FL.TheWorkArea[i + 1].Stations[0]), FL, FL.TheWorkArea[i + 1].BuildT[0]);
				//Rework CHECK for units moving to UA
				UnitReworkCheck(FL.TheWorkArea[i + 1], *(FL.TheWorkArea[i + 1].Stations[0]), FL.TheWorkArea[i + 1].Stations[0]->TimeLeft);
			}
			else
			{
				SendUnitToTS(FL, i, j, false); //unit added to rework tracker and removed from Flowline
			}
		}
		else // UA spot is not open
		{
			//Move to UA overflow
			MoveToOverFlow(FL, i, j);
		}
		AddUnitToFlow(FL, ReadUnitFile, i, j);
		MoveInFlowline(FL, i, j, FL.WorkDay);
	}
	else //not finished, timeleft > 0
	{
		AdjustTimeLeft(FL, FL.TheWorkArea[i].ValueAdded, *(FL.TheWorkArea[i].Stations[j]));
		MoveInFlowline(FL, i, j, FL.WorkDay);

	}
}
//NEED TO FIX FUNCTION, at least add an exception for ULT. Units from overflow need to start at start of line ex. if name is ult s = j, otherwise s = 0
void SimulateBasicArea(FlowLine &FL, int &i, int &j)
{
	//If the Station is already NULL we must skip it
	if (FL.TheWorkArea[i].Stations[j] == nullptr)
	{
		OverFlowManager(FL, i, j);
	}
	else//Don't Skip
	{
		if (CheckIfFinished(FL.TheWorkArea[i].Stations[j]->TimeLeft) == true)
		{
			//first check if it's at the end of an area
			if (j == FL.TheWorkArea[i].Stations.size() - 1)
			{
				//check if it's in CLT and needs settings
				int k = i; 
				//needs to be moved to a new area, only if a spot is available
				if (FL.TheWorkArea[i + 1].Stations[0] == nullptr) //there is a free spot
				{
					//first check if the unit fails
					if (UnitFailCheck(FL.TheWorkArea[i + 1]) == false)
					{
						
						FL.TheWorkArea[i + 1].Stations[0] = FL.TheWorkArea[i].Stations[j]; //Moving pointer to the next area spot
						FL.TheWorkArea[i].Stations[j] = nullptr; //Current spot is now free
						//Need Timeleft for a unit reset
						FL.TheWorkArea[i + 1].Stations[0]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i + 1].AreaName, *(FL.TheWorkArea[i + 1].Stations[0]), FL, FL.TheWorkArea[i + 1].BuildT[0]);
						//Check if rework needed
						UnitReworkCheck(FL.TheWorkArea[i + 1], *(FL.TheWorkArea[i + 1].Stations[0]), FL.TheWorkArea[i + 1].Stations[0]->TimeLeft);
					}
					else //Unit failed is true
					{
						SendUnitToTS(FL, i, j, false);
					}
					//still need to change i and j
					i = k;
					OverFlowManager(FL, i, j);
			
				}
				else //Next spot is not free
				{
					//Move to Next area overflow
					FL.TheWorkArea[i + 1].OverFlow.push_back(FL.TheWorkArea[i].Stations[j]);
					FL.TheWorkArea[i].Stations[j] = nullptr;
					//still need to change i and j
					OverFlowManager(FL, i, j);
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
					//May put rework check here in the future??
					OverFlowManager(FL, i, j);
				}
				else //next station is not free
				{
					//Need to add to area downtime
					FL.TheWorkArea[i].Stations[j]->TotalUnitDownTime += FL.WorkDay;
					MoveInFlowline(FL, i, j, FL.WorkDay);
				}
			}
		}
		else //not finished, timeleft > 0
		{
			//Add to work time completed
			AdjustTimeLeft(FL, FL.TheWorkArea[i].ValueAdded, *(FL.TheWorkArea[i].Stations[j]));
			MoveInFlowline(FL, i, j, FL.WorkDay);
		}
	}
}

void SimulateUA(FlowLine &FL, int &i, int &j)
{
	//If the Station is already NULL we must skip it
	if (FL.TheWorkArea[i].Stations[j] == nullptr)
	{
		OverFlowManager(FL, i, j);
	}
	else//Don't Skip
	{
		if (CheckIfFinished(FL.TheWorkArea[i].Stations[j]->TimeLeft) == true)
		{
			//first check if it's at the end of an area
			if (j == FL.TheWorkArea[i].Stations.size() - 1)
			{
				//check if it's in CLT and needs settings
				int k = i;
				//needs to be moved to a new area, only if a spot is available
				if (FL.TheWorkArea[i + 1].Stations[0] == nullptr) //there is a free spot
				{
					//first check if the unit fails
					if (UnitFailCheck(FL.TheWorkArea[i + 1]) == false)
					{
						FL.TheWorkArea[i + 1].Stations[0] = FL.TheWorkArea[i].Stations[j]; //Moving pointer to the next area spot
						FL.TheWorkArea[i].Stations[j] = nullptr; //Current spot is now free
						//Need Timeleft for a unit reset
						FL.TheWorkArea[i + 1].Stations[0]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i + 1].AreaName, *(FL.TheWorkArea[i + 1].Stations[0]), FL, FL.TheWorkArea[i + 1].BuildT[0]);
						//Check if unit needs rework
						UnitReworkCheck(FL.TheWorkArea[i + 1], *(FL.TheWorkArea[i + 1].Stations[0]), FL.TheWorkArea[i + 1].Stations[0]->TimeLeft);
					}
					else //Unit failed is true
					{
						SendUnitToTS(FL, i, j, false);
					}
					//still need to change i and j
					i = k;
					OverFlowManager(FL, i, j);

				}
				else //Next spot is not free
				{
					//Move to Next area overflow
					FL.TheWorkArea[i + 1].OverFlow.push_back(FL.TheWorkArea[i].Stations[j]);
					FL.TheWorkArea[i].Stations[j] = nullptr;
					//still need to change i and j
					OverFlowManager(FL, i, j);
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
					OverFlowManager(FL, i, j);
				}
				else //next station is not free
				{
					//Need to add to area downtime
					FL.TheWorkArea[i].Stations[j]->TotalUnitDownTime += FL.WorkDay;
					MoveInFlowline(FL, i, j, FL.WorkDay);
				}
			}
		}
		else //not finished, timeleft > 0
		{
			AdjustTimeLeft(FL, FL.TheWorkArea[i].ValueAdded, *(FL.TheWorkArea[i].Stations[j]));
			MoveInFlowline(FL, i, j, FL.WorkDay);
		}
	}
}

void SimulateCLT(FlowLine &FL, int &i, int &j)
{
	//If the Station is already NULL we must skip it
	if (FL.TheWorkArea[i].Stations[j] == nullptr)
	{
		OverFlowManager(FL, i, j);
	}
	else//Don't Skip
	{
		if (CheckIfFinished(FL.TheWorkArea[i].Stations[j]->TimeLeft) == true)
		{
			//first check if it's at the end of an area
			if (j == FL.TheWorkArea[i].Stations.size() - 1)
			{
				//check if it's in CLT and needs settings
				int k = i;
				k = CheckSettings(FL, k, j);
				//needs to be moved to a new area, only if a spot is available
				if (FL.TheWorkArea[k + 1].Stations[0] == nullptr) //there is a free spot
				{
					//first check if the unit fails
					if (UnitFailCheck(FL.TheWorkArea[k + 1]) == false)
					{
						FL.TheWorkArea[k + 1].Stations[0] = FL.TheWorkArea[i].Stations[j]; //Moving pointer to the next area spot
						FL.TheWorkArea[i].Stations[j] = nullptr; //Current spot is now free
						//Need Timeleft for a unit reset

						//FL.TheWorkArea[i + 1].Stations[0]->TimeLeft = FL.TheWorkArea[i + 1].BuildT[0];

						FL.TheWorkArea[k + 1].Stations[0]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[k + 1].AreaName, *(FL.TheWorkArea[k + 1].Stations[0]), FL, FL.TheWorkArea[k + 1].BuildT[0]);
						//double CalculateTimeLeft(string AreaName, Unit MOT, FlowLine FL, double BuildTime)
						//check if Unit needs rework
						UnitReworkCheck(FL.TheWorkArea[k + 1], *(FL.TheWorkArea[k + 1].Stations[0]), FL.TheWorkArea[k + 1].Stations[0]->TimeLeft);
					}
					else //Unit failed is true
					{
						SendUnitToTS(FL, i, j, false);
					}

					OverFlowManager(FL, i, j);

				}
				else //Next spot is not free
				{
					//Move to Next area overflow
					FL.TheWorkArea[k + 1].OverFlow.push_back(FL.TheWorkArea[i].Stations[j]);
					FL.TheWorkArea[i].Stations[j] = nullptr;

					//Need to add to downtime
					//still need to change i and j
					OverFlowManager(FL, i, j);
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
					FL.TheWorkArea[i].Stations[j + 1]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i].AreaName, *(FL.TheWorkArea[i].Stations[j + 1]), FL, FL.TheWorkArea[i].BuildT[j + 1]);
					OverFlowManager(FL, i, j);
				}
				else //next station is not free
				{
					//Need to add to area downtime
					FL.TheWorkArea[i].Stations[j]->TotalUnitDownTime += FL.WorkDay;
					MoveInFlowline(FL, i, j, FL.WorkDay);
				}
			}
		}
		else //not finished, timeleft > 0
		{
			AdjustTimeLeft(FL, FL.TheWorkArea[i].ValueAdded, *(FL.TheWorkArea[i].Stations[j]));
			MoveInFlowline(FL, i, j, FL.WorkDay);
		}
	}
}

void SimulateAVS(FlowLine &FL, int &i, int &j)
{
	int s = 0;
	//If the Station is already NULL we must skip it
	if (FL.TheWorkArea[i].Stations[j] == nullptr) 
	{
		OverFlowManager(FL, i, j);
	}
	else //Don't Skip
	{
		if (CheckIfFinished(FL.TheWorkArea[i].Stations[j]->TimeLeft) == true)
		{
			//first check if it's at the end of an area
			if (j == FL.TheWorkArea[i].Stations.size() - 1)
			{
				//needs to be moved to a new area, only if a spot is available
				if (OpenSpotInNextWorkArea(FL, i, s)) //this is a AVS specific thing, it can move to any open spot in ULT
				{
					//first check if the unit fails
					if (UnitFailCheck(FL.TheWorkArea[i + 1]) == false)
					{
						FL.TheWorkArea[i + 1].Stations[s] = FL.TheWorkArea[i].Stations[j]; //Moving pointer to the next area spot
						FL.TheWorkArea[i].Stations[j] = nullptr; //Current spot is now free
						//Need Timeleft for a unit reset

						//FL.TheWorkArea[i + 1].Stations[0]->TimeLeft = FL.TheWorkArea[i + 1].BuildT[0];

						FL.TheWorkArea[i + 1].Stations[s]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i + 1].AreaName, *(FL.TheWorkArea[i + 1].Stations[s]), FL, FL.TheWorkArea[i + 1].BuildT[0]);
						UnitReworkCheck(FL.TheWorkArea[i + 1], *(FL.TheWorkArea[i + 1].Stations[s]), FL.TheWorkArea[i + 1].Stations[s]->TimeLeft);
						//double CalculateTimeLeft(string AreaName, Unit MOT, FlowLine FL, double BuildTime)
					}
					else //Unit failed is true
					{
						SendUnitToTS(FL, i, j, false);
					}
					//still need to change i and j
					OverFlowManager(FL, i, j);

				}
				else //No spot in ULT is free
				{
					//Move to Next area overflow
					FL.TheWorkArea[i + 1].OverFlow.push_back(FL.TheWorkArea[i].Stations[j]);
					FL.TheWorkArea[i].Stations[j] = nullptr;

					//Need to add to downtime
					//FL.TheWorkArea[i].downtime += FL.WorkTime; //No need with overflow at this time
					//still need to change i and j
					OverFlowManager(FL, i, j);
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
					OverFlowManager(FL, i, j);
				}
				else //next station is not free
				{
					//Need to add to area downtime
					MoveInFlowline(FL, i, j, FL.WorkDay);
				}
			}
		}
		else //not finished, timeleft > 0
		{
			AdjustTimeLeft(FL, FL.TheWorkArea[i].ValueAdded, *(FL.TheWorkArea[i].Stations[j]));
			MoveInFlowline(FL, i, j, FL.WorkDay);
		}
	}
}

void SimulateULT(FlowLine &FL, int &i, int &j)
{
	//If the Station is already NULL we must skip it
	if (FL.TheWorkArea[i].Stations[j] == nullptr) //In ULT it doesn't matter which station we're at, can pick up overflow regardless
	{
		OverFlowManager(FL, i, j);
	}
	else //Don't Skip
	{
		if (CheckIfFinished(FL.TheWorkArea[i].Stations[j]->TimeLeft) == true)
		{
			//needs to be moved to a new area, only if a spot is available
			if (FL.TheWorkArea[i + 1].Stations[0] == nullptr) //there is a free spot
			{
				//first check if the unit fails
				if (UnitFailCheck(FL.TheWorkArea[i + 1]) == false)
				{
					FL.TheWorkArea[i + 1].Stations[0] = FL.TheWorkArea[i].Stations[j]; //Moving pointer to the next area spot
					FL.TheWorkArea[i].Stations[j] = nullptr; //Current spot is now free
					//Need Timeleft for a unit reset

					//FL.TheWorkArea[i + 1].Stations[0]->TimeLeft = FL.TheWorkArea[i + 1].BuildT[0];

					FL.TheWorkArea[i + 1].Stations[0]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i + 1].AreaName, *(FL.TheWorkArea[i + 1].Stations[0]), FL, FL.TheWorkArea[i + 1].BuildT[0]);
					UnitReworkCheck(FL.TheWorkArea[i + 1], *(FL.TheWorkArea[i + 1].Stations[0]), FL.TheWorkArea[i + 1].Stations[0]->TimeLeft);
					//double CalculateTimeLeft(string AreaName, Unit MOT, FlowLine FL, double BuildTime)
				}
				else //Unit failed is true
				{
					SendUnitToTS(FL, i, j, false);
				}
				//still need to change i and j
				OverFlowManager(FL, i, j);

				}
			else //Next spot is not free
			{
				//Move to Next area overflow
				FL.TheWorkArea[i + 1].OverFlow.push_back(FL.TheWorkArea[i].Stations[j]);
				FL.TheWorkArea[i].Stations[j] = nullptr;

				//Need to add to downtime
				//FL.TheWorkArea[i].downtime += FL.WorkTime; //No need with overflow at this time
				//still need to change i and j
				OverFlowManager(FL, i, j);
			}
		}
		else //not finished, timeleft > 0
		{
			AdjustTimeLeft(FL, FL.TheWorkArea[i].ValueAdded, *(FL.TheWorkArea[i].Stations[j]));
			MoveInFlowline(FL, i, j, FL.WorkDay);
		}
	}
}

void SimulateBuild(FlowLine &FL, int &i, int &j)
{
	int LastStation = FL.TheWorkArea[i].Stations.size() - 1;

	if (FL.TheWorkArea[i].Stations[j] == nullptr)
	{
		OverFlowManager(FL, i, j);
	}
	else //don't skip
	{
		if (CheckIfFinished(FL.TheWorkArea[i].Stations[j]->TimeLeft) == true && j == LastStation)
		{
			//first check if it's at the end of an area
			if (j == FL.TheWorkArea[i].Stations.size() - 1)
			{
				//needs to be moved to a new area, only if a spot is available
				if (FL.TheWorkArea[i + 1].Stations[0] == nullptr) //there is a free spot
				{
					//first check if the unit fails
					if (UnitFailCheck(FL.TheWorkArea[i + 1]) == false)
					{
						FL.TheWorkArea[i + 1].Stations[0] = FL.TheWorkArea[i].Stations[j]; //Moving pointer to the next area spot
						FL.TheWorkArea[i].Stations[j] = nullptr; //Current spot is now free
						//Need Timeleft for a unit reset

						FL.TheWorkArea[i + 1].Stations[0]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i + 1].AreaName, *(FL.TheWorkArea[i + 1].Stations[0]), FL, FL.TheWorkArea[i + 1].BuildT[0]);
						UnitReworkCheck(FL.TheWorkArea[i + 1], *(FL.TheWorkArea[i + 1].Stations[0]), FL.TheWorkArea[i + 1].Stations[0]->TimeLeft);
						//double CalculateTimeLeft(string AreaName, Unit MOT, FlowLine FL, double BuildTime)
					}
					else //Unit failed is true
					{
						SendUnitToTS(FL, i, j, false);
					}
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
		}
		else if (CheckIfFinished(FL.TheWorkArea[i].Stations[j]->TimeLeft) == true && j != LastStation)
		{
			//Check to see if station 6 is empty
			if (FL.TheWorkArea[i].Stations[LastStation] == nullptr)
			{
				FL.TheWorkArea[i].Stations[LastStation] = FL.TheWorkArea[i].Stations[j]; //move unit to station 6
				FL.TheWorkArea[i].Stations[j] = nullptr;
				FL.TheWorkArea[i].Stations[LastStation]->TimeLeft += FL.BuildStation6;
			}
			else //move to first open spot if possible
			{
				if (j < FL.TheWorkArea[i].Stations.size() - 2)
				{
					for (int k = FL.TheWorkArea[i].Stations.size() - 2; k > j; k--)
					{
						if (FL.TheWorkArea[i].Stations[k] == nullptr)
						{
							FL.TheWorkArea[i].Stations[k] = FL.TheWorkArea[i].Stations[j];
							FL.TheWorkArea[i].Stations[j] = nullptr;
							k = j; //break out of loop
						}
					}
				}
			}
			MoveInFlowline(FL, i, j, FL.WorkDay);
		}
		else //not finished, timeleft > 0
		{
			AdjustTimeLeft(FL, FL.TheWorkArea[i].ValueAdded, *(FL.TheWorkArea[i].Stations[j]));

			//Not finished but is in the build line, check if any spot in build line, except station 6 is free (starting at the end of build), also can't be at the end of the build line
			if (j < FL.TheWorkArea[i].Stations.size() - 2)
			{
				for (int k = FL.TheWorkArea[i].Stations.size() - 2; k > j; k--)
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
			//MoveInFlowline(FL, i, j, FL.WorkDay);
			OverFlowManager(FL, i, j);
		}
	}
}

void SimulatePkg(FlowLine & FL, int & i, int & j)
{
	if (FL.TheWorkArea[i].Stations[j] == nullptr)
	{
		OverFlowManager(FL, i, j);
	}
	else //don't skip
	{
		//Add to total build time for unit
		if (CheckIfFinished(FL.TheWorkArea[i].Stations[j]->TimeLeft) == true)
		{
			//check if at the end of flowline
			if (j == FL.TheWorkArea[i].Stations.size() - 1) //end of flowline
			{
				//Move Unit to completed unit list
				FL.CompletedUnits.push_back(FL.TheWorkArea[i].Stations[j]);
				FL.TheWorkArea[i].Stations[j] = nullptr;
				MoveInFlowline(FL, i, j, FL.WorkDay);
				FL.CompletedUnitCounter += 1;
			}
			else //not end of flowline but is finished, we must see if it can be moved to a new spot
			{
				//Check if next spot in the work area is free
				if (FL.TheWorkArea[i].Stations[j + 1] == nullptr) //next spot is free
				{
					//move pointer
					FL.TheWorkArea[i].Stations[j + 1] = FL.TheWorkArea[i].Stations[j];
					FL.TheWorkArea[i].Stations[j] = nullptr; //free current spot
					//Need to reset time left
					//FL.TheWorkArea[i].Stations[j + 1]->TimeLeft = FL.TheWorkArea[i].BuildT[j + 1];
					FL.TheWorkArea[i].Stations[j + 1]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i].AreaName, *(FL.TheWorkArea[i].Stations[j + 1]), FL, FL.TheWorkArea[i].BuildT[j + 1]);
					OverFlowManager(FL, i, j);
				}
				else //next station is not free
				{
					//Need to add to area downtime
					MoveInFlowline(FL, i, j, FL.WorkDay);
				}
			}
		}
		else //not finished, timeleft > 0
		{
			AdjustTimeLeft(FL, FL.TheWorkArea[i].ValueAdded, *(FL.TheWorkArea[i].Stations[j]));
			//Moving to next spot in line
			MoveInFlowline(FL, i, j, FL.WorkDay);
		}
	}
}

void AdjustTimeLeft(FlowLine & FL, bool ValueAdded, Unit & curUnit)
{
	curUnit.TimeLeft -= FL.WorkTime;
	//TimeLeft -= FL.WorkTime;
	if (ValueAdded)
	{
		FL.TimeValueAdded += FL.WorkTime;
		curUnit.TotalUnitValueTime += FL.WorkTime;
		//Need to adjust in case TimeLeft is less than 0
		if (curUnit.TimeLeft < 0)
		{
			FL.TimeValueAdded += curUnit.TimeLeft;
		}
	}
	else //No value added
	{
		FL.TimeNoValueAdded += FL.WorkTime;
		curUnit.TotalUnitNoValueTime += FL.WorkTime;
		//Need to adjust in case TimeLeft is less than 0
		if (curUnit.TimeLeft < 0)
		{
			FL.TimeNoValueAdded += curUnit.TimeLeft;
		}
	}
}

bool OpenSpotInNextWorkArea(FlowLine & FL, const int & i, int &s)
{
	if (FL.TheWorkArea[i].Stations.size() == 1) //if Workarea has only one station
	{
		if (FL.TheWorkArea[i].Stations[0] == nullptr)
		{
			s = 0;
			return true;
		}
		s = 0;
		return false;
	}
	for (int k = FL.TheWorkArea[i].Stations.size() - 1; k > 0; k--)
	{
		if (FL.TheWorkArea[i].Stations[k] == nullptr)
		{
			s = k;
			return true;
		}
	}
	s = 0;
	return false;
}

int CheckSettings(FlowLine const FL, int &k, const int j)
{
	if (FL.TheWorkArea[k].Stations[j]->Settings == true)
	{
		return k;
	}
	else
	{
		k += 1; //skipping settings
		return k;
	}

	return k;
}

void OverFlowManager(FlowLine &FL, int &i, int &j)
{
	if (FL.TheWorkArea[i].Stations[j] == nullptr)
	{
		if (j == 0 || FL.TheWorkArea[i].AreaName == "ULT")
		{
			if (!OverFlowIsEmpty(FL, i)) // if the overflow for the area is not empty
			{
				if (FL.TheWorkArea[i].Stations[j] == nullptr) //if current area is empty
				{
					//check if unit fails
					if (UnitFailCheck(FL.TheWorkArea[i]) == false)
					{
						FL.TheWorkArea[i].Stations[j] = FL.TheWorkArea[i].OverFlow.front();
						FL.TheWorkArea[i].Stations[j]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i].AreaName, *(FL.TheWorkArea[i].Stations[j]), FL, FL.TheWorkArea[i].BuildT[0]);
						RemoveFirstOverFlowUnit(FL, i);
					}
					else //unit failed
					{
						SendUnitToTS(FL, i, j, true);
					}
				}
			}
		}
	}
	MoveInFlowline(FL, i, j, FL.WorkDay);
}