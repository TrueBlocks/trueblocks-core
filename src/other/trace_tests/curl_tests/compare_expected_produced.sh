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

# Loop over all files in expected_dir
for file in "$expected_dir"*; do
    # Get the base name of the file (removing directory path)
    basefile=$(basename "$file")

    # Check if the file exists in comparison_dir
    if [[ -f "$comparison_dir$basefile" ]]; then
        # If the file exists, run the git diff
        diff_output=$(git diff --no-index "$expected_dir$basefile" "$comparison_dir$basefile")

        # Check if the diff output is non-empty
        if [[ -n "$diff_output" ]]; then
            # If it is non-empty, save it to a file in diffs_dir
            echo "$diff_output" > "$diffs_dir$basefile.diff"
            result_string="Saved diff of $file to $diffs_dir$basefile.diff"
        else
            result_string="Diff of $file is exactly identical, not saving diffs"
        fi
    else
        # If the file does not exist in comparison_dir, print a warning
        result_string="File $basefile does not exist in $comparison_dir"
    fi

    echo "$result_string"
    # add result_string to the list of results
    result_list+=("$result_string\n")
done

# Save a file to the diffs directory to tell people the command used and time of execution
echo """Comparing $comparison_dir to $expected_dir, placing results into $diffs_dir
Command executed on $(date)
Log:
$result_list""" > "$diffs_dir"log.txt
