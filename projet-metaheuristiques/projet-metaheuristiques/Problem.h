#pragma once

using namespace std;


class Problem
{
private:
	vector<vector<bool> > grid;
	vector<vector<int> > cover; // nb of times each position in the grid is covered
	int Rcapt;
	int Rcom;
	pair<int, int> dimensions;
	int nbSensors;
	int nbNotCoveredPositions;

	vector<pair<int, int> > connectSensor(int r, int c, const vector<vector<bool> > & connected);


public:
	Problem(int nrows, int ncols, int rcapt, int rcom);
	vector<vector<bool> > getGrid();
	vector<vector<int> > getCover();
	pair<int, int> getDimensions();
	int getRcapt();
	int getRcom();
	int getNbSensors();

	int	getNbNotCoveredPositions();

	void placeSensor(int r, int c);
	void removeSensor(int r, int c);
	void neighbour(int r, int c);
	bool isGridCovered();
	bool areSensorsConnected();

	vector<pair<int, int> > getNotCoveredPositions();
	pair<vector<pair<int, int> >, vector<vector<bool> > > getNotConnectedSensors();

	void randomFeasibleSolution();

	void printGrid();
	int lowerBound();
};