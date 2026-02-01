#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

// DOCTEST
#ifdef _DEBUG
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#endif

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
	double hours = 0;
	double cost = 0;
	DifficultyLevel difficulty = EASY;
};

// Enum Decision Logic (testing)
string difficultyToString(DifficultyLevel d) {
	switch (d) {
	case EASY: return "Easy";
	case INTERMEDIATE: return "Intermediate";
	case HARD: return "Hard";
	default: return "Unknown";
	}
}

// New Class- Week 2
class EmbroideryTracker {
private: 
	Session sessions[MAX_SESSIONS];
	int numSessions = 0;

	string getNonEmptyString(string prompt) {
		string input;
		do {
			cout << prompt;
			getline(cin >> ws, input);
		} while (input.empty());
		return input;
	}

	double getPositiveDouble(string prompt) {
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

public:
	EmbroideryTracker(Session s[], int numElements) {
		for (int i = 0; i < numElements; ++i) {
			addSession(s[i]);
		}
	}

	bool addSession(Session& s) {
		if (numSessions >= MAX_SESSIONS || s.hours < 0 || s.cost < 0)
			return false;
		sessions[numSessions++] = s;
		return true;
	}

	int getSessionCount() {
		return numSessions;
	}

	double calculateTotalHours() {
		double total = 0.0;
		for (int i = 0; i < numSessions; i++)
			total += sessions[i].hours;

		return total;
	}

	double getAverageHours() {
		if (numSessions == 0) return 0.0;
		return calculateTotalHours() / numSessions;
	}

	void fillSession(Session& s) {
		s.description = getNonEmptyString("Session description : ");
		s.hours = getPositiveDouble("Hours spent: ");
		s.cost = getPositiveDouble("Thread cost: ");
		s.difficulty = getDifficulty();
	}

	DifficultyLevel getHardestDifficulty() {
		DifficultyLevel hardest = EASY;
		for (int i = 0; i < numSessions; i++) {
			if (sessions[i].difficulty > hardest)
				hardest = sessions[i].difficulty;
		}
		return hardest;
	}
	
	void showBanner() {
		cout << "=========================\n";
		cout << "    Embroidery Tracker   \n";
		cout << "=========================\n\n";
	}

	void printSession(Session& s) {

		cout << left << setw(20) << s.description
			<< setw(10) << fixed << setprecision(1) << s.hours
			<< setw(10) << fixed << setprecision(2) << s.cost
			<< setw(15) << difficultyToString(s.difficulty) << endl;
	}

	void saveReport(string& name, double goal) {
		ofstream outFile("report.txt");

		outFile << "Embroidery Report for " << name << endl;
		outFile << "Weekly Hour Goal: " << fixed << setprecision(1) << goal << "\n\n";

		for (int i = 0; i < numSessions; i++) {
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

	// Calculation Logic (testing)
	double calculateTotalCost() {
		double total = 0.0;
		for (int i = 0; i < numSessions; i++)
			total += sessions[i].cost;
		return total;
	}

	
};

// Main
#ifndef _DEBUG
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
			double totalHours = calculateTotalHours(sessions, sessionCount);
			double totalCost = calculateTotalCost(sessions, sessionCount);

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
#endif

// DOCTEST
#ifdef _DEBUG // test mode only

TEST_CASE("Calculations - totals") {
	Session s[2] = {
		{"A", 2.5, 10.0, EASY},
		{"B", 3.5, 20.0, HARD},
	};
	EmbroideryTracker tracker = EmbroideryTracker(s, 2);

	CHECK(tracker.calculateTotalHours() == doctest::Approx(6.0));
	CHECK(tracker.calculateTotalCost() == doctest::Approx(30.0));
}

TEST_CASE("Calculation edge case - empty session") {
	Session s[1];
	EmbroideryTracker tracker = EmbroideryTracker(s, 1);

	CHECK(tracker.calculateTotalHours() == 0.0);
	CHECK(tracker.calculateTotalCost() == 0.0);
}

TEST_CASE("Enum decision logic") {
	CHECK(string(difficultyToString(EASY)) == "Easy");
	CHECK(string(difficultyToString(INTERMEDIATE)) == "Intermediate");
	CHECK(string(difficultyToString(HARD)) == "Hard");
}

TEST_CASE("Struct and array processing") {
	Session s[3] = {
		{"A", 1, 5, EASY},
		{"B", 2, 10, INTERMEDIATE},
		{"C", 3, 15, HARD}
	};
	EmbroideryTracker tracker = EmbroideryTracker(s, 3);

	CHECK(tracker.calculateTotalHours() == 6);
	CHECK(tracker.calculateTotalCost() == 30);
}

TEST_CASE("Class methods - normal and guard cases") {
	Session s[1] = {"Test", 2.0, 5.0, HARD};
	EmbroideryTracker t = EmbroideryTracker(s, 1);
	Session newSession = { "New", 3.0, EASY };
	
	CHECK(t.addSession(newSession) == true); // normal case
	CHECK(t.getSessionCount() == 2);
	CHECK(t.calculateTotalHours() == 5.0);
	CHECK(t.getHardestDifficulty() == HARD);
}
#endif