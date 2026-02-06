/**
 * Simple Shell Implementation
 * Operating Systems - Project 1
 * 
 * Student Name: [Hali Walker]
 * Student ID: [950554578]
 */

 //uploading current code .

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
    for (string arg : args) {
        if (arg == "|") {
            return true;
        }
    }
    
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
    for (string arg : args) {
        if (arg == ">" || arg == "<" || arg == ">>") {
            return true;
        }
    }
    
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
    for (string arg : args) {
        if (arg == "&") {
            return true;
        }
    }
    
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

    //spliy args at pipe symbol
    vector<string> args_before_pipe;
    vector<string> args_after_pipe;
    bool foundPipe = false;
    for ( string arg : args) {
        if (arg == "|") {
            foundPipe = true;
            continue;
        }
        if (!foundPipe) {
            args_before_pipe.push_back(arg);
        } else {

            args_after_pipe.push_back(arg);
        }
    }
    //testing here 
    
    // Create pipe
    
    int pipefd[2];
    pipe(pipefd);

    // Fork first command
    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Child process for first command
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe write
        close(pipefd[0]); // Close unused read end
        close(pipefd[1]); // Close original write end
        // Execute first command
        char** argv1 = new char*[args_before_pipe.size() + 1];
        for (size_t i = 0; i < args_before_pipe.size(); i++)
            argv1[i] = const_cast<char*>(args_before_pipe[i].c_str());
        argv1[args_before_pipe.size()] = nullptr;
        execvp(argv1[0], argv1);
        perror("First command failed");
        exit(1);
}
    // Fork second command
    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Child process for second command
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe read
        close(pipefd[1]); // Close unused write end
        close(pipefd[0]); // Close original read end
        // Execute second command
        char** argv2 = new char*[args_after_pipe.size() + 1];
        for (size_t i = 0; i < args_after_pipe.size(); i++)
            argv2[i] = const_cast<char*>(args_after_pipe[i].c_str());
        argv2[args_after_pipe.size()] = nullptr;
        execvp(argv2[0], argv2);
        perror("Second command failed");
        exit(1);
    }

    // Parent process
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
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

    vector<string> cmd_args;
    string input_file;
    string output_file;
    bool append = false;
    bool background = false;
    // Parse arguments
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i] == "<" && i + 1 < args.size()) {
            input_file = args[i + 1];
            i++;
        } else if (args[i] == ">" && i + 1 < args.size()) {
            output_file = args[i + 1];
            i++;
        } else if (args[i] == ">>" && i + 1 < args.size()) {
            output_file = args[i + 1];
            append = true;
            i++;
        } else if (args[i] == "&") {
            background = true;
        } else {
            cmd_args.push_back(args[i]);
        }

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
