#include "Problem.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
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


int Problem::getNbSensors(){
    return nbSensors;
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


vector<pair<int, int> > Problem::getNotCoveredPositions(){
    vector<pair<int, int> > notCoveredPositions;
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
                notCoveredPositions.push_back(make_pair(r,c));
        }
    }
    return notCoveredPositions;
}


pair<vector<pair<int, int> >, vector<vector<bool> > > Problem::getNotConnectedSensors(){
    vector<pair<int, int> > notConnectedSensors;
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
                notConnectedSensors.push_back(make_pair(r,c));
        }
    }
    return make_pair(notConnectedSensors, visited);
}



void Problem::randomFeasibleSolution(){
    // empty the grid
    for (int r=0; r<dimensions.first; r++)
        for (int c=0; c<dimensions.second; c++)
            removeSensor(r, c);

    vector<pair<int, int> > notCoveredPositions;
    // place sensors at random positions while not all positions in the grid are covered
    while(1){
        notCoveredPositions = getNotCoveredPositions();
        // all positions in the grid are covered: break
        if (notCoveredPositions.size()==0)
            break;

        // place a sensor at a random non covered position
        pair<int, int> position = notCoveredPositions[rand() % notCoveredPositions.size()];
        placeSensor(position.first, position.second);
    }

    pair<vector<pair<int, int> >, vector<vector<bool> > > NAME;
    vector<pair<int, int> > notConnectedSensors;
    vector<vector<bool> > connected;
    // make paths of sensors to connected every disconnected sensor to the well
    while(1){
        NAME = getNotConnectedSensors();
        notConnectedSensors = NAME.first;
        connected = NAME.second;
        // all sensors are connected to the well: break
        if (notConnectedSensors.size()==0)
            break;

        // create a path of sensors connecting a random disconnected sensor to the well
        pair<int, int> position = notConnectedSensors[rand() % notConnectedSensors.size()];
        vector<pair<int, int> > path = connectSensor(position.first, position.second, connected);
        for (vector<pair<int, int> >::iterator it = path.begin(); it < path.end(); it++){
            placeSensor((*it).first, (*it).second);
        }
    }

    // removing useless sensors
    for (int r=0; r<dimensions.first; r++){
        for (int c=0; c<dimensions.second; c++){
            if (grid[r][c]){
                removeSensor(r, c);
                if (!isGridCovered() || !areSensorsConnected())
                    placeSensor(r, c);
            }
        }
    }
}


vector<pair<int, int> > Problem::connectSensor(int r, int c, const vector<vector<bool> > & connected){
    vector<vector<bool> > visited (dimensions.first, vector<bool> (dimensions.second, false));
    // dist[rr][cc] = minimum nb of steps to access (rr,cc) from (r,c) (DFS)
    vector<vector<int> > dist (dimensions.first, vector<int> (dimensions.second, -1));
    vector<vector<pair<int, int > > > predecessors (dimensions.first, vector<pair<int, int> > (dimensions.second, make_pair(-1, -1)));
    dist[r][c] = 0;

    // create a sensor path between (r,c) and closestConnectedSensor to connect (r,c) to the well at position (0,0)
    pair<int, int> closestConnectedSensor = make_pair(-1,-1);
    // DFS gives minimum distances
    queue<pair<int, int> > Q;
    Q.push(make_pair(r,c));
    while(!Q.empty()){
        int rr = Q.front().first;
        int cc = Q.front().second;
        Q.pop();
        if (!visited[rr][cc]){
            visited[rr][cc] = true;
            for (int i = max(0, rr-Rcom); i < min(dimensions.first, rr+Rcom+1); i++){
                for (int j = max(0, cc-Rcom); j < min(dimensions.second, cc+Rcom+1); j++){
                    if (((rr-i)*(rr-i) + (cc-j)*(cc-j) <= Rcom*Rcom) && dist[i][j] == -1){
                        // position (i,j) not visited yet, distance dist(rr, cc)+1, predecessor (rr,cc)
                        dist[i][j] = dist[rr][cc] + 1;
                        predecessors[i][j] = make_pair(rr, cc);
                        Q.push(make_pair(i,j));
                        // connected sensor at position (i,j): (i,j) is the closest connected sensor
                        if (connected[i][j]){
                            closestConnectedSensor = make_pair(i,j);
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
    while(dist[path.back().first][path.back().second] != 0){
        path.push_back(predecessors[path.back().first][path.back().second]);
    }
    return path;
}


void Problem::printGrid(){
    cout << "Rcapt = " << Rcapt << ", Rcom = " << Rcom << endl << endl;
    // "O" at position (0,0) represents the well
    cout << " O";
    for (int c=1; c<dimensions.second; c++){
        // "x" at position (r,c) if there is a sensor at position (r,c)
        if (grid[0][c])
            cout << " x";
        // "." at position (r,c) if there is no sensor at position (r,c)
        else
            cout << " .";
    }
    cout << endl;
    for (int r=1; r<dimensions.first; r++){
        for (int c=0; c<dimensions.second; c++){
            if (grid[r][c])
                cout << " x";
            else
                cout << " .";
        }
        cout << endl;
    }
}
