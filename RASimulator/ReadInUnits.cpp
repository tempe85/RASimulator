//#include "units.h"
//
//ofstream &operator<< (ofstream &lhs, Unit &rhs)
//{
//
//
//
//	return lhs;
//}

//FlowLine SimulateFlowLine(FlowLine &FL, ifstream & ReadUnitFile, int &i, int &j)
//{
//	//int i = FL.TheWorkArea.size() - 1;
//	//int j = FL.TheWorkArea[i].Stations.size() - 1;
//	//Unit TempUnit;
//	//string TempLine;
//
//	//
//	//We're starting at the end of the flowline
//	do
//	{
//		//If the Station is already NULL we must skip it
//		if (FL.TheWorkArea[i].Stations[j] == nullptr)
//		{
//			MoveInFlowline(FL, i, j, FL.WorkDay);
//		}
//		else //Don't skip
//		{
//			if (CheckIfFinished(FL.TheWorkArea[i].Stations[j]->TimeLeft) == true)
//			{
//				//check if it's in FB
//				if (FL.TheWorkArea[i].AreaName == "FB")
//				{
//					//check if first UA spot is open
//					if (FL.TheWorkArea[i + 1].Stations[0] == nullptr)
//					{
//						if (UnitFailCheck(FL.TheWorkArea[i + 1]) == false)
//						{
//							FL.TheWorkArea[i + 1].Stations[0] = FL.TheWorkArea[i].Stations[j];
//							FL.TheWorkArea[i].Stations[j] = nullptr;
//							FL.TheWorkArea[i + 1].Stations[0]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i + 1].AreaName, *(FL.TheWorkArea[i + 1].Stations[0]), FL, FL.TheWorkArea[i + 1].BuildT[0]);
//						}
//						else
//						{
//							SendUnitToRework(FL, i, j, false); //unit added to rework tracker and removed from Flowline
//						}
//					}
//					else // UA spot is not open
//					{
//						//Move to UA overflow
//						MoveToOverFlow(FL, i, j);
//					}
//					AddUnitToFlow(FL, ReadUnitFile, i, j);
//					MoveInFlowline(FL, i, j, FL.WorkDay);
//				}
//				//check if at the end of flowline
//				else if (i == FL.TheWorkArea.size() - 1 && j == FL.TheWorkArea[i].Stations.size() - 1) //end of flowline
//				{
//					//pop unit out of the Flow Line as it is completed, first memory must be freed
//					delete FL.TheWorkArea[i].Stations[j];
//					FL.TheWorkArea[i].Stations[j] = nullptr;
//					MoveInFlowline(FL, i, j, FL.WorkDay);
//					FL.CompletedUnitCounter += 1;
//
//				}
//				else //not end of flowline but is finished, we must see if it can be moved to a new spot
//				{
//					//first check if it's at the end of an area
//					if (j == FL.TheWorkArea[i].Stations.size() - 1)
//					{
//						//needs to be moved to a new area, only if a spot is available
//						if (FL.TheWorkArea[i + 1].Stations[0] == nullptr) //there is a free spot
//						{
//							//first check if the unit fails
//							if (UnitFailCheck(FL.TheWorkArea[i + 1]) == false)
//							{
//								FL.TheWorkArea[i + 1].Stations[0] = FL.TheWorkArea[i].Stations[j]; //Moving pointer to the next area spot
//								FL.TheWorkArea[i].Stations[j] = nullptr; //Current spot is now free
//								//Need Timeleft for a unit reset
//
//								//FL.TheWorkArea[i + 1].Stations[0]->TimeLeft = FL.TheWorkArea[i + 1].BuildT[0];
//
//								FL.TheWorkArea[i + 1].Stations[0]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i + 1].AreaName, *(FL.TheWorkArea[i + 1].Stations[0]), FL, FL.TheWorkArea[i + 1].BuildT[0]);
//								//double CalculateTimeLeft(string AreaName, Unit MOT, FlowLine FL, double BuildTime)
//							}
//							else //Unit failed is true
//							{
//								SendUnitToRework(FL, i, j, false);
//							}
//							//still need to change i and j
//							MoveInFlowline(FL, i, j, FL.WorkDay);
//
//						}
//						else //Next spot is not free
//						{
//							//Move to Next area overflow
//							FL.TheWorkArea[i + 1].OverFlow.push_back(FL.TheWorkArea[i].Stations[j]);
//							FL.TheWorkArea[i].Stations[j] = nullptr;
//
//							//Need to add to downtime
//							//FL.TheWorkArea[i].downtime += FL.WorkTime; //No need with overflow at this time
//							//still need to change i and j
//							MoveInFlowline(FL, i, j, FL.WorkDay);
//						}
//					}
//					else if (FL.TheWorkArea[i].AreaName == "Build") //The build exception!
//					{
//						//it can skip over a unit if possible. Build is the only place where this can happen
//						if (FL.TheWorkArea[i + 1].Stations[0] == nullptr)
//						{
//							FL.TheWorkArea[i + 1].Stations[0] = FL.TheWorkArea[i].Stations[j];
//							FL.TheWorkArea[i].Stations[j] = nullptr;
//						}
//						else //Test is not open, so move to test overflow
//						{
//							MoveToOverFlow(FL, i, j);
//						}
//					}
//					else //Not at the end of an area, can't be moved to overflow! (unless in build line!)
//					{
//
//						//next spot is FL.TheWorkArea[i].Stations[j+1];
//						//Check if next spot in the work area is free
//						if (FL.TheWorkArea[i].Stations[j + 1] == nullptr) //next spot is free
//						{
//							//move pointer
//							FL.TheWorkArea[i].Stations[j + 1] = FL.TheWorkArea[i].Stations[j];
//							FL.TheWorkArea[i].Stations[j] = nullptr; //free current spot
//							//Need to reset time left
//							//FL.TheWorkArea[i].Stations[j + 1]->TimeLeft = FL.TheWorkArea[i].BuildT[j + 1];
//							FL.TheWorkArea[i].Stations[j + 1]->TimeLeft = CalculateTimeLeft(FL.TheWorkArea[i].AreaName, *(FL.TheWorkArea[i].Stations[j + 1]), FL, FL.TheWorkArea[i].BuildT[j + 1]);
//							MoveInFlowline(FL, i, j, FL.WorkDay);
//						}
//						else //next station is not free
//						{
//							//Need to add to area downtime
//							FL.TheWorkArea[i].downtime += FL.WorkTime;
//							MoveInFlowline(FL, i, j, FL.WorkDay);
//						}
//					}
//				}
//			}
//			else //not finished, timeleft > 0
//			{
//				FL.TheWorkArea[i].Stations[j]->TimeLeft -= FL.WorkTime; //worktime is the incremental change
//				//Add to work time completed
//				FL.WorkTimeFinished += FL.WorkTime;
//				//Need to adjust in case TimeLeft is less than 0
//				if (FL.TheWorkArea[i].Stations[j]->TimeLeft < 0)
//				{
//					FL.WorkTimeFinished += FL.TheWorkArea[i].Stations[j]->TimeLeft;
//				}
//
//				//Not finished but is in the build line, check if any spot in build line is free (starting at the end of build), also can't be at the end of the build line
//				if (FL.TheWorkArea[i].AreaName == "Build" && j < FL.TheWorkArea[i].Stations.size() - 1)
//				{
//					for (int k = FL.TheWorkArea[i].Stations.size() - 1; k > j; k--)
//					{
//						if (FL.TheWorkArea[i].Stations[k] == nullptr)
//						{
//							FL.TheWorkArea[i].Stations[k] = FL.TheWorkArea[i].Stations[j];
//							FL.TheWorkArea[i].Stations[j] = nullptr;
//							k = j; //break out of loop
//						}
//					}
//				}
//
//				//Moving to next spot in line, as long as we're not at the start
//				MoveInFlowline(FL, i, j, FL.WorkDay);
//
//			}
//		}
//
//	} while (i > 10); //negative one to allow loop to run when i = 0 and j = 0 which is the first unit in the Flowline
//
//
//	//FL.WorkDay -= FL.WorkTime;
//
//	return FL;
//}
