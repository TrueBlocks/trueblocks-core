#!/usr/bin/env bash

# test_perf.sh - Simple performance and correctness testing script
# Tests both trueblocks.eth and rotki.eth with cache clearing
# Usage: ./test_perf.sh [--cache]  (use --cache instead of --decache)

set -e  # Exit on any error

# Parse command line arguments
CACHE_MODE="--decache"  # Default to decache
for arg in "$@"; do
    case $arg in
        --cache)
            CACHE_MODE="--cache"
            echo -e "\033[1;33mUsing --cache mode instead of --decache\033[0m"
            shift
            ;;
        --help|-h)
            echo "Usage: $0 [--cache]"
            echo ""
            echo "Options:"
            echo "  --cache    Use --cache instead of --decache for initialization"
            echo "  --help     Show this help message"
            echo ""
            echo "Default behavior uses --decache to clear cache before each test."
            exit 0
            ;;
        *)
            # Unknown option
            ;;
    esac
done

# Global variables for storing results
TRUEBLOCKS_OLD_TIME=""
TRUEBLOCKS_NEW_TIME=""
ROTKI_OLD_TIME=""
ROTKI_NEW_TIME=""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build"

# Test output directories (in build directory which is .gitignored)
TEST_DIR="$BUILD_DIR/test_results"
OLD_DIR="$TEST_DIR/old"
NEW_DIR="$TEST_DIR/new"

# Clean cache function
clean_cache() {
    local address=$1
    if [ "$CACHE_MODE" = "--cache" ]; then
        echo -e "${YELLOW}Setting up cache for $address...${NC}"
    else
        echo -e "${YELLOW}Clearing cache for $address...${NC}"
    fi
    cd "$BUILD_DIR"
    yes | ../bin/chifra export --logs "$address" "$CACHE_MODE" --max_records 1500 > /dev/null 2>&1 || true
}

# Clean up function
cleanup() {
    echo -e "${YELLOW}Cleaning up test directories...${NC}"
    rm -rf "$TEST_DIR"
}

# Set up test directories
setup_dirs() {
    echo -e "${BLUE}Setting up test directories...${NC}"
    rm -rf "$TEST_DIR"
    mkdir -p "$OLD_DIR" "$NEW_DIR"
}

# Run test with timing
run_test() {
    local new_code_value=$1
    local output_file=$2
    local label=$3
    local address=$4
    local time_var=$5
    
    echo -e "${BLUE}Running $label implementation for $address...${NC}"
    
    # Set environment variable
    export NEW_CODE=$new_code_value
    
    # Change to build directory for running tests
    cd "$BUILD_DIR"
    
    # Build the chifra command (run from build directory)
    local cmd="../bin/chifra export --logs $address --max_records 1500 --output $output_file"
    
    echo "Command: $cmd"
    echo "Environment: NEW_CODE=$NEW_CODE"
    echo "Working directory: $(pwd)"
    
    # Capture timing to a variable
    local time_output
    if command -v gtime >/dev/null 2>&1; then
        # Use GNU time if available (more detailed output)
        time_output=$(gtime -f "%e %U %S %M" $cmd 2>&1)
    else
        # Fall back to built-in time and capture output
        time_output=$( (time $cmd) 2>&1 | grep -E "(real|user|sys)" | tail -3)
    fi
    
    # Store timing results for later formatting
    eval "$time_var=\"$time_output\""
    
    # Unset environment variable
    unset NEW_CODE
}

# Compare outputs
compare_outputs() {
    local address=$1
    echo -e "${BLUE}Comparing outputs for $address...${NC}"
    
    if diff -r "$OLD_DIR" "$NEW_DIR" > /dev/null; then
        echo -e "${GREEN}✅ CORRECTNESS VERIFIED: Both implementations produce identical outputs for $address${NC}"
        
        # Show what was generated
        local file_count=$(find "$OLD_DIR" -type f | wc -l)
        local total_size=$(du -sh "$OLD_DIR" 2>/dev/null | cut -f1 || echo "0B")
        echo -e "${GREEN}Generated $file_count file(s), total size: $total_size${NC}"
        
        # Show sample of first file if any exist
        local first_file=$(find "$OLD_DIR" -type f -name "*.csv" | head -1)
        if [[ -n "$first_file" ]]; then
            local line_count=$(wc -l < "$first_file")
            echo -e "${GREEN}Sample file: $(basename "$first_file") ($line_count lines)${NC}"
        fi
        
        return 0
    else
        echo -e "${RED}❌ CORRECTNESS FAILURE: Implementations produce different outputs for $address!${NC}"
        echo -e "${RED}Differences found:${NC}"
        diff -r "$OLD_DIR" "$NEW_DIR" | head -10
        return 1
    fi
}

# Performance comparison
performance_summary() {
    local address="$1"
    local old_time="$2"
    local new_time="$3"
    
    echo -e "${GREEN}Performance Summary for $address${NC}"
    echo "========================================"
    
    # Parse timing results based on format
    local old_real new_real
    
    if command -v gtime >/dev/null 2>&1; then
        # GNU time format: "real user sys memory"
        old_real=$(echo "$old_time" | awk '{print $1}')
        new_real=$(echo "$new_time" | awk '{print $1}')
        local old_memory=$(echo "$old_time" | awk '{print $4}')
        local new_memory=$(echo "$new_time" | awk '{print $4}')
        
        echo "Implementation | Real Time | Memory KB | Performance"
        echo "-------------- | --------- | --------- | -----------"
        echo "Old            | ${old_real}s     | $old_memory      | baseline"
        
        # Calculate percentage difference
        if [[ -n "$old_real" && -n "$new_real" && "$old_real" != "0" ]]; then
            local percent_diff=$(echo "scale=1; ($new_real - $old_real) / $old_real * 100" | bc -l)
            if (( $(echo "$percent_diff > 0" | bc -l) )); then
                echo "New            | ${new_real}s     | $new_memory      | ${percent_diff}% slower"
            else
                local abs_diff=$(echo "$percent_diff * -1" | bc -l)
                echo "New            | ${new_real}s     | $new_memory      | ${abs_diff}% faster"
            fi
        else
            echo "New            | ${new_real}s     | $new_memory      | comparison failed"
        fi
    else
        # Built-in time format - extract real time
        old_real=$(echo "$old_time" | grep "real" | awk '{print $2}' | sed 's/[ms]//g')
        new_real=$(echo "$new_time" | grep "real" | awk '{print $2}' | sed 's/[ms]//g')
        
        echo "Implementation | Real Time | Performance"
        echo "-------------- | --------- | -----------"
        echo "Old            | $old_real    | baseline"
        
        # For built-in time, we need to handle different formats (0m0.681s vs 0.681s)
        if [[ -n "$old_real" && -n "$new_real" ]]; then
            # Convert to seconds if in m:s format
            old_seconds=$(echo "$old_real" | awk -F'm' '{if(NF>1) print $1*60+$2; else print $1}' | sed 's/s//')
            new_seconds=$(echo "$new_real" | awk -F'm' '{if(NF>1) print $1*60+$2; else print $1}' | sed 's/s//')
            
            if [[ -n "$old_seconds" && -n "$new_seconds" && "$old_seconds" != "0" ]]; then
                local percent_diff=$(echo "scale=1; ($new_seconds - $old_seconds) / $old_seconds * 100" | bc -l)
                if (( $(echo "$percent_diff > 0" | bc -l) )); then
                    echo "New            | $new_real    | ${percent_diff}% slower"
                else
                    local abs_diff=$(echo "$percent_diff * -1" | bc -l)
                    echo "New            | $new_real    | ${abs_diff}% faster"
                fi
            else
                echo "New            | $new_real    | comparison failed"
            fi
        fi
    fi
    
    echo ""
}

# Run a complete test for one address
run_address_test() {
    local address=$1
    local test_num=$2
    
    echo -e "${GREEN}=======================================${NC}"
    echo -e "${GREEN}TEST $test_num: $address${NC}"
    echo -e "${GREEN}=======================================${NC}"
    
    # Clean cache first
    clean_cache "$address"
    
    # Set up fresh directories
    setup_dirs
    
    # Variables to capture timing results
    local old_timing new_timing
    
    echo ""
    echo -e "${GREEN}Phase 1: Testing OLD implementation (NEW_CODE=0)${NC}"
    echo "=================================================="
    run_test "0" "$OLD_DIR/output.csv" "OLD" "$address" old_timing
    
    echo ""
    echo -e "${GREEN}Phase 2: Testing NEW implementation (NEW_CODE=1)${NC}"
    echo "=================================================="
    # Clean cache again before new implementation
    clean_cache "$address"
    run_test "1" "$NEW_DIR/output.csv" "NEW" "$address" new_timing
    
    echo ""
    echo -e "${GREEN}Phase 3: Comparing Results${NC}"
    echo "=========================="
    
    if compare_outputs "$address"; then
        echo ""
        performance_summary "$address" "$old_timing" "$new_timing"
        
        # Store results globally for final summary
        if [[ "$address" == "trueblocks.eth" ]]; then
            TRUEBLOCKS_OLD_TIME="$old_timing"
            TRUEBLOCKS_NEW_TIME="$new_timing"
        elif [[ "$address" == "rotki.eth" ]]; then
            ROTKI_OLD_TIME="$old_timing"
            ROTKI_NEW_TIME="$new_timing"
        fi
        
        return 0
    else
        echo -e "${RED}Test failed for $address due to output differences.${NC}"
        return 1
    fi
}

# Create final summary table
final_summary_table() {
    echo ""
    echo -e "${BLUE}DETAILED PERFORMANCE COMPARISON:${NC}"
    echo "=================================="
    echo ""
    
    # Helper function to extract real time from timing output
    extract_time() {
        local time_output="$1"
        if command -v gtime >/dev/null 2>&1; then
            echo "$time_output" | awk '{print $1}'
        else
            echo "$time_output" | grep "real" | awk '{print $2}' | sed 's/[ms]//g'
        fi
    }
    
    # Helper function to calculate percentage difference
    calc_percentage() {
        local old_time="$1"
        local new_time="$2"
        
        if command -v gtime >/dev/null 2>&1; then
            # GNU time format - times are already in seconds
            if [[ -n "$old_time" && -n "$new_time" && "$old_time" != "0" ]]; then
                # Use awk for more reliable floating point arithmetic
                local percent_diff=$(awk "BEGIN {printf \"%.1f\", ($new_time - $old_time) / $old_time * 100}")
                local abs_diff=$(awk "BEGIN {printf \"%.1f\", sqrt(($percent_diff)^2)}")
                
                if (( $(awk "BEGIN {print ($percent_diff > 0.05) ? 1 : 0}") )); then
                    echo "${percent_diff}% slower"
                elif (( $(awk "BEGIN {print ($percent_diff < -0.05) ? 1 : 0}") )); then
                    echo "${abs_diff}% faster"
                else
                    echo "~0% (no significant change)"
                fi
            else
                echo "N/A"
            fi
        else
            # Built-in time format - convert to seconds
            local old_seconds=$(echo "$old_time" | awk -F'm' '{if(NF>1) print $1*60+$2; else print $1}' | sed 's/s//')
            local new_seconds=$(echo "$new_time" | awk -F'm' '{if(NF>1) print $1*60+$2; else print $1}' | sed 's/s//')
            
            if [[ -n "$old_seconds" && -n "$new_seconds" && "$old_seconds" != "0" ]]; then
                # Use awk for more reliable floating point arithmetic
                local percent_diff=$(awk "BEGIN {printf \"%.1f\", ($new_seconds - $old_seconds) / $old_seconds * 100}")
                local abs_diff=$(awk "BEGIN {printf \"%.1f\", sqrt(($percent_diff)^2)}")
                
                if (( $(awk "BEGIN {print ($percent_diff > 0.05) ? 1 : 0}") )); then
                    echo "${percent_diff}% slower"
                elif (( $(awk "BEGIN {print ($percent_diff < -0.05) ? 1 : 0}") )); then
                    echo "${abs_diff}% faster"
                else
                    echo "~0% (no significant change)"
                fi
            else
                echo "N/A"
            fi
        fi
    }
    
    # Extract times
    local tb_old_time=$(extract_time "$TRUEBLOCKS_OLD_TIME")
    local tb_new_time=$(extract_time "$TRUEBLOCKS_NEW_TIME")
    local rotki_old_time=$(extract_time "$ROTKI_OLD_TIME")
    local rotki_new_time=$(extract_time "$ROTKI_NEW_TIME")
    
    # Calculate percentages
    local tb_perf=$(calc_percentage "$tb_old_time" "$tb_new_time")
    local rotki_perf=$(calc_percentage "$rotki_old_time" "$rotki_new_time")
    
    # Display table
    echo "Address           | Old Time | New Time | Performance"
    echo "----------------- | -------- | -------- | -----------"
    printf "trueblocks.eth    | %8s | %8s | %s\n" "$tb_old_time" "$tb_new_time" "$tb_perf"
    printf "rotki.eth         | %8s | %8s | %s\n" "$rotki_old_time" "$rotki_new_time" "$rotki_perf"
    echo ""
}

# Main execution
main() {
    echo -e "${GREEN}TrueBlocks Performance & Correctness Test${NC}"
    echo -e "${GREEN}Multi-Address Testing Suite${NC}"
    echo -e "${GREEN}=========================================${NC}"
    echo ""
    
    # Check if chifra binary exists
    if [[ ! -f "$PROJECT_ROOT/bin/chifra" ]]; then
        echo -e "${RED}Error: chifra binary not found at $PROJECT_ROOT/bin/chifra${NC}"
        echo "Please build the project first: cd build && make"
        exit 1
    fi
    
    # Check if build directory exists
    if [[ ! -d "$BUILD_DIR" ]]; then
        echo -e "${RED}Error: build directory not found at $BUILD_DIR${NC}"
        echo "Please create build directory first"
        exit 1
    fi
    
    local failed_tests=0
    
    # Variables to store performance results
    local trueblocks_old_time trueblocks_new_time trueblocks_perf
    local rotki_old_time rotki_new_time rotki_perf
    
    # Test trueblocks.eth
    if run_address_test "trueblocks.eth" "1"; then
        # Store results (this is a simplified approach - we'd need to modify run_address_test to return these)
        trueblocks_perf="stored after test"
    else
        failed_tests=$((failed_tests + 1))
    fi
    
    echo ""
    echo ""
    
    # Test rotki.eth  
    if run_address_test "rotki.eth" "2"; then
        # Store results (this is a simplified approach - we'd need to modify run_address_test to return these)  
        rotki_perf="stored after test"
    else
        failed_tests=$((failed_tests + 1))
    fi
    
    echo ""
    echo -e "${GREEN}=======================================${NC}"
    echo -e "${GREEN}FINAL SUMMARY${NC}"
    echo -e "${GREEN}=======================================${NC}"
    
    if [[ $failed_tests -eq 0 ]]; then
        echo -e "${GREEN}✅ All tests passed! Both implementations produce identical results.${NC}"
        
        # Show detailed comparison table
        final_summary_table
        
        echo -e "${YELLOW}Tip: Run this script multiple times to get consistent timing results.${NC}"
        cleanup
        exit 0
    else
        echo -e "${RED}❌ $failed_tests test(s) failed due to output differences.${NC}"
        echo "Check $TEST_DIR for details."
        exit 1
    fi
}

# Handle script arguments (now ignores them and runs fixed test suite)
if [[ $# -gt 0 ]]; then
    echo "Note: This script now runs a fixed test suite for trueblocks.eth and rotki.eth"
    echo "Custom arguments are ignored."
    echo ""
fi

main