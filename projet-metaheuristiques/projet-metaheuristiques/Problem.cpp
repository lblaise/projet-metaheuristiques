#include "stdafx.h"
#include <queue>

using namespace std;


Problem::Problem(int nrows, int ncols, int rcapt, int rcom)
{
	dimensions = make_pair(nrows, ncols);
	grid = vector<vector<bool> >(nrows, vector<bool>(ncols, false));
	cover = vector<vector<int> >(nrows, vector<int>(ncols, 0));
	Rcapt = rcapt;
	Rcom = rcom;
	nbSensors = 0;
	nbNotCoveredPositions = nrows*ncols;
}


Problem::Problem(const Problem & p) {
	setProblem(p);
}


void Problem::setProblem(const Problem & p) {
	dimensions = p.getDimensions();
	grid = p.getGrid();
	cover = p.getCover();
	Rcapt = p.getRcapt();
	Rcom = p.getRcom();
	nbSensors = p.getNbSensors();
	nbNotCoveredPositions = p.getNbNotCoveredPositions();
}


vector<vector<bool> > Problem::getGrid() const {
	return grid;
}


vector<vector<int> > Problem::getCover() const {
	return cover;
}


pair<int, int> Problem::getDimensions() const {
	return dimensions;
}


int Problem::getRcapt() const {
	return Rcapt;
}


int Problem::getRcom() const {
	return Rcom;
}


int Problem::getNbSensors() const {
	return nbSensors;
}


int Problem::getNbNotCoveredPositions() const {
	return nbNotCoveredPositions;
}



void Problem::placeSensor(int r, int c) {
	if (!grid[r][c]) {
		grid[r][c] = true;
		nbSensors++;
		// update covered positions
		for (int i = max(0, r - Rcapt); i < min(dimensions.first, r + Rcapt + 1); i++) {
			for (int j = max(0, c - Rcapt); j < min(dimensions.second, c + Rcapt + 1); j++) {
				if ((r - i)*(r - i) + (c - j)*(c - j) <= Rcapt*Rcapt) {
					if (!cover[i][j])
						nbNotCoveredPositions--;
					cover[i][j]++;
				}
			}
		}
		// update number of connected components?
	}
}


void Problem::removeSensor(int r, int c) {
	if (grid[r][c]) {
		grid[r][c] = false;
		nbSensors--;
		// update covered positions
		for (int i = max(0, r - Rcapt); i < min(dimensions.first, r + Rcapt + 1); i++) {
			for (int j = max(0, c - Rcapt); j < min(dimensions.second, c + Rcapt + 1); j++) {
				if ((r - i)*(r - i) + (c - j)*(c - j) <= Rcapt*Rcapt) {
					cover[i][j]--;
					if (!cover[i][j])
						nbNotCoveredPositions++;
				}
			}
		}
		// update number of connected components?
	}
}


void Problem::neighbour(int r, int c) {
	if (grid[r][c])
		removeSensor(r, c);
	else
		placeSensor(r, c);
}




bool Problem::areSensorsConnected() {
	vector<vector<bool> > visited(dimensions.first, vector<bool>(dimensions.second, false));
	queue<pair<int, int> > Q;
	// BFS
	Q.push(make_pair(0, 0));
	while (!Q.empty()) {
		int r = Q.front().first;
		int c = Q.front().second;
		Q.pop();
		if (!visited[r][c]) {
			visited[r][c] = true;
			for (int i = max(0, r - Rcom); i < min(dimensions.first, r + Rcom + 1); i++) {
				for (int j = max(0, c - Rcom); j < min(dimensions.second, c + Rcom + 1); j++) {
					// (r,c) can communicate with (i,j) and there is a sensor at position (i,j)
					if (((r - i)*(r - i) + (c - j)*(c - j) <= Rcom*Rcom) && grid[i][j])
						Q.push(make_pair(i, j));
				}
			}
		}
	}

	for (int r = 0; r<dimensions.first; r++) {
		for (int c = 0; c<dimensions.second; c++) {
			// sensor at position (r,c) is not connected to the well
			if (grid[r][c] && !visited[r][c])
				return false;
		}
	}
	return true;
}


vector<pair<int, int> > Problem::getNotCoveredPositions() {
	vector<pair<int, int> > notCoveredPositions;
	for (int r = 0; r<dimensions.first; r++) {
		for (int c = 0; c<dimensions.second; c++) {
			if (!cover[r][c])
				notCoveredPositions.push_back(make_pair(r, c));
		}
	}
	return notCoveredPositions;
}


vector<vector<bool> > Problem::getConnectedSensors() {
	vector<vector<bool> > visited(dimensions.first, vector<bool>(dimensions.second, false));
	queue<pair<int, int> > Q;
	// BFS
	Q.push(make_pair(0, 0));
	while (!Q.empty()) {
		int r = Q.front().first;
		int c = Q.front().second;
		Q.pop();
		if (!visited[r][c]) {
			visited[r][c] = true;
			for (int i = max(0, r - Rcom); i < min(dimensions.first, r + Rcom + 1); i++) {
				for (int j = max(0, c - Rcom); j < min(dimensions.second, c + Rcom + 1); j++) {
					// (r,c) can communicate with (i,j) and there is a sensor at position (i,j)
					if (((r - i)*(r - i) + (c - j)*(c - j) <= Rcom*Rcom) && grid[i][j])
						Q.push(make_pair(i, j));
				}
			}
		}
	}
	return visited;
}



void Problem::randomFeasibleSolution() {
	// empty the grid
	for (int r = 0; r < dimensions.first; r++)
		for (int c = 0; c < dimensions.second; c++)
			removeSensor(r, c);

	vector<pair<int, int> > notCoveredPositions;
	// place sensors at random positions while not all positions in the grid are covered
	while (1) {
		notCoveredPositions = getNotCoveredPositions();
		// all positions in the grid are covered: break
		if (notCoveredPositions.size() == 0)
			break;

		// place a sensor at a random non covered position
		pair<int, int> position = notCoveredPositions[rand() % notCoveredPositions.size()];
		placeSensor(position.first, position.second);

		// connect the sensor to the well: add a sensor path connecting the sensor to the closest connected sensor
		vector<pair<int, int> > path = connectSensor(position.first, position.second, getConnectedSensors());
		for (vector<pair<int, int> >::iterator it = path.begin(); it < path.end(); it++) {
			placeSensor((*it).first, (*it).second);
		}
	}

	// removing useless sensors
	for (int r = 0; r<dimensions.first; r++) {
		for (int c = 0; c<dimensions.second; c++) {
			if (grid[r][c]) {
				removeSensor(r, c);
				if (nbNotCoveredPositions > 0 || !areSensorsConnected())
					placeSensor(r, c);
			}
		}
	}
}


vector<pair<int, int> > Problem::connectSensor(int r, int c, const vector<vector<bool> > & connected) {
	vector<vector<bool> > visited(dimensions.first, vector<bool>(dimensions.second, false));
	vector<vector<pair<int, int > > > predecessors(dimensions.first, vector<pair<int, int> >(dimensions.second, make_pair(-1, -1)));
	
	// create a sensor path between (r,c) and closestConnectedSensor to connect (r,c) to the well at position (0,0)
	pair<int, int> closestConnectedSensor = make_pair(-1, -1);
	// DFS gives minimum distances on the grid, hence closest connected sensor
	queue<pair<int, int> > Q;
	Q.push(make_pair(r, c));
	while (!Q.empty()) {
		int rr = Q.front().first;
		int cc = Q.front().second;
		Q.pop();
		if (!visited[rr][cc]) {
			visited[rr][cc] = true;
			for (int i = max(0, rr - Rcom); i < min(dimensions.first, rr + Rcom + 1); i++) {
				for (int j = max(0, cc - Rcom); j < min(dimensions.second, cc + Rcom + 1); j++) {
					if (((rr - i)*(rr - i) + (cc - j)*(cc - j) <= Rcom*Rcom) && predecessors[i][j].first == -1) {
						// position (i,j) not visited yet, predecessor (rr,cc) (and distance dist(rr, cc)+1)
						predecessors[i][j] = make_pair(rr, cc);
						Q.push(make_pair(i, j));
						// connected sensor at position (i,j): (i,j) is the closest connected sensor
						if (connected[i][j]) {
							closestConnectedSensor = make_pair(i, j);
							goto writePath;
						}
					}
				}
			}
		}
	}

	writePath:
	vector<pair<int, int> > path;
	// no connected sensor and no well: empty path
	if (closestConnectedSensor.first == -1 && closestConnectedSensor.second == -1)
		return path;

	// minimum path between (r,c) and closestConnectedSensor
	path.push_back(closestConnectedSensor);
	while (predecessors[path.back().first][path.back().second] != make_pair(r, c)) {
		path.push_back(predecessors[path.back().first][path.back().second]);
	}
	return path;
}


void Problem::printGrid() {
	// "O" at position (0,0) represents the well
	cout << " O";
	for (int c = 1; c<dimensions.second; c++) {
		// "x" at position (r,c) if there is a sensor at position (r,c)
		if (grid[0][c])
			cout << " x";
		// "." at position (r,c) if there is no sensor at position (r,c)
		else
			cout << " .";
	}
	cout << endl;
	for (int r = 1; r<dimensions.first; r++) {
		for (int c = 0; c<dimensions.second; c++) {
			if (grid[r][c])
				cout << " x";
			else
				cout << " .";
		}
		cout << endl;
	}
}


int Problem::lowerBound() {
	// the first sensor can cover n1 positions on the grid
	int n1 = 0;
	for (int i = -Rcapt; i <= Rcapt; i++) {
		for (int j = -Rcapt; j <= Rcapt; j++) {
			if (i*i + j*j <= Rcapt*Rcapt)
				n1++;
		}
	}
	// another sensor at position (x,y), connected to a previously added sensor at position (x+i0,y+j0),
	// can cover at most n2 positions on the grid
	int n2 = 0;
	for (int i0 = 0; i0 <= Rcom; i0++) {
		// sensor at position (x,y) can cover n2Temp new positions on the grid if there is a sensor at position (i0,j0)
		int n2Temp = 0;
		int j0 = sqrt(Rcom*Rcom - i0*i0);
		if (i0*i0 + j0*j0 == Rcom*Rcom){
			for (int i = -Rcapt; i <= Rcapt; i++) {
				for (int j = -Rcapt; j <= Rcapt; j++) {
					// position (x+i,y+j) is covered by the new sensor at position (x,y) 
					//but not by the already connected sensor at position (x+i0,y+j0)
					if ((i*i + j*j <= Rcapt*Rcapt) && ((i0 - i)*(i0 - i) + (j0 - j)*(j0 - j) > Rcapt*Rcapt)) {
						n2Temp++;
					}
				}
			}
		}
		n2 = max(n2, n2Temp);
	}
	//cout << "n1 = " << n1 << ", n2 = " << n2 << endl;
	return ceil(1 + (dimensions.first*dimensions.second - n1) / float(n2));
}



float Problem::getObjectiveValue() {
	return nbSensors + ALPHA * nbNotCoveredPositions + BETA * (getNbCCTest() - 1);
}


int Problem::getNbCCTest() {
	int nbCC = 0;

	vector<vector<bool> > visited(dimensions.first, vector<bool>(dimensions.second, false));

	for (int r = 0; r < dimensions.first; r++) {
		for (int c = 0; c < dimensions.second; c++) {
			if (grid[r][c] && !visited[r][c]) {
				nbCC++;
				// BFS
				queue<pair<int, int> > Q;
				Q.push(make_pair(r, c));
				while (!Q.empty()) {
					int rr = Q.front().first;
					int cc = Q.front().second;
					Q.pop();
					if (!visited[rr][cc]) {
						visited[rr][cc] = true;
						for (int i = max(0, rr - Rcom); i < min(dimensions.first, rr + Rcom + 1); i++) {
							for (int j = max(0, cc - Rcom); j < min(dimensions.second, cc + Rcom + 1); j++) {
								if (((rr - i)*(rr - i) + (cc - j)*(cc - j) <= Rcom*Rcom) && grid[i][j]) {
									Q.push(make_pair(i, j));
								}
							}
						}
					}
				}
			}
		}
	}
	return nbCC;
}