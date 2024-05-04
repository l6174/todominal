#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <chrono>
#include <thread>
using namespace std;

// Change this file name if needed
const string userHome = getenv("HOME"); 
const string mainFile = userHome + "/.config/todominal/todos.txt";

// Function to remove a line from a specific line from a file
void removeLine(const string& filename, int lineNumberToRemove) {
    ifstream inputFile(filename);
    vector<string> lines;

    if (inputFile.is_open()) {
        string line;
        int currentLine = 1;
        while (getline(inputFile, line)) {
            if (currentLine != lineNumberToRemove) {
                lines.push_back(line); // Keep the line if it's not the one to remove
            }
            currentLine++;
        }
        inputFile.close();

        // Write the modified content back to the file
        ofstream outputFile(filename);
        if (outputFile.is_open()) {
            for (const string& line : lines) {
                outputFile << line << endl;
            }
            outputFile.close();
            cout << "Line " << lineNumberToRemove << " removed from file: " << filename << endl;
        } else {
            cerr << "Unable to open output file: " << filename << endl;
        }
    } else {
        cerr << "Unable to open input file: " << filename << endl;
    }
}

// Functiom to get a specific line of a file
string getLineContent(const string& filename, int lineNumber) {
    ifstream file(filename);
    string lineContent;
    int currentLine = 1;

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (currentLine == lineNumber) {
                lineContent = line;
                break;
            }
            currentLine++;
        }
        file.close();
    } else {
        cerr << "Unable to open file: " << filename << endl;
    }

    return lineContent;
}

// Function to Replace a line with a given line
void replaceLine(const string& filename, int lineNumber, const string& newLine) {
    ifstream inputFile(filename);
    vector<string> lines;

    if (inputFile.is_open()) {
        string line;
        int currentLine = 1;
        while (getline(inputFile, line)) {
            if (currentLine == lineNumber) {
                lines.push_back(newLine); // Replace the line with newLine
            } else {
                lines.push_back(line); // Keep the original line
            }
            currentLine++;
        }
        inputFile.close();

        // Write the modified content back to the file
        ofstream outputFile(filename);
        if (outputFile.is_open()) {
            for (const string& line : lines) {
                outputFile << line << endl;
            }
            outputFile.close();
        } else {
            cerr << "Unable to open output file\n";
        }
    } else {
        cerr << "Unable to open input file\n";
    }
}

// Function to StrikeThrough given text
string strikethrough(const string& text) 
{
    string result;

    result = "\e[9m" + text + "\e[m"; 
    return result;
}

// Function to remove a todo
void removeTodo() {
    string toRemove;
    cout << "Enter Todo Index to Remove: ";
    cin >> toRemove;
    int toRemoveint = stoi(toRemove);
    removeLine(mainFile, toRemoveint);
}

// Function to mark a todo as done
void markDone() {
    string markInput;
    cout << "Enter Todo Index to Mark as Done: ";
    cin >> markInput;
    int markNum = stoi(markInput);
    replaceLine(mainFile, markNum, strikethrough(getLineContent(mainFile, markNum)));
}

// Function to add a new todo
void addTodo() {
    string newTodo;

    // Get New Todo Input
    cout << "Enter New Todo Name: ";
    cin.ignore(); // Clear any remaining newline characters in the buffer
    getline(cin, newTodo); // Read entire line including spaces

    // Append it to Todo File
    ofstream todoFile(mainFile, ios::app);
    todoFile << newTodo << endl;
    todoFile.close();
}

int main() {
    // Create Todo database file if does not exist
    system("mkdir -p $HOME/.config/todominal");
    system("touch $HOME/.config/todominal/todos.txt");
    
    // Input Variables
    string userInput;
    string otherInput;
    string eraseConfirmation;
    
    // Main Loop
    bool run = true;
    while (run) {
        int taskIndex = 1;
        // Header
        system("clear");
        system("figlet TODOMINAL");
        cout << "ToDo List in your Terminal!" << endl << endl;
        system("echo Hey, $(whoami).");
        
        // Display todos
        ifstream todos(mainFile);
        if (todos.is_open()) {
            string line;
            while (getline(todos, line)) {
                cout << taskIndex << ". " << line << endl;
                taskIndex++;
            }
            todos.close();
        } else {
            cerr << "Unable to open file\n";
            break;
        }
        
        // User input
        cout << "\n(A)dd Todo || (R)emove Todo || (M)ark Done || (O)ptions || (E)xit\n>> ";
        cin >> userInput;

        // Handle user input
        if (userInput == "A" || userInput == "a") {
            addTodo();
        } else if (userInput == "R" || userInput == "r") {
            removeTodo();
        } else if (userInput == "M" || userInput == "m") {
            markDone();
        } else if (userInput == "O" || userInput == "o") {
            cout << "1. Clear All Todo\n(E)xit\n>> ";
            cin >> otherInput;
            if (otherInput == "1")
            {
                cout << "Are you sure? (Y/N): ";
                std::cin >> eraseConfirmation;
                if (eraseConfirmation == "y" || eraseConfirmation == "Y") {
                    ofstream toErase(mainFile, ofstream::out | ofstream::trunc);
                    toErase.close();
                }
                else {};
            }
            else if (otherInput == "E" || otherInput == "e"){} //Exit
            else
            {
                cerr << "Enter a valid response!";
                this_thread::sleep_for(chrono::seconds(1));
            }
        } else if (userInput == "E" || userInput == "e") {
            system("clear");
            exit(0);
        } else {
            cerr << "Enter a valid response!";
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
    return 0;
}