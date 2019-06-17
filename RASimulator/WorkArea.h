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
	vector<string> Rework;
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
	double WorkDay = 700;
	double WorkTime = 1;
	int CompletedUnitCounter = 0;
	double WorkTimeFinished = 0;
	vector<WorkArea> TheWorkArea;
	vector<pair <string, Unit>> UnitList;

};
//what we're going to do is simulate 10 minutes at a time

void CreateFlowLine(vector<WorkArea> &FL);
void RemoveFirstOverFlowUnit(FlowLine &FL, int i);
//Unit* ReturnLastOverFlowUnit(FlowLine &FL, int i);
bool OverFlowIsEmpty(FlowLine &FL, int i);
void MoveToOverFlow(FlowLine &FL, int i, int j);
FlowLine SimulateFlowLine(FlowLine &FL, ifstream & ReadUnitFile);
void SimulateFlowHelper(FlowLine &FL, ifstream & ReadUnitFile);
void PrintFlowLine(FlowLine FL);
void AddUnitToFlow(FlowLine &FL, ifstream & ReadUnitFile, int i, int j);