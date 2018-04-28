/*
**  RefPig.cpp - Homework 8: Refactored Pig
**  Cole Helbling, CISP 400
**  28 April 2018
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

// Specification 1 - Create an abstract class called `Player`
class Player
{
public:
	bool again = false;
	int wins = 0;
	int losses = 0;
	int total = 0;

	virtual int Roll() = 0;

	int getTotal()
	{
		return total;
	}

	// Specification 3 - Use operator overloading where appropriate
	friend ostream & operator<<(ostream & out, const Player & player)
	{

		return out;
	}
}

class Computer : public Player
{

};

class Human : public Player
{

};

// Specification 2 - Create a class called Dice
class Dice
{
	int roll = 0;

	Dice() : Player()
	{

	}

	friend ostream & operator<<(ostream & out, const Dice & dice)
	{

		return out;
	}
}
