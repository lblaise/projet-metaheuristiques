#ifndef PROBLEM_H
#define PROBLEM_H

#include<vector>
#include<math.h>
using namespace std;



class Problem
{
    private:
        vector<vector<bool> > grid;
        int Rcapt;
        int Rcom;
        pair<int, int> dimensions;
        int nbSensors;


    public:
        Problem(int nrows, int ncols, int rcapt, int rcom);
        vector<vector<bool> > getGrid();
        pair<int, int> getDimensions();
        int getRcapt();
        int getRcom();

        void placeSensor(int r, int c);
        void removeSensor(int r, int c);
        bool isGridCovered();
        bool areSensorsConnected();

};

#endif // PROBLEM_H
