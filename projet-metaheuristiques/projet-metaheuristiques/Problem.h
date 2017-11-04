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
	int nbConnectedComponents;

	vector<pair<int, int> > connectSensor(int r, int c, const vector<vector<bool> > & connected);
	void checkConnectivityChange(int r, int c, bool removal);
	void checkConnectivity(const vector<pair<int, int> > & nearbySensorsList, vector<bool> & verifiedNearbySensors, bool & totalVerification, int & Dcc, int nbNearbySensors);
	bool browseConnectedComponent(const vector<pair<int, int> > & nearbySensorsList, vector<vector<bool> > & visited, int index, vector<bool> & verifiedNearbySensors, int nbNearbySensors);
	void getNearbySensors(int r, int c, vector<pair<int, int> > & nearbySensorsList, int & nbNearbySensors);
	bool isThereNearbySensor(int r, int c);



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
	int getNbConnectedComponents() const;

	void placeSensor(int r, int c);
	void removeSensor(int r, int c);
	void neighbour(int r, int c);
	bool areSensorsConnected();

	vector<pair<int, int> > getNotCoveredPositions();
	vector<vector<bool> > getConnectedSensors();

	void randomFeasibleSolution(const vector<pair<int, int> > & goodCoordinates = vector<pair<int, int> > ());

	void printGrid();
	int lowerBound();
	float getObjectiveValue();
	int getNbCCTest();
	void setFromFile(string fileName);
};