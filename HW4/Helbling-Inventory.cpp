/*
**  Inventory.cpp - Homework 3: Inventory Inquisitor
**  Cole Helbling, CISP 400
**  28 Feb 2018
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

const string OUTFILE = "inventory.dat";
class Item;

// Function Prototypes
void ProgramGreeting();
int Menu(vector<Item> &, int &);
void addRecord(vector<Item> &);
void displayRecord(vector<Item> &);
void changeRecord(vector<Item> &);
void deleteRecord(vector<Item> &);

class Item
{
private:
	double retail; // 100% markup of wholesale
	double wholesale;
	unsigned int quantity; // unsigned so doesn't accept negatives
	string date;
	string description;
public:
	Item(string desc = "", unsigned int q = 0, double whole = 0.00, string d = "")
	{
		// Feature 5 - Assert to make sure quantity never goes negative
		assert(q >= 0);
		retail = whole * 2;
		wholesale = whole;
		quantity = q;
		description = desc;

		if (d == "")
		{
			time_t now = time(0);
			date = ctime(&now);
		}
		else
			date = d;
	}

	void change(int what)
	{
		double whole;
		unsigned int q;
		string desc;
		bool space;

		switch (what)
		{
			case 2: // wholesale value
				cout << "What would you like to change the wholesale value to (min 5 dollars)? ";
				cin >> whole;
				while (!cin || whole < 5.0000)
				{
					cin.clear();
					cin.ignore();
					cout << "\nOnly integers greater than 5 are accepted.\n\nWhat is the wholesale value of the item? $";
					cin >> whole;
				}
				wholesale = whole;
				retail = wholesale * 2;
				break;
			case 3: // quantity
				cout << "What would you like to change the quantity to (min 0)? ";
				cin >> q;
				while (!cin || q <= 0)
				{
					cin.clear();
					cin.ignore();
					cout << "\nOnly positive integers are accepted.\n\nWhat is the quantity of the item? ";
					cin >> q;
				}
				quantity = q;
				break;
			case 5: // description
				cout << "What would you like to change the name to? ";
				cin.ignore();
				getline(cin, desc, '\n');
				space = any_of(desc.begin(), desc.end(), [](char c){return isspace(c);});
				while (cin.fail() || desc.length() > 20 || space)
				{
					cin.clear();
					cin.sync();  // cin.ignore() causes the program to hang here. TODO: research why
					cout << "\nNo more than 20 characters are accepted; whitespace is also not accepted.\n\nWhat is the name of the item (max 20 chars, no whitespace)? ";
					getline(cin, desc, '\n');
					space = any_of(desc.begin(), desc.end(), [](char c){return isspace(c);});
				}
				description = desc;
				break;
			default: cout << "That's not something you can change. Try again.\n"; break;
		}
	}

	void print()
	{
		cout << left << setw(21) << description
			 << setw(12) << quantity << fixed
			 << setprecision(2) << "$" << setw(18) << wholesale
			 << setprecision(2) << "$" << setw(15) << retail;
		cout << setw(24) << date << endl;
	}

	void log(ofstream & out)
	{
		out << description << ","
			<< quantity << ","
			<< wholesale << ","
			<< retail << ",";
		out << date << endl;
	}

	double totalWholesaleValue()
	{
		return wholesale * quantity;
	}

	string getName()
	{
		return description;
	}

	// CITATION - Data parsing modified from "Some programmer dude": https://stackoverflow.com/a/19043848
	friend istream& operator>>(istream& input, Item& item)
	{
		string quant, whole, ret, line;
		getline(input, line);
		while (input.peek() == '\n')
			input.get();

		istringstream iss(line);

		getline(iss, item.description, ',');
		getline(iss, quant, ',');
		getline(iss, whole, ',');
		getline(iss, ret, ',');
		getline(iss, item.date);

		item.quantity = atoi(quant.c_str());
		item.wholesale = atof(whole.c_str());
		item.retail = atof(ret.c_str());

		return input;
	}
};

int main()
{
	// Specification 2 - Store an unlimited number of items by using a vector<>
	vector<Item> items;
	Item item;
	string temp, desc, date;
	ifstream load;
	ofstream save;
	int choice;
	double totalValue = 0.00;

	ProgramGreeting();

	// Specification 3 - Use file I/O to save and recall your data
	load.open(OUTFILE);
	while (load >> item)
		items.push_back(item);
	load.close();

	do
	{
		Menu(items, choice);
	} while (choice != 5);

	// Feature 2 - Calculate and display the wholesale value of the inventory
	for (size_t i = 0, max = items.size(); i != max; i++)
		totalValue += items[i].totalWholesaleValue();
	cout << fixed << setprecision(2) << "Value of inventory at wholesale price: $" << totalValue << endl;

	save.open(OUTFILE);
	for (size_t i = 0, max = items.size(); i != max; i++)
		items[i].log(save);
	save.close();

	return 0;
}


void ProgramGreeting()
{
	cout << "Welcome to \"Inventory Inquisitor\", a program for managing inventory.\n";
}


int Menu(vector<Item> & items, int & choice)
{
	cout << "\n\n1. Add new record\n2. Display record\n3. Change record\n4. Delete record\n5. Exit\n6. Dump inventory to console\n\n";
	cin >> choice;
	while (!cin)
	{
		cin.clear();
		cin.ignore();
		cout << "Only integers are accepted. Try again.\n";
		cin >> choice;
	}
	switch (choice)
	{
		case 1:
			// Specification 4.a - Add
			addRecord(items);
			break;
		case 2:
			// Specification 4.b - Display
			displayRecord(items);
			break;
		case 3:
			// Specification 4.c - Change
			changeRecord(items);
			break;
		case 4:
			// Specification 4.d - Delete
			deleteRecord(items);
			break;
		case 5:
			break;
		case 6:
			// Feature 1 - Dump all items stored in vector to console
			cout << left << setw(21) << "Item Name"
				 << setw(12) << "Quantity"
				 << setw(19) << "Wholesale Price"
				 << setw(16) << "Retail Price"
				 << setw(24) << "Date Added" << endl;
			 for (size_t i = 0, max = items.size(); i != max; i++)
			 	items[i].print();
			break;
		default:
			cout << "Invalid choice.\n";
			break;
	}

	return 0;
}

void addRecord(vector<Item> & items)
{
	string desc;
	int q;
	double whole;

	cout << "What is the name of the item (max 20 chars, no whitespace)? ";
	cin.ignore();
	getline(cin, desc, '\n');
	bool space = any_of(desc.begin(), desc.end(), [](char c){return isspace(c);});
	// Specification 1.a - Validate input of name (accept everything a string would up to and including 20 characters, no whitespace)
	while (cin.fail() || desc.length() > 20 || space)// || dupe)
	{
		cin.clear();
		cin.sync();  // cin.ignore() causes the program to hang here. TODO: research why
		cout << "\nNo more than 20 characters are accepted; whitespace is also not accepted.\n\nWhat is the name of the item (max 20 chars, no whitespace)? ";
		getline(cin, desc, '\n');
		space = any_of(desc.begin(), desc.end(), [](char c){return isspace(c);});
	}

	cout << "What is the quantity of the item? ";
	cin >> q;
	// Specification 1.b - Validate input of quantity
	while (!cin || q <= 0)
	{
		cin.clear();
		cin.ignore();
		cout << "\nOnly positive integers are accepted.\n\nWhat is the quantity of the item? ";
		cin >> q;
	}

	cout << "What is the wholesale value of the item? $";
	cin >> whole;
	// Specification 1.c - Vaiidate input of wholesale value
	while (!cin || whole < 5.0000)
	{
		cin.clear();
		cin.ignore();
		cout << "\nOnly integers greater than 5 are accepted.\n\nWhat is the wholesale value of the item? $";
		cin >> whole;
	}

	Item inew(desc, q, whole);
	items.push_back(inew);

	return;
}

void displayRecord(vector<Item> & items)
{
	string element;
	bool success;

	if (items.size() == 0)
	{
		cout << "Please add at least one item before you try to display a record.\n";
		return;
	}
	cout << "Enter the name of the item you would like to display: ";
	cin.ignore();
	getline(cin, element, '\n');
	for (size_t i = 0, max = items.size(); i != max; i++)
	{
	// Feature 3 - Search for item by name
		if (items[i].getName() == element)
		{
			success = true;
			items[i].print();
		}
	}
	if (!success)
		cout << "Could not find item of name \"" << element << "\".\n";

	return;
}

void changeRecord(vector<Item> & items)
{
	string element;
	int change;
	bool success;

	if (items.size() == 0)
	{
		cout << "Please add at least one item before you try to change a record.\n";
		return;
	}
	cout << "What is the name of the record would you like to change? ";
	cin.ignore();
	getline(cin, element, '\n');
	transform(element.begin(), element.end(), element.begin(), ::tolower);
	for (size_t i = 0, max = items.size(); i != max; i++)
	{
		// Feature 4 - Change record by name
		string name = items[i].getName();
		transform(name.begin(), name.end(), name.begin(), ::tolower);
		if (name == element)
		{
			success = true;
			cout << "\nWhat would you like to change?\n1. Wholesale Value\n2. Quantity\n3. Item Name\n\n";
			cin >> change;
			switch (change)
			{
				case 1: items[i].change(2); break;
				case 2: items[i].change(3); break;
				case 3: items[i].change(5); break;
				default:
					cout << "Invalid choice.\n";
					break;
			}
		}
	}
	if (!success)
		cout << "Couldn't find item of name " << element << ".\n";

	return;
}

void deleteRecord(vector<Item> & items)
{
	int element;

	if (items.size() == 0)
	{
		cout << "Please add at least one item before you try to delete a record.\n";
		return;
	}
	cout << "Which item would you like to delete (first item is 1, second item is 2, etc.)? ";
	cin >> element;
	while(cin.fail() || element <= 0)
	{
		cin.clear();
		cin.ignore();
		cout << "Invalid item. The first item is at 1.\n\nWhich item would you like to delete (numbers only)? ";
		cin >> element;
	}
	items.erase(items.begin() + (element - 1));

	return;
}
