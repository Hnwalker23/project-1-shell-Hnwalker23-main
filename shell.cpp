/**
 * Simple Shell Implementation
 * Operating Systems - Project 1
 * 
 * Student Name: [YOUR NAME]
 * Student ID: [YOUR ID]
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

//test comment

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
 * AI ASSISTANCE: [Document any AI help here]
 */
vector<string> parseCommand(const string& input) {
    vector<string> tokens;
    
    // TODO: Implement parsing
    // YOUR CODE HERE
    
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
 * AI ASSISTANCE: [Document any AI help here]
 */
void executeBuiltIn(const vector<string>& args) {
    // TODO: Implement built-in commands
    // YOUR CODE HERE
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
 * AI ASSISTANCE: [Document any AI help here]
 */
void executeCommand(const vector<string>& args) {
    // TODO: Implement fork/exec
    // YOUR CODE HERE
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
