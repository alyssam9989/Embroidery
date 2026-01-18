#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

// Constants
const int MAX_SESSIONS = 5;
const double MIN_HOURS_GOOD = 5.0;
const double MAX_COST_GOOD = 50.0;
const int EASY_VALUE = 1;
const int INTERMEDIATE_VALUE = 2;
const int HARD_VALUE = 3;


// Enum
enum DifficultyLevel {
	EASY = EASY_VALUE,
	INTERMEDIATE = INTERMEDIATE_VALUE,
	HARD = HARD_VALUE
};

// Struct
struct Session {
	string description;
	double hours;
	double cost;
	DifficultyLevel difficulty;
};

// Function Prototypes
void showBanner();
string getNonEmptyString(const string& prompt);
double getPositiveDouble(const string& prompt);
DifficultyLevel getDifficulty();
void fillSession(Session& s);
void printSession(const Session& s);
void saveReport(const Session sessions[], int count, const string& name, double goal);
void showMenu();

// Main
int main() {
	Session sessions[MAX_SESSIONS];
	int sessionCount = 0;

	showBanner();

	string userName = getNonEmptyString("Enter your name: ");
	double weeklyGoal = getPositiveDouble("Enter your weekly goal for embroidery hours: ");

	int choice;

	do {
		showMenu();
		cin >> choice;

		while (cin.fail()) {
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid menu choice. Please enter a new choice: ";
			cin >> choice;
		}

		switch (choice) {
		case 1:
			if (sessionCount < MAX_SESSIONS) {
				fillSession(sessions[sessionCount]);
				sessionCount++;
			}
			else {
				cout << "You have reached the maximum number of sessions.\n";
			}
			break;

		case 2:
			if (sessionCount == 0) {
				cout << "No embroidery sessions recorded yet.\n";
			}
			else {
				cout << "\n" << userName << "'s Embroidery Sessions\n";
				cout << left << setw(20) << "Description"
					<< setw(10) << "Hours"
					<< setw(10) << "Cost"
					<< setw(15) << "Difficulty" << endl;

				for (int i = 0; i < sessionCount; i++) {
					printSession(sessions[i]);
				}
			}
			break;

		case 3: {
			double totalHours = 0.0;
			double totalCost = 0.0;
			int index = 0;

			while (index < sessionCount) {
				totalHours += sessions[index].hours;
				totalCost += sessions[index].cost;
				index++;
			}

			cout << "\nRecommendation for " << userName << ":\n";

			if (totalHours >= weeklyGoal && totalCost <= MAX_COST_GOOD) {
				cout << "Great job! You met your weekly goal AND stayed on budget.\n";
			}
			else if (totalHours < weeklyGoal && totalCost > MAX_COST_GOOD) {
				cout << "You may want shorter sessions or lower-cost projects.\n";
			}
			else {
				cout << "You are making steady progress. Keep going!\n";
			}
			break;
		}

		case 4:
			saveReport(sessions, sessionCount, userName, weeklyGoal);
			cout << "Report saved to report.txt\n";
			break;

		case 5:
			cout << "Goodbye, " << userName << "!\n";
			break;

		}

	} while (choice != 5);

	return 0;
}

// Functions
void showBanner() {
	cout << "=========================\n";
	cout << "    Embroidery Tracker   \n";
	cout << "=========================\n\n";
}

string getNonEmptyString(const string& prompt) {
	string input;
	do {
		cout << prompt;
		getline(cin >> ws, input);
	} while (input.empty());
	return input;
}

double getPositiveDouble(const string& prompt) {
	double value;
	do {
		cout << prompt;
		cin >> value;

		if (cin.fail() || value <= 0) {
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Please enter a positive number.\n";
		}
	} while (value <= 0);

	return value;
}

DifficultyLevel getDifficulty() {
	int choice; 
	cout << "Select difficulty (1 = Easy, 2 = Intermediate, 3 = Hard): ";
	cin >> choice;

	switch (choice) {
		case EASY_VALUE: return EASY;
		case INTERMEDIATE_VALUE: return INTERMEDIATE;
		case HARD_VALUE: return HARD;
		default: 
			cout << "Invalid choice. Defaulting to 'Easy'.\n";
			return EASY;
	}
}

void fillSession(Session& s) {
	s.description = getNonEmptyString("Session description : ");
	s.hours = getPositiveDouble("Hours spent: ");
	s.cost = getPositiveDouble("Thread cost: ");
	s.difficulty = getDifficulty();
}

void printSession(const Session& s) {
	string diff;
	if (s.difficulty == EASY) diff = "Easy";
	else if (s.difficulty == INTERMEDIATE) diff = "Intermediate";
	else diff = "Hard";

	cout << left << setw(20) << s.description
		<< setw(10) << fixed << setprecision(1) << s.hours
		<< setw(10) << fixed << setprecision(2) << s.cost
		<< setw(15) << diff << endl;
}

void saveReport(const Session sessions[], int count, const string& name, double goal) {
	ofstream outFile("report.txt");

	outFile << "Embroidery Report for " << name << endl;
	outFile << "Weekly Hour Goal: " << fixed << setprecision(1) << goal << "\n\n";

	outFile << left << setw(20) << "Description"
		<< setw(10) << "Hours"
		<< setw(10) << "Cost"
		<< setw(15) << "Difficulty" << endl;

	for (int i = 0; i < count; i++) {
		string diff;
		if (sessions[i].difficulty == EASY) diff = "Easy";
		else if (sessions[i].difficulty == INTERMEDIATE) diff = "Intermediate";
		else diff = "Hard";

		outFile << left << setw(20) << sessions[i].description
			<< setw(10) << fixed << setprecision(1) << sessions[i].hours
			<< setw(10) << fixed << setprecision(2) << sessions[i].cost
			<< setw(15) << diff << endl; 
	}

	outFile.close();
}

void showMenu() {
	cout << "\nMenu:\n";
	cout << "1. Add embroidery session.\n";
	cout << "2. View Sessions\n";
	cout << "3. Get recommendation\n";
	cout << "4. Save report\n";
	cout << "5. Quit\n";
	cout << "Enter your choice: ";
}