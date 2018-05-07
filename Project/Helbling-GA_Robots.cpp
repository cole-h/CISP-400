/*
**  GA_Robots.cpp - Final Project: Genetic Algorithm Robots
**  Cole Helbling, CISP 400
**  07 May 2018
*/

#include <algorithm>
#include <cassert>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Robot
{
public:
	int energy = 0;
	int sensors[] {0,0,0,0};
}


/*

Sensors.
• Sense if there is an object (wall, battery, nothing, don’t care) next to them in a given direction: (North, South, East, West).
• The direction it is facing (and moving).
• Keep track of total energy.

Behaviors.
• Move forward 1 square. This costs 1 energy.
• Rotate left 90 degrees for no cost.
• Rotate right 90 degrees for no cost.
• Pick up battery (if standing on battery) and gain +5 units of power.
• Do nothing.
• You can have up to 5 behaviors per gene. Code Do nothing for empty spaces if you use less than that.

The degree to which the robot successfully harvests energy from
the environment is called ’fitness’. We measure that by the total
amount of power harvested when each individual robot’s time ends.
When we finish with the entire population of 200, we calculate an
average fitness score for the population. Save the average fitness for
each generation. You will most likely see slow and steady improve-
ment over time - evolution at work. When the simulation completes,
print out the average fitness scores on the console. This is even more
effective if you are able to draw a console graph (not a requirement).

Constraints.
• Create a 12 by 12 square room.
• Start your robot on a random square with 5 units of power.
• Populate 40% of the squares with batteries.
• Each robot will move a total of 12-25 times unless it runs out of power before that (you decide at the start of the simulation). The robot’s score is the amount of power it has when it reaches this exit condition.
• Robots that hit walls with extra moves will still try to move (and fail and lose 1 power each time they hit the wall).
• Use a robot population of 200 individuals. Run all the robots through the room before you breed them.

*/
