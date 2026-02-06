/**
 * Simple Shell Implementation
 * Operating Systems - Project 1
 * 
 * Student Name: [Hali Walker]
 * Student ID: [950554578]
 */

 //testing commits - 2/5

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>



using namespace std;

// Function declarations
vector<string> parseCommand(const string& input);
bool isBuiltIn(const string& cmd);
void executeBuiltIn(const vector<string>& args);
void executeCommand(const vector<string>& args);
bool hasPipe(const vector<string>& args);
bool hasRedirection(const vector<string>& args);
bool hasBackground(const vector<string>& args);
void executePipedCommands(const vector<string>& args);
void executeWithRedirection(const vector<string>& args);
void setupSignalHandlers();

int main() {
    string input;
    
    // Setup signal handlers (for Phase 2)
    setupSignalHandlers();
    
    while (true) {
        cout << "myshell> ";
        cout.flush();
        
        if (!getline(cin, input)) {
            // Handle Ctrl+D (EOF)
            break;
        }
        
        if (input.empty()) {
            continue;
        }
        
        vector<string> tokens = parseCommand(input);
        
        if (tokens.empty()) {
            continue;
        }
        
        // Check for built-in commands
        if (isBuiltIn(tokens[0])) {
            executeBuiltIn(tokens);
        } 
        // Phase 2: Check for pipes
        else if (hasPipe(tokens)) {
            executePipedCommands(tokens);
        }
        // Phase 2: Check for redirection or background
        else if (hasRedirection(tokens) || hasBackground(tokens)) {
            executeWithRedirection(tokens);
        }
        // Phase 1: Basic command execution
        else {
            executeCommand(tokens);
        }
    }
    
    cout << endl;
    return 0;
}

/**
 * PHASE 1 - CHECKPOINT REQUIRED
 * Parse input string into vector of tokens (space-separated)
 * Example: "ls -la /tmp" -> ["ls", "-la", "/tmp"]
 * 
 * HINTS:
 * - Use stringstream to split by spaces
 * - Or use string::find() and string::substr()
 * - Handle multiple spaces between tokens
 * 
 * AI ASSISTANCE: Used ChatGPT to further understan background information on stringstream,
 * including its purpose, typical usage, and why it is suitable for parsing 
 * space-separated tokens from a string. Applied this knowledge to implement 
 * command parsing in this function.
 */
vector<string> parseCommand(const string& input) {
    vector<string> tokens;
    
    // TODO: Implement parsing
    // YOUR CODE HERE

    stringstream ss(input); // Create string stream from input
    string token; 

    while (ss >>  token) { // Extract tokens separated by whitespace
        tokens.push_back(token);
    }

    return tokens;
}

/**
 * PHASE 1 - CHECKPOINT REQUIRED
 * Check if command is a built-in (exit, cd)
 * 
 * AI ASSISTANCE: [Document any AI help here]
 */
bool isBuiltIn(const string& cmd) {
    // TODO: Implement built-in check
    // YOUR CODE HERE
    if  (cmd == "exit" || cmd == "cd") {
         return true;
    }
    return false;
}

/**
 * PHASE 1 - CHECKPOINT REQUIRED
 * Handle built-in commands:
 * - exit: terminate the shell
 * - cd: change directory (use chdir() system call)
 * 
 * HINTS:
 * - For cd: handle "cd" alone (go to HOME) and "cd <directory>"
 * - Use chdir() and check return value for errors
 * - For exit: just call exit(0)
 * 
 * AI ASSISTANCE: Used AI to better understand the background and purpose of the chdir() system call 
 *   (how it changes the process's directory and what errors can occur)
 * - Received debugging advice on using getenv() implementation for HOME directory resolution,
 */
void executeBuiltIn(const vector<string>& args) {
    // TODO: Implement built-in commands
    // YOUR CODE HERE

    string command = args[0]; //Separate command from the path
    if (command == "exit") {
        exit(0);
    } else if (command == "cd") { //chnge directory
        const char* path;
        if (args.size() > 1) { //if path is provided
             path = args[1].c_str(); //convert to C-style string
        } else { //no path provided, go to HOME
           
            path = getenv("HOME"); 
        }
        if (chdir(path) !=  0) { //change directory and check for errors
             perror("cd failed");
        }
    }
}

/**
 * PHASE 1 - CHECKPOINT REQUIRED
 * Execute external command using fork/exec
 * 
 * HINTS:
 * - fork() creates a child process
 * - In child: use execvp() to run the command
 * - In parent: use waitpid() to wait for child
 * - Convert vector<string> to char** for execvp
 * - Handle "command not found" error
 * 
 * SYSTEM CALLS NEEDED:
 * - fork()
 * - execvp()
 * - waitpid()
 * 
 * AI ASSISTANCE: ChatGPT provided background on fork, execvp, and waitpid
 * - Debugging issues:
 *     * Initially, the shell returned immediately to the prompt ('myshell>') without actually running commands
 *     * Passing C++ string objects directly to execvp caused errors and I didn’t understand why and learned it needed to convert to a C-style strings using c_str()
 *     * Child processes weren’t being created until the argument array ended with a nullptr
 */
void executeCommand(const vector<string>& args) {
    // TODO: Implement fork/exec
    // YOUR CODE HERE

    pid_t pid = fork();
    if  (pid < 0) {
        perror("Fork failed");
         return;
    }
     if (pid == 0) {
         // Allocate array
        char** argv = new char*[args.size() + 1]; // +1 for nullptr termination

        // Convert vector<string> to char* array
        for (size_t i = 0; i < args.size(); i++) {
            argv[i] = const_cast<char*>(args[i].c_str());
        }
        argv[args.size()] = nullptr; // Must end with nullptr

        // Execute the command
        execvp(argv[0], argv);

        // If execvp returns, there was an error
        perror("Command not found");
        exit(1); // terminate the  child
    } else {
        // Parent process - wait for child to finish
        int status;
        waitpid(pid, &status, 0); 
    }

}

/**
 * PHASE 2 - FINAL REQUIRED
 * Check if command contains a pipe (|)
 * 
 * AI ASSISTANCE: [Document any AI help here]
 */
bool hasPipe(const vector<string>& args) {
    // TODO: Check for pipe symbol
    // YOUR CODE HERE
    
    return false;
}

/**
 * PHASE 2 - FINAL REQUIRED
 * Check if command contains redirection (>, <, >>)
 * 
 * AI ASSISTANCE: [Document any AI help here]
 */
bool hasRedirection(const vector<string>& args) {
    // TODO: Check for redirection symbols
    // YOUR CODE HERE
    
    return false;
}

/**
 * PHASE 2 - FINAL REQUIRED
 * Check if command should run in background (&)
 * 
 * AI ASSISTANCE: [Document any AI help here]
 */
bool hasBackground(const vector<string>& args) {
    // TODO: Check for & symbol
    // YOUR CODE HERE
    
    return false;
}

/**
 * PHASE 2 - FINAL REQUIRED
 * Execute commands with pipes
 * Example: "ls | grep txt" -> run ls, pipe output to grep
 * 
 * HINTS:
 * - Split args at pipe symbol
 * - Create pipe with pipe() system call
 * - Fork for each command
 * - Use dup2() to redirect stdin/stdout
 * - Close unused pipe ends
 * 
 * SYSTEM CALLS NEEDED:
 * - pipe()
 * - fork()
 * - dup2()
 * - close()
 * - execvp()
 * - waitpid()
 * 
 * AI ASSISTANCE: [Document any AI help here]
 */
void executePipedCommands(const vector<string>& args) {
    // TODO: Implement piping
    // YOUR CODE HERE
}

/**
 * PHASE 2 - FINAL REQUIRED
 * Execute command with redirection and/or background
 * 
 * Handle:
 * - Input redirection: cmd < file
 * - Output redirection: cmd > file
 * - Append redirection: cmd >> file
 * - Background execution: cmd &
 * 
 * HINTS:
 * - Use open() with appropriate flags
 * - Use dup2() to redirect file descriptors
 * - For background: don't call waitpid() immediately
 * 
 * SYSTEM CALLS NEEDED:
 * - open()
 * - dup2()
 * - close()
 * - fork()
 * - execvp()
 * - waitpid() (only if not background)
 * 
 * AI ASSISTANCE: [Document any AI help here]
 */
void executeWithRedirection(const vector<string>& args) {
    // TODO: Implement redirection and background execution
    // YOUR CODE HERE
}

/**
 * PHASE 2 - FINAL REQUIRED
 * Setup signal handlers so Ctrl+C doesn't kill the shell
 * 
 * HINTS:
 * - Use signal() or sigaction()
 * - Catch SIGINT (Ctrl+C)
 * - Shell should continue running, but child processes can be interrupted
 * 
 * AI ASSISTANCE: [Document any AI help here]
 */
void setupSignalHandlers() {
    // TODO: Implement signal handling
    // YOUR CODE HERE
}
