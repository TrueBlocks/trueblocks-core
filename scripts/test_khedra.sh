#!/bin/bash

# test_khedra.sh - Comprehensive test suite for khedra pause/unpause functionality
# Tests both CLI commands and Control API endpoints with cross-platform compatibility
# Preserves original service state and automatically restores after testing

set -e

# Enable debug mode if requested
if [[ "${1:-}" == "--debug" ]]; then
    set -x
    DEBUG=1
else
    DEBUG=0
fi

# Detect operating system for cross-platform compatibility
case "$(uname -s)" in
    Linux*)     OS_TYPE=Linux;;
    Darwin*)    OS_TYPE=Mac;;
    CYGWIN*)    OS_TYPE=Cygwin;;
    MINGW*)     OS_TYPE=MinGw;;
    *)          OS_TYPE="Unknown";;
esac

# Colors for output (disable on non-interactive terminals)
if [[ -t 1 ]]; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    YELLOW='\033[1;33m'
    BLUE='\033[0;34m'
    NC='\033[0m' # No Color
else
    RED=''
    GREEN=''
    YELLOW=''
    BLUE=''
    NC=''
fi

# Configuration
CONTROL_PORT=8338
CONTROL_URL="http://localhost:${CONTROL_PORT}"
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
KHEDRA_BIN="${KHEDRA_BIN:-$SCRIPT_DIR/../bin/khedra}"

# Test counters
TESTS_RUN=0
TESTS_PASSED=0
TESTS_FAILED=0

# State preservation variables
ORIGINAL_SCRAPER_STATE=""
ORIGINAL_MONITOR_STATE=""
ORIGINAL_STATE_CAPTURED=false

# Function to print colored output
print_status() {
    local status=$1
    local message=$2
    case $status in
        "PASS")
            echo -e "${GREEN}[PASS]${NC} $message"
            if [[ "$status" != "INFO" ]]; then
                TESTS_PASSED=$((TESTS_PASSED + 1))
                TESTS_RUN=$((TESTS_RUN + 1))
            fi
            ;;
        "FAIL")
            echo -e "${RED}[FAIL]${NC} $message"
            if [[ "$status" != "INFO" ]]; then
                TESTS_FAILED=$((TESTS_FAILED + 1))
                TESTS_RUN=$((TESTS_RUN + 1))
            fi
            ;;
        "INFO")
            echo -e "${BLUE}[INFO]${NC} $message"
            ;;
        "WARN")
            echo -e "${YELLOW}[WARN]${NC} $message"
            ;;
    esac
}

# Function to check if khedra is running (cross-platform)
check_khedra_running() {
    print_status "INFO" "Checking if khedra daemon is running..."
    local pgrep_output
    
    # Cross-platform process detection
    if command -v pgrep >/dev/null 2>&1; then
        pgrep_output=$(pgrep -f "khedra.*daemon" 2>/dev/null || true)
    elif [[ "$OS_TYPE" == "Mac" ]] && command -v ps >/dev/null 2>&1; then
        pgrep_output=$(ps aux | grep "khedra.*daemon" | grep -v grep | awk '{print $2}' || true)
    else
        pgrep_output=$(ps -ef | grep "khedra.*daemon" | grep -v grep | awk '{print $2}' || true)
    fi
    
    if [[ -n "$pgrep_output" ]]; then
        print_status "PASS" "khedra daemon is running (PID: $pgrep_output)"
    else
        print_status "FAIL" "khedra daemon is not running. Please start it first."
        exit 1
    fi
}

# Function to check if control service is responding
check_control_service() {
    print_status "INFO" "Checking control service on port $CONTROL_PORT..."
    local response
    response=$(curl -s -m 5 "$CONTROL_URL/" 2>/dev/null)
    local curl_exit=$?
    
    if [[ $curl_exit -eq 0 ]]; then
        print_status "PASS" "Control service is responding on port $CONTROL_PORT"
        return 0
    else
        # Try alternative ports
        for port in 8337 8336 8335; do
            print_status "INFO" "Trying port $port..."
            CONTROL_URL="http://localhost:$port"
            response=$(curl -s -m 5 "$CONTROL_URL/" 2>/dev/null)
            curl_exit=$?
            if [[ $curl_exit -eq 0 ]]; then
                CONTROL_PORT=$port
                print_status "PASS" "Control service found on port $port"
                return 0
            fi
        done
        print_status "FAIL" "Control service is not responding on any expected port (8335-8338)"
        echo "HINT: Make sure khedra daemon is running with control service enabled"
        exit 1
    fi
}

# Function to test API endpoint
test_api_endpoint() {
    local method=$1
    local endpoint=$2
    local expected_status=$3
    local description=$4
    
    local response
    local status_code
    
    response=$(curl -s -m 10 -w "\n%{http_code}" -X "$method" "$CONTROL_URL/$endpoint" 2>/dev/null)
    local curl_exit=$?
    
    if [[ $curl_exit -ne 0 ]]; then
        print_status "FAIL" "$description (curl failed with exit code $curl_exit)"
        return 1
    fi
    
    status_code=$(echo "$response" | tail -n1)
    # Cross-platform way to get all but last line
    if command -v head >/dev/null 2>&1; then
        # Try head -n -1, fall back to sed if it fails
        local body=$(echo "$response" | head -n -1 2>/dev/null || echo "$response" | sed '$d')
    else
        local body=$(echo "$response" | sed '$d')
    fi
    
    if [[ "$status_code" == "$expected_status" ]]; then
        print_status "PASS" "$description (HTTP $status_code)"
        if [[ -n "$body" && "$body" != "null" ]]; then
            echo "  Response: $body"
        fi
    else
        print_status "FAIL" "$description (Expected HTTP $expected_status, got $status_code)"
        if [[ -n "$body" ]]; then
            echo "  Response: $body"
        fi
    fi
}

# Function to test CLI command
test_cli_command() {
    local command=$1
    local expected_exit_code=$2
    local description=$3
    
    local output
    local exit_code
    
    output=$("$KHEDRA_BIN" $command 2>&1)
    exit_code=$?
    
    if [[ $exit_code -eq $expected_exit_code ]]; then
        print_status "PASS" "$description (exit code $exit_code)"
        echo "  Output: $output"
    else
        print_status "FAIL" "$description (Expected exit code $expected_exit_code, got $exit_code)"
        echo "  Output: $output"
    fi
}

# Function to get service status via API
get_service_status() {
    local response
    response=$(curl -s -m 5 "$CONTROL_URL/isPaused" 2>/dev/null)
    local curl_exit=$?
    
    if [[ $curl_exit -eq 0 && -n "$response" ]]; then
        echo "$response" | jq -r '.[] | "\(.name): \(.status)"' 2>/dev/null || echo "$response"
    else
        print_status "WARN" "Failed to get status (curl exit: $curl_exit)"
    fi
}

# Function to capture original service states
capture_original_state() {
    print_status "INFO" "Capturing original service states..."
    local response
    response=$(curl -s -m 5 "$CONTROL_URL/isPaused" 2>/dev/null)
    local curl_exit=$?
    
    if [[ $curl_exit -ne 0 ]]; then
        print_status "WARN" "Failed to capture original state - will skip restoration"
        return 1
    fi
    
    # Parse individual service states
    if command -v jq >/dev/null 2>&1; then
        ORIGINAL_SCRAPER_STATE=$(echo "$response" | jq -r '.[] | select(.name=="scraper") | .status' 2>/dev/null || echo "unknown")
        ORIGINAL_MONITOR_STATE=$(echo "$response" | jq -r '.[] | select(.name=="monitor") | .status' 2>/dev/null || echo "unknown")
    else
        # Fallback parsing without jq
        if echo "$response" | grep -q '"scraper".*"paused"'; then
            ORIGINAL_SCRAPER_STATE="paused"
        else
            ORIGINAL_SCRAPER_STATE="running"
        fi
        
        if echo "$response" | grep -q '"monitor".*"paused"'; then
            ORIGINAL_MONITOR_STATE="paused"
        else
            ORIGINAL_MONITOR_STATE="running"
        fi
    fi
    
    print_status "INFO" "Original states - scraper: $ORIGINAL_SCRAPER_STATE, monitor: $ORIGINAL_MONITOR_STATE"
    ORIGINAL_STATE_CAPTURED=true
    return 0
}

# Function to restore original service states
restore_original_state() {
    if [[ "$ORIGINAL_STATE_CAPTURED" != "true" ]]; then
        print_status "WARN" "Original state was not captured - skipping restoration"
        return 0
    fi
    
    print_status "INFO" "Restoring original service states..."
    
    # Restore scraper state
    if [[ "$ORIGINAL_SCRAPER_STATE" == "paused" ]]; then
        curl -s -m 5 -X POST "$CONTROL_URL/pause?name=scraper" >/dev/null 2>&1
    elif [[ "$ORIGINAL_SCRAPER_STATE" == "running" ]]; then
        curl -s -m 5 -X POST "$CONTROL_URL/unpause?name=scraper" >/dev/null 2>&1
    fi
    
    # Restore monitor state
    if [[ "$ORIGINAL_MONITOR_STATE" == "paused" ]]; then
        curl -s -m 5 -X POST "$CONTROL_URL/pause?name=monitor" >/dev/null 2>&1
    elif [[ "$ORIGINAL_MONITOR_STATE" == "running" ]]; then
        curl -s -m 5 -X POST "$CONTROL_URL/unpause?name=monitor" >/dev/null 2>&1
    fi
    
    # Give services a moment to adjust
    sleep 1
    
    # Verify restoration
    verify_state_restoration
}

# Function to verify state restoration
verify_state_restoration() {
    print_status "INFO" "Verifying state restoration..."
    local response
    response=$(curl -s -m 5 "$CONTROL_URL/isPaused" 2>/dev/null)
    local curl_exit=$?
    
    if [[ $curl_exit -ne 0 ]]; then
        print_status "WARN" "Failed to verify state restoration"
        return 1
    fi
    
    local current_scraper_state current_monitor_state
    
    if command -v jq >/dev/null 2>&1; then
        current_scraper_state=$(echo "$response" | jq -r '.[] | select(.name=="scraper") | .status' 2>/dev/null || echo "unknown")
        current_monitor_state=$(echo "$response" | jq -r '.[] | select(.name=="monitor") | .status' 2>/dev/null || echo "unknown")
    else
        # Fallback parsing without jq
        if echo "$response" | grep -q '"scraper".*"paused"'; then
            current_scraper_state="paused"
        else
            current_scraper_state="running"
        fi
        
        if echo "$response" | grep -q '"monitor".*"paused"'; then
            current_monitor_state="paused"
        else
            current_monitor_state="running"
        fi
    fi
    
    if [[ "$current_scraper_state" == "$ORIGINAL_SCRAPER_STATE" && "$current_monitor_state" == "$ORIGINAL_MONITOR_STATE" ]]; then
        print_status "PASS" "Service states successfully restored"
        print_status "INFO" "Final states - scraper: $current_scraper_state, monitor: $current_monitor_state"
    else
        print_status "FAIL" "State restoration failed"
        print_status "INFO" "Expected - scraper: $ORIGINAL_SCRAPER_STATE, monitor: $ORIGINAL_MONITOR_STATE"
        print_status "INFO" "Actual - scraper: $current_scraper_state, monitor: $current_monitor_state"
    fi
}

# Cleanup function to ensure state restoration on exit
cleanup_on_exit() {
    echo
    print_status "INFO" "Performing cleanup..."
    restore_original_state
}

# Set trap to ensure cleanup happens even if script is interrupted
trap cleanup_on_exit EXIT

echo "=============================================="
echo "  KHEDRA PAUSE/UNPAUSE FUNCTIONALITY TESTS"
echo "=============================================="
echo

# Initial checks
print_status "INFO" "Starting khedra functionality tests..."
print_status "INFO" "Using khedra binary: $KHEDRA_BIN"

# Check if khedra binary exists
if [[ ! -x "$KHEDRA_BIN" ]]; then
    print_status "FAIL" "khedra binary not found or not executable at: $KHEDRA_BIN"
    echo "HINT: Make sure khedra is built with 'make' in the project root"
    exit 1
fi

check_khedra_running
print_status "INFO" "About to check control service..."
check_control_service
print_status "INFO" "Control service check completed"

# Capture original state before running tests
capture_original_state

echo
print_status "INFO" "Initial service status:"
get_service_status
echo

# Test 1: CLI pause individual services
echo "--- Test 1: CLI Pause Individual Services ---"
test_cli_command "pause scraper" 0 "CLI pause scraper"
test_cli_command "pause monitor" 0 "CLI pause monitor"

echo
print_status "INFO" "Status after CLI pause individual services:"
get_service_status
echo

# Test 2: CLI pause already paused services
echo "--- Test 2: CLI Pause Already Paused Services ---"
test_cli_command "pause scraper" 0 "CLI pause already paused scraper"
test_cli_command "pause monitor" 0 "CLI pause already paused monitor"

echo

# Test 3: CLI pause all services
echo "--- Test 3: CLI Pause All Services ---"
test_cli_command "unpause all" 0 "CLI unpause all (setup for next test)"
test_cli_command "pause all" 0 "CLI pause all services"

echo
print_status "INFO" "Status after CLI pause all:"
get_service_status
echo

# Test 4: CLI unpause individual services
echo "--- Test 4: CLI Unpause Individual Services ---"
test_cli_command "unpause scraper" 0 "CLI unpause scraper"
test_cli_command "unpause monitor" 0 "CLI unpause monitor"

echo
print_status "INFO" "Status after CLI unpause individual services:"
get_service_status
echo

# Test 5: CLI unpause already running services
echo "--- Test 5: CLI Unpause Already Running Services ---"
test_cli_command "unpause scraper" 0 "CLI unpause already running scraper"
test_cli_command "unpause monitor" 0 "CLI unpause already running monitor"

echo

# Test 6: CLI unpause all services
echo "--- Test 6: CLI Unpause All Services ---"
test_cli_command "pause all" 0 "CLI pause all (setup for next test)"
test_cli_command "unpause all" 0 "CLI unpause all services"

echo
print_status "INFO" "Status after CLI unpause all:"
get_service_status
echo

# Test 7: CLI invalid service names
echo "--- Test 7: CLI Invalid Service Names ---"
test_cli_command "pause invalid_service" 0 "CLI pause invalid service (shows error)"
test_cli_command "pause api" 0 "CLI pause non-pausable service (api) (shows error)"
test_cli_command "pause control" 0 "CLI pause non-pausable service (control) (shows error)"
test_cli_command "pause ipfs" 0 "CLI pause non-pausable service (ipfs) (shows error)"

echo

# Test 8: API pause individual services
echo "--- Test 8: API Pause Individual Services ---"
test_api_endpoint "POST" "pause?name=scraper" "200" "API pause scraper"
test_api_endpoint "POST" "pause?name=monitor" "200" "API pause monitor"

echo
print_status "INFO" "Status after API pause individual services:"
get_service_status
echo

# Test 9: API pause already paused services
echo "--- Test 9: API Pause Already Paused Services ---"
test_api_endpoint "POST" "pause?name=scraper" "200" "API pause already paused scraper"
test_api_endpoint "POST" "pause?name=monitor" "200" "API pause already paused monitor"

echo

# Test 10: API pause all services (name=all)
echo "--- Test 10: API Pause All Services (name=all) ---"
test_api_endpoint "POST" "unpause" "200" "API unpause all (setup)"
test_api_endpoint "POST" "pause?name=all" "200" "API pause all services (name=all)"

echo
print_status "INFO" "Status after API pause all (name=all):"
get_service_status
echo

# Test 11: API pause all services (no name parameter)
echo "--- Test 11: API Pause All Services (no name) ---"
test_api_endpoint "POST" "unpause" "200" "API unpause all (setup)"
test_api_endpoint "POST" "pause" "200" "API pause all services (no name)"

echo
print_status "INFO" "Status after API pause all (no name):"
get_service_status
echo

# Test 12: API unpause individual services
echo "--- Test 12: API Unpause Individual Services ---"
test_api_endpoint "POST" "unpause?name=scraper" "200" "API unpause scraper"
test_api_endpoint "POST" "unpause?name=monitor" "200" "API unpause monitor"

echo
print_status "INFO" "Status after API unpause individual services:"
get_service_status
echo

# Test 13: API unpause already running services
echo "--- Test 13: API Unpause Already Running Services ---"
test_api_endpoint "POST" "unpause?name=scraper" "200" "API unpause already running scraper"
test_api_endpoint "POST" "unpause?name=monitor" "200" "API unpause already running monitor"

echo

# Test 14: API unpause all services (name=all)
echo "--- Test 14: API Unpause All Services (name=all) ---"
test_api_endpoint "POST" "pause" "200" "API pause all (setup)"
test_api_endpoint "POST" "unpause?name=all" "200" "API unpause all services (name=all)"

echo
print_status "INFO" "Status after API unpause all (name=all):"
get_service_status
echo

# Test 15: API unpause all services (no name parameter)
echo "--- Test 15: API Unpause All Services (no name) ---"
test_api_endpoint "POST" "pause" "200" "API pause all (setup)"
test_api_endpoint "POST" "unpause" "200" "API unpause all services (no name)"

echo
print_status "INFO" "Status after API unpause all (no name):"
get_service_status
echo

# Test 16: API invalid service names
echo "--- Test 16: API Invalid Service Names ---"
test_api_endpoint "POST" "pause?name=invalid_service" "400" "API pause invalid service"
test_api_endpoint "POST" "pause?name=api" "400" "API pause non-pausable service (api)"
test_api_endpoint "POST" "pause?name=control" "400" "API pause non-pausable service (control)"
test_api_endpoint "POST" "pause?name=ipfs" "400" "API pause non-pausable service (ipfs)"

echo

# Test 17: API isPaused queries
echo "--- Test 17: API isPaused Queries ---"
test_api_endpoint "GET" "isPaused?name=scraper" "200" "API isPaused scraper"
test_api_endpoint "GET" "isPaused?name=monitor" "200" "API isPaused monitor"
test_api_endpoint "GET" "isPaused?name=all" "200" "API isPaused all"
test_api_endpoint "GET" "isPaused" "200" "API isPaused (no name)"

echo

# Test 18: Mixed CLI and API operations
echo "--- Test 18: Mixed CLI and API Operations ---"
test_cli_command "pause scraper" 0 "CLI pause scraper"
test_api_endpoint "POST" "pause?name=monitor" "200" "API pause monitor"
test_api_endpoint "GET" "is-paused" "200" "API check status"
test_cli_command "unpause all" 0 "CLI unpause all"

echo
print_status "INFO" "Final service status:"
get_service_status
echo

# Test Summary
echo "=============================================="
echo "              TEST SUMMARY"
echo "=============================================="
echo "Total tests run:    $TESTS_RUN"
echo -e "Tests passed:       ${GREEN}$TESTS_PASSED${NC}"
echo -e "Tests failed:       ${RED}$TESTS_FAILED${NC}"
echo
print_status "INFO" "OS detected: $OS_TYPE"
if [[ "$ORIGINAL_STATE_CAPTURED" == "true" ]]; then
    print_status "INFO" "Original service states were preserved and restored"
else
    print_status "WARN" "Original service states could not be captured"
fi

if [[ $TESTS_FAILED -eq 0 ]]; then
    echo -e "\n${GREEN}🎉 All tests passed!${NC}"
    # Don't exit here - let cleanup_on_exit handle state restoration
else
    echo -e "\n${RED}❌ Some tests failed.${NC}"
    # Don't exit here - let cleanup_on_exit handle state restoration
fi

# Exit with appropriate code (cleanup_on_exit will run automatically)
exit $([[ $TESTS_FAILED -eq 0 ]] && echo 0 || echo 1)
