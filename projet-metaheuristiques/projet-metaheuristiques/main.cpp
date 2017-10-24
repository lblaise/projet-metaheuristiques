// projet-metaheuristiques.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <time.h>


int main()
{
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


	srand(time(NULL));
	cout << "\n\nRandom solutions:" << endl;
	for (int i = 0; i<7; i++) {
		p->randomFeasibleSolution();
		p->printGrid();
		cout << endl << endl;
	}

	cout << "Lower bound: " << p->lowerBound() << endl;
	

	system("PAUSE");
    return 0;
}

