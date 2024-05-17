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

// Define colors for priorities
string RED = "\033[1;31m";
string YELLOW = "\033[1;33m";
string GREEN = "\033[1;32m";
string RESET = "\033[0m";

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

// Function to remove a todo, check for int
void removeTodo() {
    string toRemove;
    cout << "Enter Todo Index to Remove: ";
    std::cin >> toRemove;

    bool isValid = true;
    for (char c : toRemove) {
        if (!isdigit(c)) {
            isValid = false;
            break;
        }
    }

    if (isValid) {
        int toRemoveint = stoi(toRemove);
        removeLine(mainFile, toRemoveint);
    } else {
        cerr << "Invalid input. Please enter a valid integer index." << endl;
    }
}


// Function to mark a todo as done
void markDone() {
    string markInput;
    cout << "Enter Todo Index to Mark as Done: ";
    std::cin >> markInput;
    int markNum = stoi(markInput);
    replaceLine(mainFile, markNum, strikethrough(getLineContent(mainFile, markNum)));
}

// Function to print indexed contents of a file
void printTodo() {
    int i = 1;
    ifstream todos(mainFile);
    if (todos.is_open()) {
        string line;
        while (getline(todos, line)) {
            string color;
            if (line.find("(h)") != string::npos) {
                color = RED;
            } else if (line.find("(m)") != string::npos) {
                color = YELLOW;
            } else if (line.find("(l)") != string::npos) {
                color = GREEN;
            } else {
                color = RESET;
            }
            cout << i << ". " << color << line << RESET << endl;
            i++;
        }
        todos.close();
    } else {
        cerr << "Unable to open file\n";
        exit(1);
    }
}

void printTodoRofi() {
    RED = "<span foreground=\"#f38ba8\">";
    YELLOW = "<span foreground=\"#f9e2af\">";
    GREEN = "<span foreground=\"#a6e3a1\">";
    RESET = "</span>";
    int i = 1;
    ifstream todos(mainFile);
    if (todos.is_open()) {
        string line;
        while (getline(todos, line)) {
            string color;
            if (line.find("(h)") != string::npos) {
                color = RED;
            } else if (line.find("(m)") != string::npos) {
                color = YELLOW;
            } else if (line.find("(l)") != string::npos) {
                color = GREEN;
            } else {
                color = "<span>";
            }
            cout << i << ". " << color << line << RESET << endl;
            i++;
        }
        todos.close();
    } else {
        cerr << "Unable to open file\n";
        exit(1);
    }
}

// Function to add a new todo
void addTodoCli(string toAdded) {
    ofstream todoFile(mainFile, ios::app);
    todoFile << toAdded << endl;
    todoFile.close();
}

void addTodo() {
    string todoName, priority;
    cout << "Enter New Todo Name: ";
    cin.ignore();
    getline(cin, todoName);
    cout << "Enter Priority (High, Medium, Low): ";
    getline(cin, priority);
    if (priority.empty())
    {
        todoName = todoName;
    }
    else
    {
        char priorityChar = tolower(priority[0]);
        todoName = todoName + " (" + priorityChar + ")";
    }
    ofstream todoFile(mainFile, ios::app);
    todoFile << todoName << endl;
    todoFile.close();
}

int main(int argc, char* argv[]) {
    // Create Todo database file if does not exist
    system("mkdir -p $HOME/.config/todominal");
    system("touch $HOME/.config/todominal/todos.txt");
    int argNum = argc - 1; // To avoid confusion

    // If 1 or more arguments are passed (CLI Mode)
    if (argNum > 0)
    {
        // Store arguments in variables
        string firstArg;
        string secondArg;
        // Usage (Help Box)
        string helpBox = "Usage:\n   todominal add {Task Name (string)}\n   todominal remove {Task Index (int)}\n   todominal done {Task Index (int)}\n   todominal clearall\n   todominal list\n   todominal help\nExample:\n   todominal remove 2\n   todominal add \"Hello World!\"\n   todominal done 3\n";
        if (argNum == 1)
        {
            firstArg = argv[1];
            if (firstArg == "clearall")
            {
                ofstream toErase(mainFile, ofstream::out | ofstream::trunc);
                toErase.close();
            }
            else if (firstArg == "help")
            {
                cout << helpBox;
            }
            else if (firstArg == "list")
            {
                printTodo();
            }
            else if (firstArg == "list-rofi")
            {
                printTodoRofi();
            }
            else
            {
                cout << helpBox;
            }
        }
        else if (argNum == 2)
        {
            firstArg = argv[1];
            secondArg = argv[2];
            if (firstArg == "add")
            {
                addTodoCli(secondArg);
            }
            else if (firstArg == "remove")
            {
                int lineIndex = stoi(argv[2]);
                string Task = getLineContent(mainFile, lineIndex);
                removeLine(mainFile, lineIndex); 
            }
            else if (firstArg == "done")
            {
                int lineIndex = stoi(argv[2]);
                string Task = getLineContent(mainFile, lineIndex); 
                replaceLine(mainFile, lineIndex, strikethrough(getLineContent(mainFile, lineIndex)));
            }
            else
            {
                cout << helpBox;
            }
            
        }
        else
        {
            cout << "Only 2 arguments are allowed!" << endl;
        }
    }
    // If no arguments are passed (Interactive CLI Mode)
    else
    {
        // Input Variables
        string userInput;
        string otherInput;
        string eraseConfirmation;
        
        // Main Loop
        bool run = true;
        while (run) {
            // Header
            system("clear");
            system("figlet TODOMINAL");
            cout << "ToDo List in your Terminal!" << endl << endl;
            system("echo Hey, $(whoami).");
            
            printTodo();
            
            // User input
            cout << "\n(A)dd Todo || (R)emove Todo || (M)ark Done || (O)ptions || (E)xit\n>> ";
            std::cin >> userInput;

            // Handle user input
            if (userInput == "A" || userInput == "a") {
                addTodo();
            } else if (userInput == "R" || userInput == "r") {
                removeTodo();
            } else if (userInput == "M" || userInput == "m") {
                markDone();
            } else if (userInput == "O" || userInput == "o") {
                cout << "1. Clear All Todo\n(E)xit\n>> ";
                std::cin >> otherInput;
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
    }
    return 0;
}
