// File:          PhaseAController.cpp
// Date: 20 June 2020
// Description:
// Author: Raymond Ao Da Chen z5077195
// Modifications:

// You may need to add webots include files such as
// <webots/DistanceSensor.hpp>, <webots/Motor.hpp>, etc.
// and/or to add some other includes
#include <webots/Robot.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <webots/GPS.hpp>

#define TIME_STEP 64
#define MAX_SPEED 6.28
#define FILE "../../PathPlan.txt"
#define turn 2.222676802

// All the webots classes are defined in the "webots" namespace
using namespace webots;
using namespace std;

void moveForward(Motor *leftMotor, Motor *rightMotor, double currentLMP, double currentRMP);
void turnLeft(Motor *leftMotor, Motor *rightMotor, double currentLMP, double currentRMP);
void turnRight(Motor *leftMotor, Motor *rightMotor, double currentLMP, double currentRMP);


void moveForward(Motor *leftMotor, Motor *rightMotor, double currentLMP, double currentRMP){
  
  leftMotor->setPosition(currentLMP + 8.25);
  rightMotor->setPosition(currentRMP + 8.25);
}

void turnLeft(Motor *leftMotor, Motor *rightMotor, double currentLMP, double currentRMP){
  
  leftMotor->setPosition(currentLMP+ - 2.222676802);
  rightMotor->setPosition(currentRMP + 2.222676802);
}

void turnRight(Motor *leftMotor, Motor *rightMotor, double currentLMP, double currentRMP){
  
  leftMotor->setPosition(currentLMP + 2.222676802);
  rightMotor->setPosition(currentRMP - 2.222676802);
}

int main(int argc, char **argv) {

  int steps = 0;
  cout << FILE << endl;
  string path; //stores the path that at the robot should take
  ifstream PathPlan(FILE);
  getline(PathPlan,path);
  cout << path << endl;
  cout << "Done - Path plan read!" << endl;
  cout << "Start - Execute path plan!" << endl;
  
  steps = path.length()-3; //number of total steps (minus the first 3 that will always be there)
 
  // create the Robot instance.
  Robot *robot = new Robot();
  //Initialise sensors
  
  DistanceSensor *frontSensor = robot->getDistanceSensor("dsF");
  frontSensor->enable(TIME_STEP);
  DistanceSensor *leftSensor = robot->getDistanceSensor("dsL");
  leftSensor->enable(TIME_STEP);
  DistanceSensor *rightSensor = robot->getDistanceSensor("dsR");
  rightSensor->enable(TIME_STEP);
  
  //Initialise Motors
  
  Motor *leftMotor = robot->getMotor("left wheel motor");
  leftMotor->setPosition(0.0);
  leftMotor->setVelocity(MAX_SPEED);
  Motor *rightMotor = robot->getMotor("right wheel motor");
  rightMotor->setPosition(0.0);
  rightMotor->setVelocity(MAX_SPEED);
  
  PositionSensor *leftMotorPosition = robot->getPositionSensor("left wheel sensor");
  leftMotorPosition->enable(TIME_STEP);
  PositionSensor *rightMotorPosition = robot->getPositionSensor("right wheel sensor");
  rightMotorPosition->enable(TIME_STEP);
  
  // get the time step of the current world.
  int timeStep = (int)robot->getBasicTimeStep();
  
  string leftWall = "N";
  string frontWall = "N";
  string rightWall = "N";
  
  int i = 3; //path/step counter

  double expectedLMP = 0.0;
  double expectedRMP = 0.0;
  double time = 0.0;
  
  int currRow = path[0] - 48; // convert char into integer
  int currCol = path[1] - 48; // convert char into integer
  int newRow = currRow;
  int newCol = currCol;
  char currHead = path[2]; //storing the heading of the robot
  char newHead = currHead;
  
  while (robot->step(timeStep) != -1) {
    // Read the sensors:
    
    robot->step(TIME_STEP);
    //locating the walls
    double frontD = frontSensor->getValue();
    double leftD = leftSensor->getValue();
    double rightD = rightSensor->getValue();
    
    //locating the time elapsed during simulation
    double simTime = robot->getTime();
    
    //Detecting the walls
    if (frontD <= 800.00) {
      frontWall = "Y";
    } else {
      frontWall = "N";
    }
    
    if (leftD <= 800.00) {
      leftWall = "Y";
    } else {
      leftWall = "N";
    }
    
    if (rightD <= 800.00) {
      rightWall = "Y";
    } else {
      rightWall = "N";
    }
    
    char F = 'F';
    char L = 'L';
    char R = 'R';
    

    //Main part of the controller
    if (simTime >= time || time == 0){
        if (path[i] == F){
          
          //Moving forward
          moveForward(leftMotor, rightMotor, expectedLMP, expectedRMP);
          
          //Reduced speed for increased accuracy
          leftMotor->setVelocity(0.8*MAX_SPEED);
          rightMotor->setVelocity(0.8*MAX_SPEED);
          expectedLMP = expectedLMP + 8.25;
          expectedRMP = expectedRMP + 8.25;
          
          //Printing console information
          if (i-3 >= 0 && i-3<=9){
            cout << "Step: 0" << i-3 << ", ";
          } else {
            cout << "Step: " << i-3 << ", ";
          }
          cout << "Row: " << newRow << ", ";
          cout << "Column: " << newCol << ", ";
          cout << "Heading: " << newHead << ", ";
          cout << "Left Wall: " << leftWall << ", ";
          cout << "Front Wall: " << frontWall << ", ";
          cout << "Right Wall: " << rightWall << ", " << endl;
          
          //Determining the row, column positions
          if (currHead == 'N'){
            newRow = currRow + -1;
            newCol = currCol;
          } else if (currHead == 'E'){
            newRow = currRow;
            newCol = currCol + 1;
          } else if (currHead == 'S'){
            newRow = currRow + 1;
            newCol = currCol;
          } else if (currHead == 'W'){
            newRow = currRow;
            newCol = currCol + -1;
          }
          
          currRow = newRow;
          currCol = newCol;
          
          time = time + 2;
          i=i+1;
          
        } else if (path[i] == L){
        
          //Turning left
          turnLeft(leftMotor, rightMotor, expectedLMP, expectedRMP);
          
          //Reduce speed for increased accuracy
          leftMotor->setVelocity(0.4*MAX_SPEED);
          rightMotor->setVelocity(0.4*MAX_SPEED);
          expectedLMP = expectedLMP + -turn;
          expectedRMP = expectedRMP + turn;
          
          //Printing console information
          if (i-3 >= 0 && i-3<=9){
            cout << "Step: 0" << i-3 << ", ";
          } else {
            cout << "Step: " << i-3 << ", ";
          }
          cout << "Row: " << newRow << ", ";
          cout << "Column: " << newCol << ", ";
          cout << "Heading: " << newHead << ", ";
          cout << "Left Wall: " << leftWall << ", ";
          cout << "Front Wall: " << frontWall << ", ";
          cout << "Right Wall: " << rightWall << ", " << endl;
          
          //Determining the headings
          if (currHead == 'N'){
            newHead = 'W';
          } else if (currHead == 'E'){
            newHead = 'N';
          } else if (currHead == 'S'){
            newHead = 'E';
          } else if (currHead == 'W'){
            newHead = 'S';
          }
          
          currHead = newHead;
          time = time + 2;
          i=i+1;
          
        } else if (path[i] == R){
          
          //Turning right
          turnRight(leftMotor, rightMotor, expectedLMP, expectedRMP);
          //Reduce speed for increased accuracy
          leftMotor->setVelocity(0.4*MAX_SPEED);
          rightMotor->setVelocity(0.4*MAX_SPEED);
          expectedLMP = expectedLMP + turn;
          expectedRMP = expectedRMP + -turn;
          
          //Printing console information
          if (i-3 >= 0 && i-3<=9){
            cout << "Step: 0" << i-3 << ", ";
          } else {
            cout << "Step: " << i-3 << ", ";
          }
          cout << "Row: " << newRow << ", ";
          cout << "Column: " << newCol << ", ";
          cout << "Heading: " << newHead << ", ";
          cout << "Left Wall: " << leftWall << ", ";
          cout << "Front Wall: " << frontWall << ", ";
          cout << "Right Wall: " << rightWall << ", " << endl;
          
          //Determining the headings
          if (currHead == 'N'){
            newHead = 'E';
          } else if (currHead == 'E'){
            newHead = 'S';
          } else if (currHead == 'S'){
            newHead = 'W';
          } else if (currHead == 'W'){
            newHead = 'N';
          }
          
          currHead = newHead;
          time = time + 2;  
          i=i+1;
        }
    
    //For the final update
    if (i == steps+3 && simTime >= time){
      cout << "Step: " << i-3 << ", ";
      cout << "Row: " << newRow << ", ";
      cout << "Column: " << newCol << ", ";
      cout << "Heading: " << newHead << ", ";
      cout << "Left Wall: " << leftWall << ", ";
      cout << "Front Wall: " << frontWall << ", ";
      cout << "Right Wall: " << rightWall << ", " << endl;
      cout << "Done - Path plan executed" << endl;
      break;
  }
    }

  }


  delete robot;
  return 0;
}
