[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/9_QoGimx)
# Project 1: Simple Shell

**Operating Systems**  
**Checkpoint Due:** Friday, January 30  
**Final Due:** Friday, February 6  

---

## Overview

In this project, you will implement a simple Unix shell in C++. A shell is a command-line interface that allows users to interact with the operating system by executing commands. You'll learn about process management, file descriptors, pipes, and signals.

## Learning Objectives

- Understand process creation with `fork()` and `exec()`
- Implement inter-process communication using pipes
- Work with file descriptors and I/O redirection
- Handle signals appropriately
- Parse and interpret command-line input

## Project Structure

```
shell-project/
├── shell.cpp           # Main implementation file (YOUR CODE HERE)
├── Makefile           # Build configuration (PROVIDED)
├── test_phase1.sh     # Checkpoint tests
├── test_phase2.sh     # Final tests
├── README.md          # This file
└── AI_USAGE.md        # Document AI assistance here
```

## Getting Started

### Building the Shell

```bash
make              # Compile the shell
./myshell         # Run your shell
make clean        # Remove compiled files
```

### Running Tests

```bash
# Make test scripts executable (first time only)
chmod +x test_phase1.sh test_phase2.sh

# Run checkpoint tests (Phase 1)
./test_phase1.sh

# Run all tests (Phase 2)
./test_phase2.sh
```

**IMPORTANT:** Your code must pass tests on GitHub Classroom, not just on your local machine

## Grading Breakdown

### Phase 1 - Checkpoint (25%)
**Due: January 30**  
**Branch: `checkpoint`**

- Basic command execution (15%)
- Built-in commands: `exit`, `cd` (5%)
- Error handling: command not found (5%)

### Phase 2 - Final (75%)
**Due: February 6**  
**Branch: `main`**

All Phase 1 features plus:
- Pipes (`|`) - 25%
- I/O Redirection (`>`, `<`, `>>`) - 25%
- Background processes (`&`) - 15%
- Signal handling (Ctrl+C) - 10%

### Bonus Features (Up to +20%)
- Command history
- Tab completion
- Job control (`fg`, `bg`, `jobs`)
- Multiple pipes in one command
- Combined features (e.g., `cmd1 | cmd2 > file`)

## Required Features

### Phase 1: Basic Shell

#### 1. Command Execution (15%)
Your shell must:
- Display a prompt: `myshell> `
- Read user input
- Parse the input into command and arguments
- Execute external commands using `fork()` and `execvp()`
- Wait for the command to complete

**Example:**
```
myshell> ls -l
total 32
-rw-r--r-- 1 user user  1234 Jan 15 10:30 shell.cpp
-rw-r--r-- 1 user user   456 Jan 15 10:30 Makefile
myshell> pwd
/home/user/shell-project
myshell> 
```

#### 2. Built-in Commands (5%)

**exit:** Terminate the shell
```
myshell> exit
$
```

**cd:** Change directory
```
myshell> cd /tmp
myshell> pwd
/tmp
myshell> cd
myshell> pwd
/home/user
```

#### 3. Error Handling (5%)

Handle command not found:
```
myshell> invalidcommand
Command not found: invalidcommand
myshell> 
```

### Phase 2: Advanced Features

#### 4. Pipes (25%)

Connect output of one command to input of another:
```
myshell> ls | grep cpp
shell.cpp
myshell> cat file.txt | wc -l
42
```

**System calls needed:** `pipe()`, `fork()`, `dup2()`, `close()`

#### 5. I/O Redirection (25%)

**Output redirection (`>`):** Write output to file
```
myshell> ls > output.txt
myshell> cat output.txt
Makefile
shell.cpp
```

**Append redirection (`>>`):** Append output to file
```
myshell> echo line1 > file.txt
myshell> echo line2 >> file.txt
myshell> cat file.txt
line1
line2
```

**Input redirection (`<`):** Read input from file
```
myshell> cat < input.txt
contents of input.txt
```

**System calls needed:** `open()`, `dup2()`, `close()`

#### 6. Background Processes (15%)

Run commands in the background:
```
myshell> sleep 10 &
[1] 12345
myshell> # shell returns immediately, can run more commands
```

Shell should **not wait** for background processes to finish.

#### 7. Signal Handling (10%)

Pressing Ctrl+C should:
- ✗ NOT kill the shell
- ✓ Kill the current foreground child process (if any)
- ✓ Return to the prompt

**System calls needed:** `signal()` or `sigaction()`

## Implementation Hints

### Parsing Commands

**Goal:** Split input string into individual tokens (words)

**Approach 1 - Using stringstream:**
- Create a `stringstream` from the input string
- The `>>` operator automatically splits by whitespace
- Loop and extract each token into your vector

**Approach 2 - Manual parsing:**
- Use `string::find()` to locate spaces
- Use `string::substr()` to extract each word
- Handle multiple consecutive spaces

**Tip:** The first approach is simpler! Check the `<sstream>` header documentation.

### Fork/Exec Pattern

**The Process:**
1. **Fork** - Creates a copy of your process
   - Returns 0 in the child process
   - Returns child's PID in the parent process
   - Returns -1 if it fails

2. **Exec (in child only)** - Replaces the child's program with the command
   - Use `execvp()` - it searches the PATH for you
   - Needs a null-terminated array of char pointers
   - Only returns if there's an error!

3. **Wait (in parent only)** - Parent waits for child to finish
   - Use `waitpid()` with the child's PID
   - Prevents zombie processes

**Key Insight:** After fork(), you have TWO processes running. Use an if-statement to make them do different things.

**Converting vector to char\*\*:**
You'll need to create a char\* array where:
- Each element points to a token's C-string (use `.c_str()`)
- The last element must be `nullptr`
- Don't forget to clean up memory!

### Converting vector<string> to char**

**Why needed:** `execvp()` requires a C-style array, not a C++ vector.

**What you need:**
- Dynamically allocate an array of char pointers
- Size should be `args.size() + 1` (the +1 is for the null terminator!)
- Each element points to a string's C-string data
- Last element must be `nullptr`

**Steps:**
1. Create `char** argv` with correct size
2. Loop through vector and assign each `.c_str()` to argv
3. Set the last element to `nullptr`
4. After use, delete the array (but NOT the strings - they're owned by the vector)

**Memory management:** You allocated the array with `new`, so remember to `delete[]` it

### Creating Pipes

**Concept:** A pipe is a one-way communication channel between processes.

**Setup:**
- Call `pipe()` with an array of 2 integers: `int pipefd[2]`
- If successful, `pipefd[0]` is the read end, `pipefd[1]` is the write end
- Check if `pipe()` returns -1 (error)

**Visual:**
```
Process 1 writes → pipefd[1] -----> pipefd[0] → Process 2 reads
                    (write end)      (read end)
```

**Critical Rules:**
1. Close the end you're NOT using in each process
2. The parent must close BOTH ends after forking children
3. Use `dup2()` to redirect stdin/stdout to the pipe
4. Close the original pipe file descriptors after `dup2()`

**Why close unused ends?** If you don't, the reading process will never see EOF and will hang forever

## AI Usage Policy

You **MAY** use AI tools (ChatGPT, Claude, Copilot, etc.) to help with this project, but:

1. **You must cite ALL AI assistance** in:
   - Comments next to the code: `// AI: Helped with pipe creation`
   - `AI_USAGE.md` file

2. **You must understand the code** - Tech interviews will test your understanding

3. **Don't just copy/paste** - You need to explain it in the interview

See `AI_USAGE.md` for the citation template.

## Submission Instructions

### For Checkpoint (Jan 30)
1. Complete Phase 1 features
2. Commit and push to `checkpoint` branch:
   ```bash
   git add .
   git commit -m "Checkpoint submission"
   git push origin checkpoint
   ```
3. Verify tests pass on GitHub Classroom

### For Final (Feb 6)
1. Complete all Phase 2 features
2. Commit and push to `main` branch:
   ```bash
   git add .
   git commit -m "Final submission"
   git push origin main
   ```
3. Verify tests pass on GitHub Classroom

## Common Issues

### "Command not found" for valid commands
- Check that you're using `execvp()`, not `execv()`
- Ensure argv is null-terminated
- Verify you're passing correct arguments

### Pipe not working
- Make sure to close unused pipe ends
- Use `dup2()` to redirect stdin/stdout
- Fork for each command in the pipe

### Background process waits anyway
- Don't call `waitpid()` for background processes
- Check for `&` at the end of command

### Shell exits on Ctrl+C
- Install signal handler before main loop
- Ignore SIGINT in parent process

## Grading Criteria

- **Correctness:** Does it pass the tests?
- **Code Quality:** Clean, readable, well-commented
- **Error Handling:** Proper error messages and recovery
- **Tech Interview:** Can you explain your code?

---
