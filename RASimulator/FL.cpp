#include "WorkArea.h"


WorkArea::WorkArea()
{

}

WorkArea::WorkArea(string name, vector<Unit*> stations)
{
	AreaName = name;
	Stations = stations;

}

void WorkArea::AddToOverflow(list<Unit> &Overflow, Unit MOT)
{
	Overflow.push_back(MOT);

	return;
}

void WorkArea::AddToRework(vector<Unit> &Rework, Unit MOT)
{
	Rework.push_back(MOT);


	return;
}

void CreateFlowLine(vector<WorkArea> &FL)
{
	FL.push_back(*(new FB));
	FL.push_back(*(new UA));
	FL.push_back(*(new INIT));
	FL.push_back(*(new AVS));
	FL.push_back(*(new ULTest));
	FL.push_back(*(new Receiving));
	FL.push_back(*(new Prep));
	FL.push_back(*(new Build));
	FL.push_back(*(new CLTest));
	FL.push_back(*(new Settings));
	FL.push_back(*(new Doors));
	FL.push_back(*(new Inspection));
	FL.push_back(*(new Packaging));
	//pointer -> FL.push_back(new Packaging);
	return;
}
//ostream& operator<<(ostream& os, const list<Unit>& v)
//{
//	for (list<Unit>::iterator it = v.begin(); it != v.end(); ++it)
//	{
//		v._Getpfirst;
//		v.
//	}
//
//
//	os << endl;
//	return os;
//}

ostream& operator<<(ostream& os, const vector<Unit*>& v)
{
	for (int i = 0; i < v.size(); ++i)
	{
		os << (v[i])->UnitName << ",";
	}
	os << endl;
	return os;
}