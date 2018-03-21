/*
**  TODO.cpp - Homework 5: TODO List
**  Cole Helbling, CISP 400
**  16 March 2018
*/

#include <algorithm>
#include <cassert>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const string OUTFILE = "todolist.dat";
class Task;

// Function Prototypes
void ProgramGreeting();
void Menu(vector<Task> &, char &);
void addRecord(vector<Task> &);
void displayRecord(vector<Task> &);
void deleteRecord(vector<Task> &);
void markComplete(vector<Task> &);
void deleteAllRecords(vector<Task> &);
int ack(int, int);

class Task
{
private:
	string task;
	string date;
	// CITATION - How to auto increment task ID: https://stackoverflow.com/a/17858383
	static int currID;
	int task_id;
	bool complete;
public:
	// Feature 3 - Auto increment task ID using initialization list
	Task(string t = "", string d = "", bool c = false) : task_id(currID++)
	{
		// Feature 1 - Assert current ID hasn't underflowed
		assert(currID >= -1);

		task = t;
		complete = c;

		if (d == "")
		{
			time_t now = time(0);
			tm * time = localtime(&now);
			date = to_string(1 + time->tm_mon) + "/" + to_string(time->tm_mday) + "/" + to_string(1900 + time->tm_year);
		}
		else
			date = d;
	}

	void log(ofstream & out)
	{
		out << task_id << ","
			<< task << ","
			<< date << ","
			<< complete << endl;
	}

	void setCurrID(int);

	// Feature 4 - Mark item as completed
	void setCompletion(bool status)
	{
		complete = status;
	}

	int getCompletion()
	{
		return complete;
	}

	int getID()
	{
		return task_id;
	}

	int getCurrID()
	{
		return currID;
	}

	friend istream& operator>>(istream & input, Task & task)
	{
		string line, id;
		string completed;

		getline(input, line);
		while (input.peek() == '\n')
			input.get();

		istringstream iss(line);

		getline(iss, id, ',');
		getline(iss, task.task, ',');
		getline(iss, task.date, ',');
		getline(iss, completed);

		task.task_id = atoi(id.c_str());
		task.complete = atoi(completed.c_str());

		return input;
	}

	// Specification 2b - Overload the << operator
	friend ostream& operator<<(ostream & output, const Task & task)
	{
		return output << left << setw(4) << task.task_id << setw(30) << task.task
			   << setw(15) << task.date << setw(4) << task.complete << endl;
	}
};

int Task::currID = 0;

void Task::setCurrID(int i)
{
	currID = i;
}

int main()
{
	Task task;
	vector<Task> tasks;
	ifstream load;
	ofstream save;
	char choice;
	int count = 1;

	ProgramGreeting();

	// Specification 4 - Load tasks from file (persistent list)
	load.open(OUTFILE);
	while (load >> task)
	{
		count++;
		if (task.getCurrID() < count)
			task.setCurrID(count + 1);
		else
			task.setCurrID(task.getID() + 1);

		tasks.push_back(task);
	}
	load.close();

	do
	{
		cin.clear();
		cin.sync();
		Menu(tasks, choice);
	} while (choice != 'q' && choice != 'Q');
	// Specification 5 - Create a command symbol to quit

	save.open(OUTFILE);
	for (size_t i = 0, max = tasks.size(); i != max; i++)
		tasks[i].log(save);
	save.close();

	// Feature 2 - Tell the user how many tasks they have left
	count = 0;
	for (int i = 0, max = tasks.size(); i != max; i++)
	{
		if (tasks[i].getCompletion() == 0)
			count++;
	}

	if (count > 0)
		cout << "You have " << count << " task(s) to do. Get on it!\n";
	else
		cout << "You have finished all your tasks! Good job!\n";

	return 0;
}

void ProgramGreeting()
{
	cout << "Welcome to \"TODO\", a program for creating a todo list! The task IDs reset only if you completely delete your todo list.\n";
}

void Menu(vector<Task> & tasks, char & choice)
{
	cout << "\n\n+ :: Add new task\n? :: Display all tasks\n- :: Remove task by ID\n/ :: Mark item as complete\nx :: Delete all tasks\nr :: Recursion example\nq :: Save tasks and exit\n\n";
	cin >> choice;

	switch (choice)
	{
		// Specification 1 - Allow the user to enter tasks with a '+' symbol
		case '+':
			addRecord(tasks);
			break;
		// Specification 2 - Allow the user to display tasks with '?'
		case '?':
			cout << endl;
			cout << left << setw(4) << "ID" << setw(30) << "Task"
			   	 << setw(15) << "Date Added" << setw(4) << "Complete" << endl;
			for (size_t i = 0, max = tasks.size(); i != max; i++)
			 	cout << tasks[i];
			break;
		// Specification 3 - Allow the user to remove tasks with a '-' symbol
		case '-':
			deleteRecord(tasks);
			break;
		case '/':
			markComplete(tasks);
			break;
		case 'x':
			// Feature 5 - Delete all records in the vector
			//	(if accidental, can send SIGINT to recover previous data because changes are only committed on exit; data added in that run is lost)
			deleteAllRecords(tasks);
			break;
		case 'r':
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					cout << "Ackermann(" << i << ", " << j << ") = " << ack(i, j) << endl;
		case 'Q':
		case 'q':
			break;
		default:
			cout << "Invalid choice.\n";
			break;
	}
}

void addRecord(vector<Task> & tasks)
{
	string desc;

	cout << "\n(Disclaimer: duplicate names ARE allowed)\nWhat is the task? ";
	cin.ignore();
	getline(cin, desc, '\n');

	while (cin.fail())
	{
		cin.clear();
		cin.sync();  // cin.ignore() causes the program to hang here. TODO: research why
		cout << "\nInvalid input.\n\nWhat is the task? ";
		getline(cin, desc, '\n');
	}

	Task tnew(desc);
	tasks.push_back(tnew);

	return;
}

void deleteRecord(vector<Task> & tasks)
{
	int element;

	if (tasks.size() == 0)
	{
		cout << "Please add at least one item before you try to delete a record.\n";
		return;
	}

	cout << "Which item would you like to delete? ";
	cin >> element;

	while(cin.fail() || element <= 0)
	{
		cin.clear();
		cin.ignore();
		cout << "Invalid item.\n\nWhich item would you like to delete (positive integers only)? ";
		cin >> element;
	}

	tasks.erase(tasks.begin() + (element - 1));

	return;
}

void markComplete(vector<Task> & tasks)
{
	int element;

	if (tasks.size() == 0)
	{
		cout << "Please add at least one item before you try to mark a record as complete.\n";
		return;
	}

	cout << "Which item would you like to mark as complete? ";
	cin >> element;

	while(cin.fail() || element <= 0)
	{
		cin.clear();
		cin.ignore();
		cout << "Invalid item.\n\nWhich item would you like to mark complete (positive integers only)? ";
		cin >> element;
	}

	tasks[element - 1].setCompletion(true);

	return;
}

void deleteAllRecords(vector<Task> & tasks)
{
	char in;
	Task task;

	cout << "This is irrecoverable. Are you sure you wish to delete all your tasks (Y/N)? ";
	cin >> in;

	if (in == 'Y')
	{
		cout << "All your tasks have been deleted!\n";
		tasks.clear();
	}
	else
		cout << "Tasks were not deleted.\n";

	task.setCurrID(1);

	return;
}

// Ackermann's function
int ack(int i, int j)
{
	if (i == 0)
		return j + 1;
	if (j == 0)
		return ack(i - 1, 1);

	return ack(i - 1, ack(i, j - 1));
}
