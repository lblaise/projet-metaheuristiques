// projet-metaheuristiques.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <time.h>
#include <list>
#include <algorithm>

using namespace std;

void test();
int solvePb(int size, int Rcapt, int Rcom);
bool contains(pair<int, int> position, const list<pair<int, int> > & positionsList);
void lowerBounds();
void solveAll();



int main()
{
	time_t t0, t1;
	time(&t0);

	solveAll();

	time(&t1);
	cout << "\n\nTIME: " << t1 - t0 << " seconds" << endl;

	system("PAUSE");
    return 0;
}


int solvePb(int size, int Rcapt, int Rcom) {
	// to be changed
	bool diversificationCondition = false;
	bool intensificationCondition1 = false;
	bool intensificationCondition2 = false;


	srand(time(NULL));

	Problem * currentGrid = new Problem(size, size, Rcapt, Rcom);
	currentGrid->randomFeasibleSolution();
	Problem * neighbourGrid = new Problem(*currentGrid);
	float bestValue = currentGrid->getObjectiveValue();
	Problem * bestGrid = new Problem(*currentGrid);

	list<pair<int, int> > taboo(size, make_pair(-1, -1));
	vector<vector<int> > frequentSensors(size, vector<int>(size, 0));
	vector<pair<int, int> > goodCoordinates;
	int improvementCounter = 0; // number of steps since the last improvement of the best value

	
	/*bestGrid->printGrid();
	cout << "Value of the first random grid: " << bestGrid->getObjectiveValue() << endl << endl;*/


	vector<int> neighbourhood;
	for (int i = 1; i < currentGrid->getDimensions().first*currentGrid->getDimensions().second; i++) {
		neighbourhood.push_back(i);
	}


	for (int counter = 0; counter < NB_ITER; counter++) {
		pair<int, int> bestNeighbour = make_pair(0, 0);
		float bestNeighbourValue = currentGrid->getDimensions().first * currentGrid->getDimensions().second;

		// diversification: start again from a new random solution
		if (improvementCounter > MAX_ITER_WITHOUT_IMPROVEMENT) {
			currentGrid->randomFeasibleSolution();
			// the current grid is the best grid so far
			if (currentGrid->getObjectiveValue() < bestValue) {
				bestValue = currentGrid->getObjectiveValue();
				bestGrid->setProblem(*currentGrid);
				improvementCounter = 0;
			}
		}

		// intensification: start again from the best solution, without taboo
		else if (intensificationCondition1) {
			currentGrid->setProblem(*bestGrid);
			for (int i = 0; i < size; i++) {
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
				improvementCounter = 0;
			}
		}

		// neighbours of the current grid: Hamming distance = 1
		random_shuffle(neighbourhood.begin(), neighbourhood.end());
		for (int i = 0; i < currentGrid->getDimensions().first * currentGrid->getDimensions().second - 1; i++) {
			int r = neighbourhood[i] / currentGrid->getDimensions().first;
			int c = neighbourhood[i] % currentGrid->getDimensions().first;
			
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
			improvementCounter = 0;
		}
		else {
			improvementCounter++;
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

	/*cout << "After " << NB_ITER << " iterations:" << endl;
	bestGrid->printGrid();
	cout << "Best value: " << bestGrid->getObjectiveValue() << endl;
	cout << "Nb of sensors int the best grid: " << bestGrid->getNbSensors() << endl;*/
	return bestGrid->getObjectiveValue();
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


void lowerBounds() {
	vector<pair<int, int> > R;
	R.push_back(make_pair(1, 1));
	R.push_back(make_pair(1, 2));
	R.push_back(make_pair(2, 2));
	R.push_back(make_pair(2, 3));
	R.push_back(make_pair(3, 3));
	R.push_back(make_pair(3, 4));

	vector<int> sizes;
	sizes.push_back(10);
	sizes.push_back(15);
	sizes.push_back(20);
	sizes.push_back(25);
	sizes.push_back(30);
	sizes.push_back(40);
	sizes.push_back(50);

	Problem * p;

	for (auto itSize = sizes.begin(); itSize < sizes.end(); itSize++) {
		for (auto itR = R.begin(); itR < R.end(); itR++) {
			p = new Problem(*itSize, *itSize, (*itR).first, (*itR).second);
			cout << *itSize << "x" << *itSize << ", (" << (*itR).first << ", " << (*itR).second << "): " << p->lowerBound() << endl;
		}
	}
}



void solveAll() {
	vector<pair<int, int> > R;
	R.push_back(make_pair(1, 1));
	R.push_back(make_pair(1, 2));
	R.push_back(make_pair(2, 2));
	R.push_back(make_pair(2, 3));
	R.push_back(make_pair(3, 3));
	R.push_back(make_pair(3, 4));

	vector<int> sizes;
	sizes.push_back(10);
	sizes.push_back(15);
	sizes.push_back(20);
	sizes.push_back(25);
	sizes.push_back(30);
	sizes.push_back(40);
	sizes.push_back(50);

	Problem * p;
	int t0, t1, value;

	for (auto itSize = sizes.begin(); itSize < sizes.end(); itSize++) {
		for (auto itR = R.begin(); itR < R.end(); itR++) {
			t0 = time(NULL);
			p = new Problem(*itSize, *itSize, (*itR).first, (*itR).second);
			value = solvePb(*itSize, (*itR).first, (*itR).second);
			t1 = time(NULL);

			cout << "$" << *itSize << " \\times " << *itSize << ", \\ (" << (*itR).first << ", " << (*itR).second << ")$ & " << value << " & " << p->lowerBound() << " & " << t1 - t0 << " \\\\" << endl;
			cout << "\\hline" << endl;
		}
	}
}