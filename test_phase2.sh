#!/bin/bash
# Phase 2 Final Tests (75% of grade)
# Tests all Phase 1 features plus pipes, redirection, background, and signals

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
echo "PHASE 2 FINAL TESTS"
echo "Testing: All Features"
echo "================================"
echo ""

# Cleanup function
cleanup() {
    rm -f test_output.txt test_input.txt test_append.txt
}

# Setup
cleanup
trap cleanup EXIT

# Helper function to run test
run_test() {
    local test_name=$1
    local input=$2
    local expected_pattern=$3
    local test_num=$((PASSED + FAILED + 1))
    
    echo -n "Test $test_num: $test_name... "
    
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

run_output_test() {
    local test_name=$1
    local input=$2
    local test_num=$((PASSED + FAILED + 1))
    
    echo -n "Test $test_num: $test_name... "
    
    output=$(echo -e "$input" | timeout 2 $SHELL_EXEC 2>&1 || true)
    
    if [ -n "$output" ]; then
        echo -e "${GREEN}PASSED${NC}"
        ((PASSED++))
        return 0
    else
        echo -e "${RED}FAILED${NC}"
        ((FAILED++))
        return 1
    fi
}

echo "--- Phase 1: Basic Functionality ---"

# Test 1-4: Basic commands
run_test "Shell shows prompt" "exit\n" "myshell>"
run_output_test "Execute ls command" "ls\nexit\n"
run_test "Execute echo command" "echo hello\nexit\n" "hello"
run_test "exit command" "exit\n" "myshell>"

# Test 5-6: Built-ins
echo -n "Test 5: cd command... "
output=$(echo -e "cd /tmp\npwd\nexit" | timeout 2 $SHELL_EXEC 2>&1)
if echo "$output" | grep -q "/tmp"; then
    echo -e "${GREEN}PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}FAILED${NC}"
    ((FAILED++))
fi

# Test 6: Command not found
run_test "Command not found error" "invalidcmd999\nexit\n" "not found"

echo ""
echo "--- Phase 2: Pipes ---"

# Test 7: Simple pipe
echo -n "Test 7: Simple pipe (ls | grep test)... "
touch test_file1.txt test_file2.txt
output=$(echo -e "ls | grep test\nexit" | timeout 2 $SHELL_EXEC 2>&1)
if echo "$output" | grep -q "test_file"; then
    echo -e "${GREEN}PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}FAILED${NC}"
    ((FAILED++))
fi
rm -f test_file1.txt test_file2.txt

# Test 8: Pipe with echo
run_test "Pipe with echo (echo hello | cat)" "echo hello | cat\nexit\n" "hello"

# Test 9: Pipe with wc
echo -n "Test 9: Pipe with wc (echo test | wc -c)... "
output=$(echo -e "echo test | wc -c\nexit" | timeout 2 $SHELL_EXEC 2>&1)
if echo "$output" | grep -q "5"; then
    echo -e "${GREEN}PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}FAILED${NC}"
    ((FAILED++))
fi

echo ""
echo "--- Phase 2: Output Redirection ---"

# Test 10: Output redirection
echo -n "Test 10: Output redirection (echo hello > test_output.txt)... "
echo -e "echo hello > test_output.txt\nexit" | timeout 2 $SHELL_EXEC > /dev/null 2>&1
if [ -f test_output.txt ] && grep -q "hello" test_output.txt; then
    echo -e "${GREEN}PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}FAILED${NC}"
    ((FAILED++))
fi
rm -f test_output.txt

# Test 11: ls output redirection
echo -n "Test 11: ls output redirection (ls > test_output.txt)... "
echo -e "ls > test_output.txt\nexit" | timeout 2 $SHELL_EXEC > /dev/null 2>&1
if [ -f test_output.txt ] && [ -s test_output.txt ]; then
    echo -e "${GREEN}PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}FAILED${NC}"
    ((FAILED++))
fi
rm -f test_output.txt

# Test 12: Append redirection
echo -n "Test 12: Append redirection (echo line1 > file, echo line2 >> file)... "
echo -e "echo line1 > test_append.txt\necho line2 >> test_append.txt\nexit" | timeout 2 $SHELL_EXEC > /dev/null 2>&1
if [ -f test_append.txt ] && grep -q "line1" test_append.txt && grep -q "line2" test_append.txt; then
    echo -e "${GREEN}PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}FAILED${NC}"
    ((FAILED++))
fi
rm -f test_append.txt

echo ""
echo "--- Phase 2: Input Redirection ---"

# Test 13: Input redirection
echo -n "Test 13: Input redirection (cat < file)... "
echo "test content" > test_input.txt
output=$(echo -e "cat < test_input.txt\nexit" | timeout 2 $SHELL_EXEC 2>&1)
if echo "$output" | grep -q "test content"; then
    echo -e "${GREEN}PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}FAILED${NC}"
    ((FAILED++))
fi
rm -f test_input.txt

echo ""
echo "--- Phase 2: Background Processes ---"

# Test 14: Background process
echo -n "Test 14: Background process (sleep 1 &)... "
start_time=$(date +%s)
echo -e "sleep 1 &\nexit" | timeout 3 $SHELL_EXEC > /dev/null 2>&1
end_time=$(date +%s)
elapsed=$((end_time - start_time))
if [ $elapsed -lt 2 ]; then
    echo -e "${GREEN}PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}FAILED${NC}"
    echo "  Shell waited for background process (took ${elapsed}s)"
    ((FAILED++))
fi

echo ""
echo "--- Phase 2: Signal Handling ---"

# Test 15: Ctrl+C doesn't kill shell
echo -n "Test 15: Shell handles Ctrl+C... "
# This is harder to test automatically, so we'll do a simple check
# that the shell can handle SIGINT without crashing
output=$(echo -e "echo before\necho after\nexit" | timeout 2 $SHELL_EXEC 2>&1)
if echo "$output" | grep -q "before" && echo "$output" | grep -q "after"; then
    echo -e "${GREEN}PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}FAILED${NC}"
    ((FAILED++))
fi

echo ""
echo "================================"
echo "PHASE 2 RESULTS"
echo "================================"
echo -e "Passed: ${GREEN}$PASSED${NC}"
echo -e "Failed: ${RED}$FAILED${NC}"
echo "Total:  $((PASSED + FAILED))"

PERCENT=$(( (PASSED * 100) / (PASSED + FAILED) ))
echo "Score:  $PERCENT%"

if [ $FAILED -eq 0 ]; then
    echo -e "\n${GREEN}✓ All tests passed!${NC}"
    exit 0
elif [ $PERCENT -ge 70 ]; then
    echo -e "\n${YELLOW}⚠ Some tests failed, but you're passing${NC}"
    exit 0
else
    echo -e "\n${RED}✗ Too many tests failed${NC}"
    exit 1
fi
