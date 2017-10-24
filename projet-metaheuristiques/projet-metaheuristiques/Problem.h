#pragma once

using namespace std;


class Problem
{
private:
	vector<vector<bool> > grid;
	int Rcapt;
	int Rcom;
	pair<int, int> dimensions;
	int nbSensors;

	vector<pair<int, int> > connectSensor(int r, int c, const vector<vector<bool> > & connected);


public:
	Problem(int nrows, int ncols, int rcapt, int rcom);
	vector<vector<bool> > getGrid();
	pair<int, int> getDimensions();
	int getRcapt();
	int getRcom();
	int getNbSensors();

	void placeSensor(int r, int c);
	void removeSensor(int r, int c);
	bool isGridCovered();
	bool areSensorsConnected();

	vector<pair<int, int> > getNotCoveredPositions();
	pair<vector<pair<int, int> >, vector<vector<bool> > > getNotConnectedSensors();

	void randomFeasibleSolution();

	void printGrid();
	int lowerBound();
};