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
	Problem(const Problem & p);
	void setProblem(const Problem & p);
	vector<vector<bool> > getGrid() const;
	vector<vector<int> > getCover() const;
	pair<int, int> getDimensions() const;
	int getRcapt() const;
	int getRcom() const;
	int getNbSensors() const;
	int	getNbNotCoveredPositions() const;

	void placeSensor(int r, int c);
	void removeSensor(int r, int c);
	void neighbour(int r, int c);
	bool areSensorsConnected();

	vector<pair<int, int> > getNotCoveredPositions();
	vector<vector<bool> > getConnectedSensors();

	void randomFeasibleSolution();

	void printGrid();
	int lowerBound();
	float getObjectiveValue();
	int getNbCCTest();
};