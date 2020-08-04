// File:        Raymond Ao Da Chen z5077195
// Date:
// Description:
// Author:
// Modifications:

#include <webots/Robot.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

#define FILE "../../Map4_1.txt"
#define horxMax 9
#define horyMax 6
#define verxMax 10
#define veryMax 5

// All the webots classes are defined in the "webots" namespace
using namespace webots;
using namespace std;

int possiblePaths[10000][100] = {0};
char directions[10000][100];
int noPaths = 0;

void findPath(int locX, int locY, int floodfillnum, int maze[][9], int pathArray[], int horWalls[][9], int verWalls[][10], int stepCount, char directionArray[]);

void findPath(int locX, int locY, int floodfillnum, int maze[][9], int pathArray[], int horWalls[][9], int verWalls[][10], int stepCount, char directionArray[]){
    
    //North
    if (locY > 0){
      if ((horWalls[locY][locX] == 0) && (maze[locY-1][locX] == (floodfillnum-1))){
        pathArray[2*stepCount] = locY;
        pathArray[2*stepCount+1] = locX;
        directionArray[stepCount+3] = 'N';
        if (floodfillnum == 1){
          pathArray[2*(stepCount+1)] = locY-1;
          pathArray[2*(stepCount+1)+1] = locX;
          for (int i=0; i<2*(stepCount+1)+1; i++){
            possiblePaths[noPaths][i] = pathArray[i];
            possiblePaths[noPaths][i+1] = pathArray[i+1];
            i++;
          }
          
          for (int i = 0; i < stepCount+4; i++){
            directions[noPaths][i] = directionArray[i];
          }
          
          noPaths++;
        }
        findPath(locX, locY-1, floodfillnum-1, maze, pathArray, horWalls, verWalls, stepCount+1, directionArray);
      } 
    }
    
    //South
    if (locY < 4){
      if ((horWalls[locY+1][locX] == 0) && (maze[locY+1][locX] == (floodfillnum-1))){
        pathArray[2*stepCount] = locY;
        pathArray[2*stepCount+1] = locX;
        directionArray[stepCount+3] = 'S';
        if (floodfillnum == 1){
          pathArray[2*(stepCount+1)] = locY+1;
          pathArray[2*(stepCount+1)+1] = locX;
          for (int i=0; i<2*(stepCount+1)+1; i++){
            possiblePaths[noPaths][i] = pathArray[i];
            possiblePaths[noPaths][i+1] = pathArray[i+1];
            i++;
          }
          
          for (int i = 0; i < stepCount+4; i++){
            directions[noPaths][i] = directionArray[i];
          }
          
          noPaths++;
        }
        findPath(locX, locY+1, floodfillnum-1, maze, pathArray, horWalls, verWalls, stepCount+1, directionArray);
      }
    }
    
    //East
    if (locX < 8){
      if ((verWalls[locY][locX+1] == 0) && (maze[locY][locX+1] == (floodfillnum-1))){
        pathArray[2*stepCount] = locY;
        pathArray[2*stepCount+1] = locX;
        directionArray[stepCount+3] = 'E';
        if (floodfillnum == 1){
          pathArray[2*(stepCount+1)] = locY;
          pathArray[2*(stepCount+1)+1] = locX+1;
          for (int i=0; i<2*(stepCount+1)+1; i++){
            possiblePaths[noPaths][i] = pathArray[i];
            possiblePaths[noPaths][i+1] = pathArray[i+1];
            i++;
          }
          
          for (int i = 0; i < stepCount+4; i++){
            directions[noPaths][i] = directionArray[i];
          }
          
          noPaths++;
        }
        findPath(locX+1, locY, floodfillnum-1, maze, pathArray, horWalls, verWalls, stepCount+1, directionArray);
      }
    }
    
    //West
    if (locX > 0){
      if ((verWalls[locY][locX] == 0) && (maze[locY][locX-1] == (floodfillnum-1))){
        pathArray[2*stepCount] = locY;
        pathArray[2*stepCount+1] = locX;
        directionArray[stepCount+3] = 'W';
        if (floodfillnum == 1){
          pathArray[2*(stepCount+1)] = locY;
          pathArray[2*(stepCount+1)+1] = locX-1;
          for (int i=0; i<2*(stepCount+1)+1; i++){
            possiblePaths[noPaths][i] = pathArray[i];
            possiblePaths[noPaths][i+1] = pathArray[i+1];
            i++;
          }
          
          for (int i = 0; i < stepCount+4; i++){
            directions[noPaths][i] = directionArray[i];
          }
          
          noPaths++;
        }
        findPath(locX-1, locY, floodfillnum-1, maze, pathArray, horWalls, verWalls, stepCount+1, directionArray);
      }
    }
}

int main(int argc, char **argv) {

  Robot *robot = new Robot();
  int horWalls[6][9] = {{0}};
  int verWalls[5][10] = {{0}};
  
  int horyCounter = 0;
  int veryCounter = 0;

  int i = 0;
  int charRow = 0;
  int mapRow = 0;
  int even = 1;
  int orX = 0;
  int orY = 0;
  char orDirection = 'N';
  
  char HW = '-'; //horizontal wall
  char VW = '|'; //vertical wall
  char N_0 = '^';
  char S_0 = 'v';
  char E_0 = '>';
  char W_0 = '<';
  
  string line; //stores the path that at the robot should take
  string map[11]; //stores the map
  ifstream MapPlan(FILE);
  
  cout << "--- Task 1 ---" << endl;
  
  while(getline(MapPlan,line)){
    charRow = line.length();
    map[mapRow] = line;
    mapRow = mapRow + 1;
    int horxCounter = 0;
    int verxCounter = 0;
    
    
    for(i = 0; i <= charRow; i++){
    
      if((i % 4 == 2) && (even == 1)){
        if(line[i] == HW){
          horWalls[horyCounter][horxCounter] = 1;
          horxCounter = horxCounter + 1;
          
        } else {
          horWalls[horyCounter][horxCounter] = 0;
          horxCounter = horxCounter + 1;
        }
      }
      
      if((i % 4 == 0) && (even == 0)){
        if(line[i] == VW){
          verWalls[veryCounter][verxCounter] = 1;
          verxCounter = verxCounter + 1;
        } else {
          verWalls[veryCounter][verxCounter] = 0;
          verxCounter = verxCounter + 1;
        }
      }
      
      if (line[i] == N_0 || line[i] == S_0 || line[i] == E_0 || line[i] == W_0){
        orX = (i+1)/4;
        orY = veryCounter;
        
        if (line[i] == N_0){
          orDirection = 'N';
        } else if (line[i] == S_0) {
          orDirection = 'S';
        } else if (line[i] == E_0) {
          orDirection = 'E';
        } else if (line[i] == W_0) {
          orDirection = 'W';
        }
        
      }

    }
    
    if(even == 1){
      horyCounter = horyCounter + 1;
      even = 0;
    } else {
      even = 1;
      veryCounter = veryCounter + 1;
    }
    
  }
  
  for (i = 0; i < mapRow; i++){
    cout << map[i] << endl;
  }
  
  int x = 0;
  int y = 0;

  cout << "--- Task 2 ---" << endl;
  
  //Flood Fill
  
  //all Cell values <- 45
  int maze[5][9] = {0};
  
  for(y=0;y<5;y++){
    for(x=0;x<9;x++){
      maze[y][x] = 45;
    }
  }
  
  //Goal Cell Value = 0
  maze[2][4] = 0;
  
  //Current explored value = 0
  
  int CurrentExploredValue = 0;
  int MazeValueChanged = 1;
  
  //Enter the while loop
  
  int rowCount = 0;
  int colCount = 0;
  
  while (MazeValueChanged != 0){
    MazeValueChanged = 0;
    for (rowCount = 0; rowCount<5; rowCount++){
      for (colCount = 0; colCount<9; colCount++){
        if (maze[rowCount][colCount] == CurrentExploredValue){
          
          //North
          if (horWalls[rowCount][colCount] == 0){
            if (maze[rowCount-1][colCount] == 45){
              maze[rowCount-1][colCount] = maze[rowCount][colCount] + 1;
              MazeValueChanged = 1;
            }
          }
          
          //South
          if (horWalls[rowCount+1][colCount] == 0){
            if (maze[rowCount+1][colCount] == 45){
              maze[rowCount+1][colCount] = maze[rowCount][colCount] + 1;
              MazeValueChanged = 1;
            }
          }
          
          //East
          if (verWalls[rowCount][colCount+1] == 0){
            if (maze[rowCount][colCount+1] == 45){
              maze[rowCount][colCount+1] = maze[rowCount][colCount] + 1;
              MazeValueChanged = 1;
            }
          }
          
          //West
          if (verWalls[rowCount][colCount] == 0){
            if (maze[rowCount][colCount-1] == 45){
              maze[rowCount][colCount-1] = maze[rowCount][colCount] + 1;
              MazeValueChanged = 1;
            }
          }
          
        }
      }
    }
    CurrentExploredValue = CurrentExploredValue + 1;
  }
  
  CurrentExploredValue = maze[orY][orX]+1;
  
  //Depth First Search
  
  int pathArray[2*CurrentExploredValue] = {0};
  char directionArray[CurrentExploredValue];
  int stepCount = 0;
  //cout << "Current " << CurrentExploredValue << endl;
  
  directionArray[0] = orX + 48;
  directionArray[1] = orY + 48;
  directionArray[2] = orDirection;
  
  int floodfillnum = CurrentExploredValue-1;//16
  int locX = orX;
  int locY = orY;
  int j = 0;

  findPath(locX, locY, floodfillnum, maze, pathArray, horWalls, verWalls, stepCount, directionArray);  
  
  //Modifying and printing the map
  
  string tempmap[11]; //stores temporary map for printing
  int numPrint = CurrentExploredValue-2;
  int tens = 0;
  int ones = 0;
  for (i = 0; i < noPaths; i++){
    cout << "--- Path " << i+1 << " ---"<< endl;
    numPrint = CurrentExploredValue-2;
    for (int temp = 0; temp < mapRow; temp++){
      tempmap[temp] = map[temp];
    }
    
    for(colCount=2;colCount<2*CurrentExploredValue;colCount++){
    
      if (numPrint >= 10){
        tens = numPrint/10;
        ones = numPrint%10;
        tempmap[((possiblePaths[i][colCount])*2+1)][((possiblePaths[i][colCount+1])*4+3)] = ones+48;
        tempmap[((possiblePaths[i][colCount])*2+1)][((possiblePaths[i][colCount+1])*4+2)] = tens+48;
      } else {
        tens = -48;
        ones = numPrint;
        tempmap[((possiblePaths[i][colCount])*2+1)][((possiblePaths[i][colCount+1])*4+2)] = ones+48;
        tempmap[((possiblePaths[i][colCount])*2+1)][((possiblePaths[i][colCount+1])*4+1)] = tens+48;
      }
    
      numPrint--;
      colCount++;
    }
    
    for (int k = 0; k < mapRow; k++){
        cout << tempmap[k] << endl;
    }
  }
  
  cout << "--- Task 3 ---" << endl;
  
  //Path finding
  
  char pathFinder[noPaths][100] = {0};
  int pathFinderCounter = 0;
  
  for (i = 0; i < noPaths; i++){
    pathFinderCounter = 3;
    for(j = 0; j < 3; j++){
      pathFinder[i][j] = directions[i][j];
      
    }
    
    for (j = 3; j < CurrentExploredValue+3; j++){
      if (directions[i][j] == directions[i][j-1]){
        pathFinder[i][pathFinderCounter] = 'F';
        pathFinderCounter++;
      }
      
      if (directions[i][j] == 'W' && directions[i][j-1] == 'N'){
        pathFinder[i][pathFinderCounter] = 'L';
        pathFinderCounter++;
        pathFinder[i][pathFinderCounter] = 'F';
        pathFinderCounter++;
      }
      
      if (directions[i][j] == 'E' && directions[i][j-1] == 'S'){
        pathFinder[i][pathFinderCounter] = 'L';
        pathFinderCounter++;
        pathFinder[i][pathFinderCounter] = 'F';
        pathFinderCounter++;
      }
      
      if (directions[i][j] == 'N' && directions[i][j-1] == 'E'){
        pathFinder[i][pathFinderCounter] = 'L';
        pathFinderCounter++;
        pathFinder[i][pathFinderCounter] = 'F';
        pathFinderCounter++;
      }
      
      if (directions[i][j] == 'S' && directions[i][j-1] == 'W'){
        pathFinder[i][pathFinderCounter] = 'L';
        pathFinderCounter++;
        pathFinder[i][pathFinderCounter] = 'F';
        pathFinderCounter++;
      }
      
      if (directions[i][j] == 'E' && directions[i][j-1] == 'N'){
        pathFinder[i][pathFinderCounter] = 'R';
        pathFinderCounter++;
        pathFinder[i][pathFinderCounter] = 'F';
        pathFinderCounter++;
      }
      
      if (directions[i][j] == 'W' && directions[i][j-1] == 'S'){
        pathFinder[i][pathFinderCounter] = 'R';
        pathFinderCounter++;
        pathFinder[i][pathFinderCounter] = 'F';
        pathFinderCounter++;
      }
      
      if (directions[i][j] == 'S' && directions[i][j-1] == 'E'){
        pathFinder[i][pathFinderCounter] = 'R';
        pathFinderCounter++;
        pathFinder[i][pathFinderCounter] = 'F';
        pathFinderCounter++;
      }
      
      if (directions[i][j] == 'N' && directions[i][j-1] == 'W'){
        pathFinder[i][pathFinderCounter] = 'R';
        pathFinderCounter++;
        pathFinder[i][pathFinderCounter] = 'F';
        pathFinderCounter++;
      }
      
    }
    
  }
  
  //Finding number of turns per path and least number of turns shortest path;
  int numTurns[noPaths] = {0};
  int turnCounter = 0;
  int leastTurns = 0;
  int leastSteps = 0;
  
  for (i=0;i<noPaths;i++){
    turnCounter = 0;
    for(j=0;j<100;j++){
      if (pathFinder[i][j] == 'L' || pathFinder[i][j] == 'R'){
        turnCounter++;
      }
    }
    //cout << turnCounter << endl;
    numTurns[i] = turnCounter;
    
    if (turnCounter <= numTurns[leastTurns]){
      leastTurns = i;
      leastSteps = CurrentExploredValue - 1 + turnCounter;
    }
  }
  
  for (int temp = 0; temp < mapRow; temp++){
      tempmap[temp] = map[temp];
  }
    
  numPrint = CurrentExploredValue-2;  
    
  for(colCount=2;colCount<2*CurrentExploredValue;colCount++){
  
    if (numPrint >= 10){
        tens = numPrint/10;
        ones = numPrint%10;
        tempmap[((possiblePaths[leastTurns][colCount])*2+1)][((possiblePaths[leastTurns][colCount+1])*4+3)] = ones+48;
        tempmap[((possiblePaths[leastTurns][colCount])*2+1)][((possiblePaths[leastTurns][colCount+1])*4+2)] = tens+48;
    } else {
        tens = -48;
        ones = numPrint;
        tempmap[((possiblePaths[leastTurns][colCount])*2+1)][((possiblePaths[leastTurns][colCount+1])*4+2)] = ones+48;
        tempmap[((possiblePaths[leastTurns][colCount])*2+1)][((possiblePaths[leastTurns][colCount+1])*4+1)] = tens+48;
    }
      colCount++;
      numPrint--;
  }
    
  for (int k = 0; k < mapRow; k++){
        cout << tempmap[k] << endl;
  }
  
  cout << "Steps: " << leastSteps << endl;
  cout << "Path: ";
  for (i = 0; i < leastSteps+3; i++){
    cout << pathFinder[leastTurns][i];
  }
  cout << endl;
  //cout << "Least turns " << leastTurns << endl;
  cout << "--- Task 4 ---" << endl;
  cout << "File: ./PathPlanFound.txt" << endl;
  cout << "Path: ";
  for (i = 0; i < leastSteps+3; i++){
    cout << pathFinder[leastTurns][i];
  }
  
  cout << endl;
  
  ofstream writefile("../../PathPlanFound.txt");
  for (i = 0; i < leastSteps+3; i++){
    writefile << pathFinder[leastTurns][i];
  }
  writefile.close();
  
  // get the time step of the current world.
  int timeStep = (int)robot->getBasicTimeStep();

  while (robot->step(timeStep) != -1) {

    break;
  };

  delete robot;
  return 0;
}
