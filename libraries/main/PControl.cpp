#include "PControl.h"
#include "Printer.h"
extern Printer printer;

inline float angleDiff(float a) {
  while (a<-PI) a += 2*PI;
  while (a> PI) a -= 2*PI;
  return a;
}

PControl::PControl(void) {
}

void PControl::init(const int totalWayPoints_in, const int stateDims_in, double * wayPoints_in) {
  stateDims = stateDims_in;
  totalWayPoints = totalWayPoints_in;
  wayPoints = wayPoints_in;
  veerLeft = true;
  rfReadingsSize = 0;
}

int PControl::getWayPoint(int dim) {
  return wayPoints[currentWayPoint*stateDims+dim];
}

void PControl::calculateControl(state_t * state) {
  updatePoint(state->x, state->y);
  if (currentWayPoint == totalWayPoints) return; // stops motors at final point

  // set up variables
  int x_des = getWayPoint(0);
  int y_des = getWayPoint(1);

  yaw_des = atan2(y_des - state->y, x_des - state->x);
  yaw = state->heading;
  u = Kp*angleDiff(yaw_des - yaw);

  uL = max(0.0,min(255.0,(avgPower - u)*Kl));
  uR = max(0.0,min(255.0,(avgPower + u)*Kr));

}

void PControl::rfNavigateLoop(RF& rf) {
  rfReadings[rfReadingsSize] = rf.getPower(); //starts at 0
  ++rfReadingsSize;
  
  if (rfReadingsSize == RFLOOPREADINGS) {
    //remove all readings under 215, calculate average of first half and second half of readings
    unsigned int sum = 0;
    unsigned int numReadings = 0;
    for (unsigned int index = 0; index < RFLOOPREADINGS/2; ++index) { //for each reading in first block of array
      if (rfReadings[index] > RFEXCLUDEBELOW) {
        sum += rfReadings[index];
        ++numReadings;
      }
    }
    firstAverage = (double)sum / (double)numReadings;

    sum = 0;
    numReadings = 0;
    for (unsigned int index = RFLOOPREADINGS/2; index < RFLOOPREADINGS; ++index) { //for each reading in second block of array
      if (rfReadings[index] > RFEXCLUDEBELOW) {
        sum += rfReadings[index];
        ++numReadings;
      }
    }
    secondAverage = (double)sum / (double)numReadings;

    if (secondAverage < firstAverage) {
      veerLeft = !veerLeft; //flip veering direction
    }

    rfReadingsSize = 0; //reset readings
  }
  
  //adjust hardcoded average motor thrust and motor power differential below
  
  //set motor control efforts based on veerLeft
  if (veerLeft) {
    uL = 40;
    uR = 60;
  }
  else {
    uL = 60;
    uR = 40;
  }
}

String PControl::printAverages(void) {
  String printString = "First Ave: " + String(firstAverage) +
  " Second Ave: " + String(secondAverage) +
  " Veering Direction:";
  if (veerLeft)
    printString += "Left";
  else
    printString += "Right";
  printString += " RFReadingsSize: " + String(rfReadingsSize);
  /*" RF Readings: ";
  for (int i = 0; i < rfReadingsSize; ++i)
    printString += String(rfReadings[i]) + " "; */
  return printString;
}

String PControl::printString(void) {
  String printString = "PControl: Yaw_Des: " + String(yaw_des*180.0/PI)
    + " Yaw: " + String(yaw*180.0/PI)
    + " u: " + String(u) + " uL: " + String(uL) + " uR: " + String(uR); 

  return printString; 
}

String PControl::printWaypointUpdate(void) {
  String wayPointUpdate = "PControl: Current Waypoint: " + String(currentWayPoint)
    + " Distance from Waypoint: " + String(dist);
  return wayPointUpdate;
}

void PControl::updatePoint(float x, float y) {
  if (currentWayPoint == totalWayPoints) return; // don't check if finished

  int x_des = getWayPoint(0);
  int y_des = getWayPoint(1);
  dist = sqrt(pow(x-x_des,2) + pow(y-y_des,2));

  if (dist < SUCCESS_RADIUS && currentWayPoint < totalWayPoints) {
    String changingWPMessage = "Got to waypoint " + String(currentWayPoint)
      + ", now directing to next point";
    int cwpmTime = 20;
    currentWayPoint++;
    if (currentWayPoint == totalWayPoints) {
      changingWPMessage = "Congratulations! You completed the path! Stopping motors.";
      uR=0;
      uL=0;
      cwpmTime = 0;
    }
    printer.printMessage(changingWPMessage,cwpmTime);
  }
}
