#include "Problem.h"
#include <iostream>
#include <queue>

using namespace std;


Problem::Problem(int nrows, int ncols, int rcapt, int rcom)
{
    dimensions = make_pair(nrows, ncols);
    grid = vector<vector<bool> > (nrows, vector<bool> (ncols, false));
    Rcapt = rcapt;
    Rcom = rcom;
    nbSensors = 0;
}


vector<vector<bool> > Problem::getGrid(){
    return grid;
}


pair<int, int> Problem::getDimensions(){
    return dimensions;
}


int Problem::getRcapt(){
    return Rcapt;
}


int Problem::getRcom(){
    return Rcom;
}



void Problem::placeSensor(int r, int c){
    if (!grid[r][c]){
        grid[r][c] = true;
        nbSensors++;
    }
}


void Problem::removeSensor(int r, int c){
    if (grid[r][c]){
        grid[r][c] = false;
        nbSensors--;
    }
}


bool Problem::isGridCovered(){
    for (int r=0; r<dimensions.first; r++){
        for (int c=0; c<dimensions.second; c++){
            bool covered = grid[r][c];
            // is position (r,c) covered by a sensor at position (i,j) such that sqrt((r-i)^2 + (c-j)^2) <= Rcapt?
            for (int i = max(0, r-Rcapt); i < min(dimensions.first, r+Rcapt+1); i++){
                for (int j = max(0, c-Rcapt); j < min(dimensions.second, c+Rcapt+1); j++){
                    if ((r-i)*(r-i) + (c-j)*(c-j) <= Rcapt*Rcapt){
                        covered = covered || grid[i][j];
                        if (covered)
                            goto next;
                    }
                }
            }
            next:
            if (!covered && !(r==0 && c==0))
                return false;
        }
    }
    return true;
}


bool Problem::areSensorsConnected(){
    vector<vector<bool> > visited (dimensions.first, vector<bool> (dimensions.second, false));
    queue<pair<int, int> > Q;
    // BFS
    Q.push(make_pair(0, 0));
    while (!Q.empty()){
        int r = Q.front().first;
        int c = Q.front().second;
        Q.pop();
        if (!visited[r][c]){
            visited[r][c] = true;
            for (int i = max(0, r-Rcom); i < min(dimensions.first, r+Rcom+1); i++){
                for (int j = max(0, c-Rcom); j < min(dimensions.second, c+Rcom+1); j++){
                    // (r,c) can communicate with (i,j) and there is a sensor at position (i,j)
                    if (((r-i)*(r-i) + (c-j)*(c-j) <= Rcom*Rcom) && grid[i][j])
                        Q.push(make_pair(i, j));
                }
            }
        }
    }

    for (int r=0; r<dimensions.first; r++){
        for (int c=0; c<dimensions.second; c++){
            // sensor at position (r,c) is not connected to the well
            if (grid[r][c] && !visited[r][c])
                return false;
        }
    }
    return true;
}
