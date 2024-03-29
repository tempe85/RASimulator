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
	int MaxOverFlowSize = 0;

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
	FlowLine() {};
	~FlowLine() {};
	double WorkDay = 2400;
	double WorkDayCounter = 0;
	double WorkTime = 1;
	int UnitMode;
	int AssignUnitID = 1;
	int CompletedUnitCounter = 0;
	int unitGeneratorCounter = 0;
	int userInputGenerator = 0;
	double TimeValueAdded = 0;
	double TimeNoValueAdded = 0;
	double TimeUnitRework = 0;
	vector<WorkArea> TheWorkArea;
	vector<pair <string, Unit>> UnitList;
	//vector<Unit*> ReWork;
	list<Unit*> ReWork;
	vector<Unit*> CompletedUnits;
	int BuildStation6 = 10;
	bool CheckAreaDown = false;

	unordered_map<string, int> ReworkTimeByArea = {
			{ "FB", 0},
			{ "UA", 0 },
			{ "INIT", 0 },
			{ "AVS", 0 },
			{ "ULT", 0 },
			{ "Rec", 0},
			{ "Prep", 0 },
			{ "Build", 0 },
			{ "CLT", 0 },
			{"Settings", 0},
			{ "Doors", 0 },
			{ "Ins", 0 },
			{ "Pkg", 0 },
	};

	unordered_map<string, int> OrderOfWorkAreas = {
			{ "FB", 0},
			{ "UA", 0 },
			{ "INIT", 0 },
			{ "AVS", 0 },
			{ "ULT", 0 },
			{ "Rec", 0},
			{ "Prep", 0 },
			{ "Build", 0 },
			{ "CLT", 0 },
			{"Settings", 0},
			{ "Doors", 0 },
			{ "Ins", 0 },
			{ "Pkg", 0 },
	};

};
//what we're going to do is simulate 10 minutes at a time

FlowLine FillFlowLine(FlowLine &FL, Unit TestUnit, ifstream & ReadUnitFile);
void CreateFlowLineAreaOrderMap(FlowLine & FL);
void CreateFlowLine(vector<WorkArea> &FL);
void RemoveFirstOverFlowUnit(FlowLine &FL, int i);
//Unit* ReturnLastOverFlowUnit(FlowLine &FL, int i);
bool OverFlowIsEmpty(FlowLine &FL, int i);
void MoveToOverFlow(FlowLine &FL, int i, int j);
FlowLine SimulateFlowLine(FlowLine &FL, ifstream & ReadUnitFile, int &i, int &j);
void SimulateFlowHelper(FlowLine &FL, ifstream & ReadUnitFile, int continueSim, int const ListSimulator);
void SimulateFlowHelperTester(FlowLine &FL, ifstream & ReadUnitFile);
void PrintFlowLine(FlowLine FL);
void PrintFailCheckData(FlowLine FL, int i, int j);
void AddUnitToFlow(FlowLine &FL, ifstream & ReadUnitFile, int i, int j);
void UnitFailCheck(WorkArea const curArea, Unit & curUnit);
void UnitFailCheckHelper(WorkArea const curArea, Unit & curUnit, FlowLine & FL);
void UnitFailCheckReset(Unit & curUnit);
void UnitTSHelper(FlowLine &FL);
void UnitReworkCheck(FlowLine & FL, WorkArea curArea, Unit & curUnit, double & unitBuildTime);
void PrintUnitFail(Unit FailedUnit, WorkArea curArea);
void SendUnitToTS(FlowLine &FL, const int i, const int j);
void MoveUnitToNewWorkArea(FlowLine &FL, const int i, const int j);
void MoveUnitToNextStation(FlowLine &FL, const int i, const int j);

Unit CheckUnitType(string line, FlowLine FL);

bool CheckIfFinished(double TimeLeft);
double CalculateTimeLeft(string AreaName, Unit MOT, FlowLine FL, double BuildTime);
void MoveInFlowline(FlowLine &FL, int &i, int &j, double & WorkdayTime);
bool OpenSpotInNextWorkArea(FlowLine & FL, const int & i, int &s);
void AdjustTimeLeft(FlowLine & FL, bool ValueAdded, Unit & curUnit);
int CheckSettings(FlowLine const FL, int &k, const int j);
void CountAreaDowntime(FlowLine &FL);
void CalculateUnitDownTime(FlowLine FL);
string GenerateUnitList(FlowLine & FL);
void ProgramInputsFromUser(FlowLine & FL, int &ListSimulator);
void UpdateCompletedAreas(Unit &curUnit, WorkArea const curWorkArea);
vector<string> CreateAreaOrderString(FlowLine & FL);


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
void SimDataOutputs(FlowLine FL, ofstream & SimData);

//template <typename T
ostream& operator<<(ostream& os, const vector<Unit*>& v);
