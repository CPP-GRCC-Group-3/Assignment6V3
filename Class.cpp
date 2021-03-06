/*
* Amanuel Fufa, Caleb Brace, Caleb McGraw
* CIS 227
* Week 7
*
* 1/27/21 - 3/8/21
*/

#include <map>
#include "Header.h"
#include <string>
#include <algorithm>
#include <iostream>



using namespace std;

int winCount = 0;
int lossCount = 0;

vector<char> guessBank;

string hidden;

//display word in given position in the array
void GameHandler::display(bool typeCheck, int& n) {

	//this runs if reading from array
	if (typeCheck) {
		cout << endl << "Position " << n << ": " << wordArray[n - 1] << endl << endl;
	}

	//this runs if reading from vector (made from file)
	else {
		cout << endl << "Position " << n << ": " << wordsVector[n - 1] << endl << endl;
	}

}

void GameHandler::displayHint() {
	cout << hidden.at(0) << endl;
	addToScore(hintMinusPoints);

}

//get word, and "hide it" from displaying
void GameHandler::hide(bool typeCheck, int& n) {
	//this runs if reading from array

	if (typeCheck) {

		hidden = wordArray[n - 1];
		int x = hidden.length();

		string hidi(x - winCount, '-');

		cout << hidi << endl;
	}

	//this runs if reading from vector (made from file)
	else {
		hidden = wordsVector[n - 1];
		int x = hidden.length();

		string hidi(x - winCount, '-');

		cout << hidi << endl;
	}
	
	// Stores each char of string in vector for later manipulation
	for (int i = 0; i < hidden.length(); i++) {
		guessBank.push_back(hidden[i]);
	}
}


//validate menu has correct input numbers
bool GameHandler::ValidateMenuScope(int& menu) {

	if (menu >= 1 && menu <= 5) {
		return true;
	}

	else {
		return false;
	}
}

bool GameHandler::checkGuess(string& guess) {

	// converts the guess to lower case to ensure a hit regardless of capitilization
	transform(guess.begin(), guess.end(), guess.begin(), ::tolower);

	//if guess is correct
	if (count(guessBank.begin(), guessBank.end(), guess)) {

		winCount = winCount + 1;
		addToScore(correctGuessPoints);

		cout << "You have guessed " << winCount << " letters correct!" << endl;

		// finds the guessed letter within the guessBank array and erases it

		vector<char>::iterator gFinder = find(guessBank.begin(), guessBank.end(), guess);
		if (gFinder != guessBank.end()) {
			guessBank.erase(gFinder);
		}

		return true;
	}

	//if guess is incorrect
	else {

		lossCount = lossCount + 1;
		addToScore(incorrectGuessPoints);

		cout << "You have guessed " << lossCount << " times incorrectly! (out of 5)" << endl;

		return false;
	}
}

bool GameHandler::gameState() {

	//if game hasnt been lost or won yet
	if (lossCount <= 4 && winCount < hidden.length()) {

		return true;
	}

	//reset when game is finished
	else {

		if (lossCount == 5) {
			cout << "\nYou Lose\n";
		}
		else {
			addToScore(finishWordScore);
			cout << "\nYou Win!\n";
		}

		//TODO Write score to file
		writeScoreToFile(initials, currentScore);

		setCurrentScore(0);
		guessBank.clear();

		winCount = 0;
		lossCount = 0;

		return false;
	}
}

//validate array choice is within scope
bool GameHandler::ValidateArrayScope(int& arrayPosition) {

	//ensure a word is chosen, not too low or high
	if (arrayPosition >= 1 && arrayPosition <= 12) {
		return true;
	}

	else {
		return false;
	}
}

void GameHandler::CreateVector() {
	string line;
	ifstream myFile;

	myFile.open(wordsFile);

	//Get all lines in file and add them to a vector
	if (myFile.is_open()) {
		while (getline(myFile, line)) {
			wordsVector.push_back(line);
		}

		myFile.close();
	}
}

void GameHandler::addToScore(int& score) {
	currentScore += score;
}

void GameHandler::setCurrentScore(int score) {
	currentScore = score;
}

int GameHandler::getCurrentScore() {
	return currentScore;
}

void GameHandler::writeScoreToFile(string& name, int& score) {
	ofstream file(scoreFile, ios::app);

	transform(name.begin(), name.end(), name.begin(), ::toupper);

	file << name << " " << score << endl;

	file.close();
}

void GameHandler::readScoreFile() {
	ifstream file;
	string line;
	string name;
	int score;

	file.open(scoreFile);

	while (getline(file, line)) {

		name = line.substr(0, 3);
		score = stoi(line.substr(3));

		highScores[score] = name;
	}

	for (map<int, string>::iterator m = highScores.begin(); m != highScores.end(); m++) {
		cout << m->second << " " << m->first << endl;
	}

	file.close();
}

void GameHandler::logData(string& init, int& menuOption) {
	ofstream logFile;

	time_t now = time(0);

	tm* ltm = localtime(&now);

	string fileName = "LogFile.log";

	logFile.open(fileName, ios::out | ios::app);

	if (logFile.is_open()) {
		logFile << "User: " << init << " " << 1 + ltm->tm_mon << "/" << ltm->tm_mday << "/" << 1900 + ltm->tm_year << " @ " << ltm->tm_hour << ":" << ltm->tm_min << " interacted with menu option  " << menuOption << "\n";
	}


	logFile.close();
}

void GameHandler::setInitials(string& init) {
	initials = init;
}

string GameHandler::getInitials() {
	return initials;
}



GameHandler::GameHandler() {
	CreateVector();
	setCurrentScore(0);
}

GameHandler::~GameHandler() {

}