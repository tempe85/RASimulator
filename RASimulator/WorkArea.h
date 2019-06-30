#include "units.h"



class WorkArea
{
public:
	WorkArea();
	WorkArea(string name, vector<Unit*> stations);
	void AddToOverflow(list<Unit> &Overflow, Unit MOT);
	void AddToRework(vector<Unit> &Rework, Unit MOT);
	

	void RemoveFromRework(vector<Unit> &Rework, Unit MOT)
	{
		Rework.pop_back();
		return;
	}
	string AreaName;
	vector<Unit*> Stations;
	vector<double> BuildT;
	int downtime = 0;
	//vector<Unit*> OverFlow;
	list<Unit*> OverFlow;
	//vector<string> Rework;
	int FailCoefficient = 0;
	int ReworkCoefficient = 0;
	int AreaDownCoefficient = 0;
	bool AreaDown = false;
	int AreaDownTimer = 0;

	bool ValueAdded;
	bool CheckIfAreaDown();
};
class FB : public WorkArea
{
public:
	FB();
};

class UA : public WorkArea
{
public:
	UA();

};

class INIT : public WorkArea
{
public: INIT();
};

class AVS : public WorkArea
{
public:
	AVS();
};
class ULTest : public WorkArea
{
public:
	ULTest();

};

class Receiving : public WorkArea
{
public:
	Receiving();

};
class Prep : public WorkArea
{
public:
	Prep();

};

class Build : public WorkArea
{
public:
	Build();

};

class CLTest : public WorkArea
{
public:
	CLTest();

};

class Settings : public WorkArea
{
public:
	Settings();
};

class Doors : public WorkArea
{
public:
	Doors();

};

class Inspection : public WorkArea
{
public:
	Inspection();
};

class Packaging : public WorkArea
{
public:
	Packaging();
};

class FlowLine
{
public:
	double WorkDay = 480;
	double WorkTime = 1;
	int CompletedUnitCounter = 0;
	double TimeValueAdded = 0;
	double TimeNoValueAdded = 0;
	vector<WorkArea> TheWorkArea;
	vector<pair <string, Unit>> UnitList;
	vector<Unit*> ReWork;
	vector<Unit*> CompletedUnits;
	int BuildStation6 = 10;
	bool CheckAreaDown = false;


};
//what we're going to do is simulate 10 minutes at a time


void CreateFlowLine(vector<WorkArea> &FL);
void RemoveFirstOverFlowUnit(FlowLine &FL, int i);
//Unit* ReturnLastOverFlowUnit(FlowLine &FL, int i);
bool OverFlowIsEmpty(FlowLine &FL, int i);
void MoveToOverFlow(FlowLine &FL, int i, int j);
FlowLine SimulateFlowLine(FlowLine &FL, ifstream & ReadUnitFile, int &i, int &j);
void SimulateFlowHelper(FlowLine &FL, ifstream & ReadUnitFile, int k);
void PrintFlowLine(FlowLine FL);
void AddUnitToFlow(FlowLine &FL, ifstream & ReadUnitFile, int i, int j);
bool UnitFailCheck(WorkArea curArea);
void UnitReworkCheck(WorkArea curArea, Unit curUnit, double & unitBuildTime);
void PrintUnitFail(Unit FailedUnit, WorkArea curArea);
void SendUnitToTS(FlowLine &FL, const int i, const int j, const bool IsOverFlow);

bool CheckIfFinished(double TimeLeft);
double CalculateTimeLeft(string AreaName, Unit MOT, FlowLine FL, double BuildTime);
void MoveInFlowline(FlowLine &FL, int &i, int &j, double & WorkdayTime);
bool OpenSpotInNextWorkArea(FlowLine & FL, const int & i, int &s);
void AdjustTimeLeft(FlowLine & FL, bool ValueAdded, Unit & curUnit);
int CheckSettings(FlowLine const FL, int &k, const int j);
void CountAreaDowntime(FlowLine &FL);
void CalculateUnitDownTime(FlowLine FL);

///WorkAreaSim Functions
FlowLine SimulateFlowLine2(FlowLine &FL, ifstream & ReadUnitFile);
void SimulateFormboard(FlowLine &FL, int &i, int &j, ifstream & ReadUnitFile);
void SimulateUA(FlowLine &FL, int &i, int &j);
void SimulateBasicArea(FlowLine &FL, int &i, int &j);
void SimulateBuild(FlowLine &FL, int &i, int &j);
void SimulatePkg(FlowLine & FL, int & i, int & j);
void SimulateULT(FlowLine &FL, int &i, int &j);
void SimulateAVS(FlowLine &FL, int &i, int &j);
void SimulateCLT(FlowLine &FL, int &i, int &j);
void OverFlowManager(FlowLine &FL, int &i, int &j);
bool AreaDownHelper(FlowLine &FL, int & i, int & j);


//Output file functions
void UnitTimeOutputs(FlowLine const FL, ofstream & UnitOutputs);

//template <typename T
ostream& operator<<(ostream& os, const vector<Unit*>& v);
