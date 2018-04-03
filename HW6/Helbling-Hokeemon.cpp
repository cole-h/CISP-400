/*
**  Hokeemon.cpp - Homework 6: Hokeemon
**  Cole Helbling, CISP 400
**  31 March 2018
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

class Creature;

// Function Prototypes
void Feed(vector<unique_ptr<Creature>> &);
void Listen(vector<unique_ptr<Creature>> &);
void Menu(vector<unique_ptr<Creature>> &, char &);
void PassTime(vector<unique_ptr<Creature>> &);
void Play(vector<unique_ptr<Creature>> &);
void ProgramGreeting();
void Resurrect(vector<unique_ptr<Creature>> &);

// Specification 6 - Implement with an abstract base class called Creature()
class Creature
{
public:
	bool dead = false;
	bool unresponsive = false;
	int boredom = 0;
	int hunger = 0;
	string mood;
	string name;

	// Specification 3 - Use a constructor to initialize hunger and boredom to random numbers between 0 - 9
	// NOTE - Hunger is initialized to random between 0 - 4 due to death occurring at 10
	Creature(string n)
	{
		// Feature 1 - Name creature
		name = n;
		hunger = rand() % 5;
		boredom = rand() % 10;

		// Feature 3 - Assert to make sure hunger and boredom don't underflow
		assert(hunger >= 0);
		assert(boredom >= 0);
	}

	virtual string getName() = 0;
	virtual void incrementHunger() = 0;
	virtual void incrementBoredom() = 0;

	// Feature 2 - Moods based on boredom level
	void setMood()
	{
		if (boredom >= 0 && boredom < 5)
			mood = "Happy";
		if (boredom >= 5 && boredom < 10)
			mood = "OK";
		if (boredom >= 10 && boredom < 15)
			mood = "Frustrated";
		if (boredom >= 15 && boredom < 20)
			mood = "Mad";
		if (boredom >= 20)
			mood = "Catatonic";
		if (hunger >= 10)
			mood = "Dead";
	}

	string getMood()
	{
		return mood;
	}

	int getHunger()
	{
		return hunger;
	}

	int getBoredom()
	{
		return boredom;
	}

	void decrementHunger()
	{
		hunger -= 4;

		if (hunger < 0)
			hunger = 0;
	}

	void decrementBoredom()
	{
		boredom -= 4;

		if (boredom < 0)
			boredom = 0;
	}

	void setDead(int revive = 0)
	{
		if (revive)
		{
			dead = false;
			unresponsive = false;
		}
		if (!revive)
		{
			dead = true;
			cout << this->getName() << " just died from neglect! You monster!\n";
		}
	}

	void setCatatonic()
	{
		unresponsive = true;
		cout << this->getName() << " is now unresponsive due to neglect.\n";
	}

	int getStatus()
	{
		if (dead)
			return 1;
		if (unresponsive)
			return 2;

		return 0;
	}

	void Reset()
	{
		hunger = 0;
		boredom = 0;
		setMood();
	}
};

// Specification 7a - Instantiate using child classes (at least 2), each child represents a different type of Hokeemon.
class Big : public Creature
{
public:
	Big(string n) : Creature(n)
	{
	}

	string getName()
	{
		return "Big " + name;
	}

	void incrementHunger()
	{
		if (hunger < 10)
			hunger += 2;
	}

	void incrementBoredom()
	{
		if (boredom < 20)
			boredom += 2;
	}
};

// Specification 7b - Instantiate using child classes (at least 2), each child represents a different type of Hokeemon.
class Small : public Creature
{
public:
	Small(string n) : Creature(n)
	{
	}

	string getName()
	{
		return "Small " + name;
	}

	void incrementHunger()
	{
		if (hunger < 10)
			hunger++;
	}

	void incrementBoredom()
	{
		if (boredom < 20)
			boredom++;
	}
};


int main()
{
	srand(time(NULL));

	char choice;
	int max = (rand() % 6) + 1;
	string name;
	vector<unique_ptr<Creature>> creatures;
	vector<string> names;

	ProgramGreeting();

	cout << "You have been given " << max << " creatures, congratulations!\n";
	for (int i = 0; i < max; i++)
	{
		cout << "Input the name of creature #" << i + 1 << ": ";
		cin >> name;
		names.push_back(name);
	}

	for (int i = 0; i < max; i++)
	{
		int random = rand() % 2;

		if (random == 1)
			creatures.push_back(make_unique<Big>(names[i]));
		else if (random == 0)
			creatures.push_back(make_unique<Small>(names[i]));

		creatures[i]->setMood();
	}

	do
	{
		cin.clear();
		cin.sync();
		Menu(creatures, choice);
		PassTime(creatures);
	} while (choice != 'q' && choice != 'Q');

	cout << "\nGoodbye! Your living creatures will be released back into the wild.\n";

	return 0;
}

void ProgramGreeting()
{
	cout << "Welcome to HW6, \"Hokeemon\", a Pokemon wannabe! 'Big' creatures gain hunger and boredom twice as fast as 'Small' creatures.\n\n";
}

// Specification 1 - You will need to implement a PassTime() method which will increase hunger and boredom by 1. This will be called after Listen, Play or Feed.
void PassTime(vector<unique_ptr<Creature>> & creatures)
{
	for (size_t i = 0, max = creatures.size(); i != max; i++)
	{
		creatures[i]->incrementHunger();
		creatures[i]->incrementBoredom();
		// Specification 4 - If hunger exceeds 10, your Hokeemon will die of starvation!
		if (creatures[i]->getHunger() >= 10 && creatures[i]->getStatus() != 1)
			creatures[i]->setDead();
		// Specification 5 - If boredom exceeds 20, your hokeemon will slip into an unresponsive, catatonic state.
		if (creatures[i]->getBoredom() >= 20 && creatures[i]->getStatus() == 0)
			creatures[i]->setCatatonic();

		creatures[i]->setMood();
	}
}

// Specification 2 - Create a numeric menu which will allow you to select Listen, Play, Feed, or Quit. Do not accept any other options.
void Menu(vector<unique_ptr<Creature>> & creatures, char & choice)
{
	size_t j = 0;

	for (size_t i = 0, max = creatures.size(); i != max; i++)
	{
		if (creatures[i]->getStatus() == 1)
			j++;
		if (j >= max)
		{
			cout << "\nAll your creatures are dead. Seek professional help before you cause harm to living beings.\n";
			choice = 'q';
			return;
		}
	}

	cout << "\n\nl :: Listen to a creature\np :: Play with a creature\nf :: Feed a creature\nr :: Resurrect a dead creature at the cost of a living creature\nq :: Exit\n\n";
	cin >> choice;

	switch (choice)
	{
		case 'l':
		case 'L':
			Listen(creatures);
			break;
		case 'p':
		case 'P':
			Play(creatures);
			break;
		case 'f':
		case 'F':
			Feed(creatures);
			break;
		case 'Q':
		case 'q':
			break;
		// Feature 4 - Resurrect creature at the cost of another (basically necromancy)
		case 'r':
		case 'R':
			Resurrect(creatures);
			break;
		// Feature 5 - Print all creature's data (pre-PassTime()), hidden
		case 'a':
			for (size_t i = 0, max = creatures.size(); i != max; i++)
				cout << creatures[i]->getName() << ": " << creatures[i]->getMood() << "\t" << creatures[i]->getBoredom() << " " << creatures[i]->getHunger() << " " << creatures[i]->getStatus() << endl;
			cout << endl;
			break;
		default:
			cout << "Invalid choice.\n";
			break;
	}
}


void Listen(vector<unique_ptr<Creature>> & creatures)
{
	size_t index;

	cout << "Which creature would you like to listen to? ";
	cin >> index;

	while (cin.fail() || index < 0 || index > (creatures.size() - 1))
	{
		cin.clear();
		cin.ignore();
		cout << "Invalid choice.\nWhich creature would you like to listen to? ";
		cin >> index;
	}

	if (creatures[index]->getStatus() == 0)
		cout << creatures[index]->getName() << ": My hunger is at " << creatures[index]->getHunger() << " out of 10,\n\t\tmy boredom is at " << creatures[index]->getBoredom() << " out of 20,\n\t\tand my mood is " << creatures[index]->getMood() << ".\n";
	else if (creatures[index]->getStatus() == 1)
		cout << creatures[index]->getName() << " is dead.\n";
	else if (creatures[index]->getStatus() == 2)
		cout << creatures[index]->getName() << " is unresponsive.\n";

	return;
}


void Play(vector<unique_ptr<Creature>> & creatures)
{
	size_t index;

	cout << "Which creature would you like to play with? ";
	cin >> index;

	while (cin.fail() || index < 0 || index > (creatures.size() - 1))
	{
		cin.clear();
		cin.ignore();
		cout << "Invalid choice.\nWhich creature would you like to play with? ";
		cin >> index;
	}

	if (creatures[index]->getStatus() == 0)
	{
		creatures[index]->decrementBoredom();
		cout << creatures[index]->getName() << ": Yippee! My boredom is now at " << creatures[index]->getBoredom() << " out of 20.\n";
	}
	else if (creatures[index]->getStatus() == 1)
		cout << creatures[index]->getName() << " is dead.\n";
	else if (creatures[index]->getStatus() == 2)
		cout << creatures[index]->getName() << " is unresponsive.\n";

	return;
}


void Feed(vector<unique_ptr<Creature>> & creatures)
{
	size_t index;

	cout << "Which creature would you like to feed? ";
	cin >> index;

	while (cin.fail() || index < 0 || index > (creatures.size() - 1))
	{
		cin.clear();
		cin.ignore();
		cout << "Invalid choice.\nWhich creature would you like to feed? ";
		cin >> index;
	}

	if (creatures[index]->getStatus() == 0)
	{
		creatures[index]->decrementHunger();
		cout << creatures[index]->getName() << ": Yummy! My hunger is now at " << creatures[index]->getHunger() << " out of 10.\n";
	}
	else if (creatures[index]->getStatus() == 1)
		cout << creatures[index]->getName() << " is dead.\n";
	else if (creatures[index]->getStatus() == 2)
		cout << creatures[index]->getName() << " is unresponsive.\n";

	return;
}


void Resurrect(vector<unique_ptr<Creature>> & creatures)
{
	size_t revive, kill;

	if (creatures.size() < 2)
	{
		cout << "You need at least 2 creatures to trade lives.\n";
		return;
	}

	cout << "Which creature would you like to revive, and at the cost of which other creature? ";
	cin >> revive >> kill;

	while (cin.fail() || revive < 0 || kill < 0 || revive > (creatures.size() - 1) || kill > (creatures.size() - 1))
	{
		cin.clear();
		cin.ignore();
		cout << "One of your choices was invalid.\nWhich creature would you like to revive, and at the cost of which other creature? ";
		cin >> revive >> kill;
	}

	if (creatures[revive]->getStatus() == 1 && creatures[kill]->getStatus() == 0)
	{
		creatures[revive]->Reset();
		cout << "Reviving " << creatures[revive]->getName() << ".\nThe creature now has 0 hunger and 0 boredom. Do take better care of it this time.\n";
		creatures[revive]->setDead(1);
		creatures[kill]->setDead();
	}
	else if (creatures[revive]->getStatus() != 1)
		cout << creatures[revive]->getName() << " is not (yet) dead.\n";
	else if (creatures[kill]->getStatus() != 0)
		cout << creatures[kill]->getName() << " is nota valid candidate.\n";

	return;
}
