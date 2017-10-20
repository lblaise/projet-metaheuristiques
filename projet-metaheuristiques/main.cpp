#include <iostream>
#include <vector>
#include "Problem.h"

using namespace std;

int main()
{
    Problem *p = new Problem(10, 10, 1, 1);
    p->placeSensor(5, 5);

    cout << "grid covered: " << p->isGridCovered() << ", sensors connected to the well:" << p->areSensorsConnected() << endl;
    for (int r=0; r < p->getDimensions().first; r++){
        for (int c=0; c < p->getDimensions().second; c++){
            p->placeSensor(r, c);
        }
    }
    cout << "Placing sensors everywhere" << endl;
    cout << "grid covered: " << p->isGridCovered() << ", sensors connected to the well:" << p->areSensorsConnected() << endl;

    return 0;
}
