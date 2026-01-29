#!/bin/bash
# Phase 1 Checkpoint Tests (25% of grade)
# Tests basic command execution and built-in commands

set -e  # Exit on error

SHELL_EXEC="./myshell"
PASSED=0
FAILED=0

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "================================"
echo "PHASE 1 CHECKPOINT TESTS"
echo "Testing: Basic Commands & Built-ins"
echo "================================"
echo ""

# Helper function to run test
run_test() {
    local test_name=$1
    local input=$2
    local expected_pattern=$3
    local test_num=$((PASSED + FAILED + 1))
    
    echo -n "Test $test_num: $test_name... "
    
    # Run the shell with input and capture output
    output=$(echo -e "$input" | timeout 2 $SHELL_EXEC 2>&1 || true)
    
    if echo "$output" | grep -q "$expected_pattern"; then
        echo -e "${GREEN}PASSED${NC}"
        ((PASSED++))
        return 0
    else
        echo -e "${RED}FAILED${NC}"
        echo "  Expected pattern: $expected_pattern"
        echo "  Got output: $output"
        ((FAILED++))
        return 1
    fi
}

# Helper function to check if command produces output
run_output_test() {
    local test_name=$1
    local input=$2
    local test_num=$((PASSED + FAILED + 1))
    
    echo -n "Test $test_num: $test_name... "
    
    # Run the shell with input and check if it produces output
    output=$(echo -e "$input" | timeout 2 $SHELL_EXEC 2>&1 || true)
    
    if [ -n "$output" ]; then
        echo -e "${GREEN}PASSED${NC}"
        ((PASSED++))
        return 0
    else
        echo -e "${RED}FAILED${NC}"
        echo "  Expected: Some output"
        echo "  Got: Empty output"
        ((FAILED++))
        return 1
    fi
}

# Test 1: Shell starts and shows prompt
echo "--- Basic Functionality ---"
run_test "Shell shows prompt" "exit\n" "myshell>"

# Test 2: ls command
run_output_test "Execute ls command" "ls\nexit\n"

# Test 3: pwd command
run_output_test "Execute pwd command" "pwd\nexit\n"

# Test 4: echo command
run_test "Execute echo command" "echo hello world\nexit\n" "hello world"

# Test 5: ls with arguments
run_output_test "Execute ls with flags" "ls -l\nexit\n"

echo ""
echo "--- Built-in Commands ---"

# Test 6: exit command works
echo -n "Test 6: exit command terminates shell... "
output=$(echo "exit" | timeout 2 $SHELL_EXEC 2>&1 || true)
if [ $? -eq 0 ]; then
    echo -e "${GREEN}PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}FAILED${NC}"
    ((FAILED++))
fi

# Test 7: cd command (test by running pwd after cd)
echo -n "Test 7: cd command changes directory... "
output=$(echo -e "cd /tmp\npwd\nexit" | timeout 2 $SHELL_EXEC 2>&1)
if echo "$output" | grep -q "/tmp"; then
    echo -e "${GREEN}PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}FAILED${NC}"
    echo "  Expected: /tmp in output"
    echo "  Got: $output"
    ((FAILED++))
fi

echo ""
echo "--- Error Handling ---"

# Test 8: Command not found error
run_test "Handle command not found" "nonexistentcommand123\nexit\n" "not found"

echo ""
echo "================================"
echo "PHASE 1 RESULTS"
echo "================================"
echo -e "Passed: ${GREEN}$PASSED${NC}"
echo -e "Failed: ${RED}$FAILED${NC}"
echo "Total:  $((PASSED + FAILED))"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}✓ All Phase 1 tests passed!${NC}"
    exit 0
else
    echo -e "\n${RED}✗ Some tests failed${NC}"
    exit 1
fi
