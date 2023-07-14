#!/bin/bash

# Define directories
if [ $# -lt 1 ]; then
    comparison_dir="produced/"  # folder containing test results to compare
else
    comparison_dir=$1
    [[ "${comparison_dir: -1}" != "/" ]] && comparison_dir="$comparison_dir/"  # add a trailing slash if it does not exist
fi
if [ $# -lt 2 ]; then
    expected_dir="expected/"  # folder containing expected results
else
    expected_dir=$2
    [[ "${expected_dir: -1}" != "/" ]] && expected_dir="$expected_dir/"  # add a trailing slash if it does not exist
fi
if [ $# -lt 3 ]; then
    diffs_dir="diffs/"  # folder to save the diffs of the comparison
else
    diffs_dir=$3
    [[ "${diffs_dir: -1}" != "/" ]] && diffs_dir="$diffs_dir/"  # add a trailing slash if it does not exist
fi
echo "Comparing $comparison_dir to $expected_dir, placing results into $diffs_dir"

# hit enter to continue
echo "Press enter to continue..."
read

# Ensure that diffs_dir exists and is empty
mkdir -p "$diffs_dir"
rm -f "$diffs_dir"*.diff

for file in "$expected_dir"*; do  # Loop over all files in expected_dir
    basefile=$(basename "$file")  # Get the base name of the file (removing directory path)
    if [[ -f "$comparison_dir$basefile" ]]; then  # If the file exists, run the git diff
        # save command to run in a command_list
        command_to_run="git diff --no-index $expected_dir$basefile $comparison_dir$basefile"
        command_list+=("$command_to_run")
        diff_output=$(eval "$command_to_run")
        if [[ -n "$diff_output" ]]; then  # if we have non-empty diffs
            echo "$diff_output" > "$diffs_dir$basefile.diff"  # save to a diff file
            difference_list+=("$basefile")  # add the file to the list of differences
        else
            identical_list+=("$basefile")  # add the file to the list of identical files
        fi
    else
        not_found_list+=("$basefile")  # add the file to the list of not found files
    fi
done

# Save a file to the diffs directory to tell people the command used and time of execution
echo """Comparing $comparison_dir to $expected_dir, placing results into $diffs_dir
Command executed on $(date)
=== Commands executed ===
$(printf "%s\n" "${command_list[@]}")
=== Not found ===
$(printf "%s\n" "${not_found_list[@]}")
=== Identical ===
$(printf "%s\n" "${identical_list[@]}")
=== Difference found ===
$(printf "%s\n" "${difference_list[@]}")""" > "$diffs_dir"log.txt

# print the log file
cat "$diffs_dir"log.txt
