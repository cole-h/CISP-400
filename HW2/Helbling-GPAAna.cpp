/*
**  GPAAna.cpp - Homework 2: GPA Analyzer
**  Cole Helbling, CISP 400
**  7 Feb 2018
*/

#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <cctype>
#include <iomanip>

using namespace std;

// Function Prototype
void showMenu();
void Log();
void ProgramGreeting();
void getMax(int&);

// Specification 2: Create a Grade class which holds the test score and the letter grade.
class Grade
{
private:
	int grade;
	char letter;

public:
	void setGrade(int);
	char getLetter();
	int getGrade();

	Grade()
	{
		grade = -1;
		letter = 'I';
	}
};

void Grade::setGrade(int g)
{
	grade = g;
	if (g >= 90 && g <= 100)
		letter = 'A';
	else if (g >= 80 && g < 90)
		letter = 'B';
	else if (g >= 70 && g < 80)
		letter = 'C';
	else if (g >= 60 && g < 70)
		letter = 'D';
	else if (g < 60)
		letter = 'F';
}

char Grade::getLetter()
{
	return letter;
}

int Grade::getGrade()
{
	return grade;
}


int main()
{
	ProgramGreeting();
	showMenu();

	// Specification 1: Use a dynamically allocated array to handle an unlimited number of test scores.
	Grade* grades = nullptr;
	Grade* temp = nullptr;
	int size = 1;
	int old, total, grade;
	int points = 0;
	double average;
	ofstream output;
	string fname;

	// Feature 1: Name the file to allow multiple runs with different data
	cout << "Please name the file (include .txt at the end; make sure the name is unique so you don't overwrite another file by accident): ";
	cin.clear();
	cin.ignore();
	getline(cin, fname);
	output.open(fname);

	grades = new Grade[size];

	getMax(total);

	cout << "Enter as many grades as you would like; input any non-integer character to exit\n\nEnter earned points on assignement #" << size << ": ";
	cin >> grade;

	while (cin)
	{
		// Specification 3: Use an assert to protect the letter grade by not allowing negative scores.
		assert(grade >= 0);

		//add another grade:
		old = size;
		size++;
		temp = new Grade[size];
		for (int i = 0; i < old; i++)
			temp[i] = grades[i];
		temp->setGrade(grade);
		points += grade;

		// Specification 6: Log each individual grade to a .txt file. Make sure the total goes here when the program quits, too.
		output << "Assignment #" << (size - 1) << ": " << grade << endl;

		delete[] grades;
		grades = temp;
		temp = nullptr;

		cout << "Enter as many grades as you would like; input any non-integer character to exit\n\nEnter earned points on assignement #" << size << ": ";
		cin >> grade;
	}

	average = ((double) points / total) * 100;

	if (!cin)
	{
		// Specification 5: Display average grade with appropriate headings
		cout << setprecision(2) << fixed << showpoint;
		cout << "Earned " << points << " points / " << total << " points = " << average << "%" << endl;
		cout << "Average points earned: " << ((double) points / size) << endl;
		output << "\nEarned " << points << " points / " << total << " points = " << average << "%" << endl << "Average points earned: " << ((double) points / size) << endl;

		// Feature 4: Encourage the student whose grades are being input.
		if (average >= 90 && average <= 100)
			output << "You rock! Keep it up!\n";
		else if (average >= 80 && average < 90)
			output << "You're getting there! Work harder!\n";
		else if (average >= 70 && average < 80)
			output << "You can do better.\n";
		else
			output << "You need to try harder.\n";
	}

	output.close();
	delete[] grades;
	grades = nullptr;

	// Feature 2: Allow multiple students by rerunning the program
	char again;
	cout << "Would you like to calculate another student's data? y/N\n";
	cin.clear();
	cin.ignore();
	cin >> again;
	if (again == 'y' || again == 'Y')
		main();
	else
		return 0;
}

void showMenu()
{
	// Specification 4: Create a simple menu: 1 - help, 2 - enter grade, 3 - quit. Use `char user_menu;` for menu input
	char user_menu;

	cout << "1 - help\n2 - enter grade(s)\n3 - quit\n\n";

	do
	{
		cin >> user_menu;

		switch (user_menu)
		{
			case 49: ProgramGreeting(); continue;
			case 50: return;
			case 51: exit(0);
			default:
				cout << "Invalid input of " << user_menu << ".\n"; continue;
		}
	} while (user_menu != 50);
}

void ProgramGreeting()
{
	cout << "\n\nThis is the second homework assignment, \"GPA Analyzer\" -- a program to help calculate the average score. Enter as many point values as you want after entering in the total number of points.\n\n";
	return;
}


// Feature 3: Ask for max points and pass by reference
void getMax(int & max)
{
	do
	{
		cout << "What are the total points possible (positive integers only)? ";
		cin >> max;
	} while (max < 0);
}