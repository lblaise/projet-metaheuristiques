// projet-metaheuristiques.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <time.h>
#include <queue>
#include <list>


using namespace std;

void test();
void solvePb();
bool contains(pair<int, int> position, const list<pair<int, int> > & positionsList);



int main()
{
	time_t t0, t1;
	time(&t0);

	solvePb();

	time(&t1);
	cout << "\n\nTIME: " << t1 - t0 << " seconds" << endl;

	system("PAUSE");
    return 0;
}


void solvePb() {
	// to be changed
	bool diversificationCondition = false;
	bool intensificationCondition1 = false;
	bool intensificationCondition2 = false;


	srand(time(NULL));

	Problem * currentGrid = new Problem(NB_ROWS, NB_COLS, R_CAPT, R_COM);
	currentGrid->randomFeasibleSolution();
	Problem * neighbourGrid = new Problem(*currentGrid);
	float bestValue = currentGrid->getObjectiveValue();
	Problem * bestGrid = new Problem(*currentGrid);

	list<pair<int, int> > taboo(LEN_TABOO, make_pair(-1, -1));
	vector<vector<int> > frequentSensors(NB_ROWS, vector<int>(NB_COLS, 0));
	vector<pair<int, int> > goodCoordinates;

	
	bestGrid->printGrid();
	cout << "Value of the first random grid: " << bestGrid->getObjectiveValue() << endl << endl;

	for (int counter = 0; counter < NB_ITER; counter++) {
		pair<int, int> bestNeighbour = make_pair(0, 0);
		float bestNeighbourValue = currentGrid->getDimensions().first * currentGrid->getDimensions().second;

		// diversification: start again from a new random solution
		if (diversificationCondition) {
			currentGrid->randomFeasibleSolution();
			// the current grid is the best grid so far
			if (currentGrid->getObjectiveValue() < bestValue) {
				bestValue = currentGrid->getObjectiveValue();
				bestGrid->setProblem(*currentGrid);
			}
		}

		// intensification: start again from the best solution, without taboo
		else if (intensificationCondition1) {
			currentGrid->setProblem(*bestGrid);
			for (int i = 0; i < LEN_TABOO; i++) {
				taboo.pop_front();
				taboo.push_back(make_pair(-1, -1));
			}
		}

		// intensification: start again from a random solution with sensors at sensors at positions where there often is one
		else if (intensificationCondition2) {
			currentGrid->randomFeasibleSolution(goodCoordinates);
			// the current grid is the best grid so far
			if (currentGrid->getObjectiveValue() < bestValue) {
				bestValue = currentGrid->getObjectiveValue();
				bestGrid->setProblem(*currentGrid);
			}
		}

		// neighbours of the current grid: Hamming distance = 1
		for (int i = 1; i < currentGrid->getDimensions().first * currentGrid->getDimensions().second; i++) {
			int r = i / currentGrid->getDimensions().first;
			int c = i % currentGrid->getDimensions().first;
			
			// taboo: don't search this neighbour
			if (contains(make_pair(r, c), taboo))
				continue;

			// neighbour: add or remove sensor at position (r,c)
			neighbourGrid->neighbour(r, c);

			// current neighbour is the best neighbour so far
			if (neighbourGrid->getObjectiveValue() < bestNeighbourValue) {
				bestNeighbourValue = neighbourGrid->getObjectiveValue();
				bestNeighbour = make_pair(r, c);
				// current neighbour is better than the current grid: break and use the current neigbour as the next grid
				if (bestNeighbourValue < currentGrid->getObjectiveValue()) {
					// go back to the current grid (and then to the current neighbour after exiting the neigbourhood loop
					neighbourGrid->neighbour(r, c);
					break;
				}
			}
			// go back to the current grid in order to compute the next neighbour
			neighbourGrid->neighbour(r, c);
		}
		// the current grid is replaced by its best neighbour or by its first strictly better neighbour
		currentGrid->neighbour(bestNeighbour.first, bestNeighbour.second);
		neighbourGrid->neighbour(bestNeighbour.first, bestNeighbour.second);
		
		// the current grid is the best grid so far
		if (currentGrid->getObjectiveValue() < bestValue) {
			bestValue = currentGrid->getObjectiveValue();
			bestGrid->setProblem(*currentGrid);
		}

		// update taboo
		taboo.pop_front();
		taboo.push_back(make_pair(bestNeighbour.first, bestNeighbour.second));

		// update often used coordinates
		frequentSensors[bestNeighbour.first][bestNeighbour.second] ++;
		if (frequentSensors[bestNeighbour.first][bestNeighbour.second] > FREQUENT) {
			goodCoordinates.push_back(make_pair(bestNeighbour.first, bestNeighbour.second));
		}

	}

	cout << "After " << NB_ITER << " iterations:" << endl;
	bestGrid->printGrid();
	cout << "Best value: " << bestGrid->getObjectiveValue() << endl;
	cout << "Nb of sensors int the best grid: " << bestGrid->getNbSensors() << endl;
}



void test() {
	Problem *p = new Problem(10, 10, 1, 1);
	p->placeSensor(1, 0);
	p->placeSensor(1, 1);
	p->placeSensor(1, 2);
	p->placeSensor(1, 3);
	p->placeSensor(1, 4);
	p->placeSensor(1, 5);
	p->placeSensor(2, 5);
	p->placeSensor(3, 5);
	p->placeSensor(4, 5);
	p->placeSensor(5, 5);
	p->printGrid();

	cout << "cc: " << p->getNbConnectedComponents() << endl;


	srand(time(NULL));
	cout << "\n\nRandom solutions:" << endl;
	for (int i = 0; i<5; i++) {
		p->randomFeasibleSolution();
		p->printGrid();
		cout << "nb connected components: " << p->getNbConnectedComponents() << endl << endl;
	}


	cout << "Lower bound: " << p->lowerBound() << endl << endl;

	p->setFromFile("testGrid.txt");
	cout << "test grid:" << endl;
	p->printGrid();
	cout << p->getNbConnectedComponents() << " connected components" << endl << endl;
	cout << "placing central sensor (position (3, 4)):" << endl;
	p->placeSensor(3, 4);
	p->printGrid();
	cout << p->getNbConnectedComponents() << " connected components" << endl;
}


bool contains(pair<int, int> position, const list<pair<int, int> > & positionsList) {
	for (auto it = positionsList.begin(); it != positionsList.end(); it++) {
		if ((*it) == position)
			return true;
	}
	return false;
}