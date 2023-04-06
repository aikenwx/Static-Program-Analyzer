#!/bin/bash
set -euo pipefail
# el crapo script

cd Team21/Tests21

# make sure autotester has already been built
if [[ ! -x ../Code21/build/src/autotester/autotester ]]; then
    >&2 echo "AutoTester binary not present!"
    exit 42
fi

rm -rf /tmp/autotester_test_outputs
mkdir /tmp/autotester_test_outputs

test_names="$(find . -name "*_queries.txt" | sed -e "s|_queries.txt$||" -e  "s|^\./||")"

test_dir_names="$({ echo "$test_names" | grep '/' | cut -d"/" -f 1 || test $? = 1; })"
IFS=$'\n'; for i in $test_dir_names; do mkdir "/tmp/autotester_test_outputs/$i"; done

# because asan will cause autotester to exit with exit code 1
set +e
IFS=$'\n'; for name in $test_names; do
    if [[ ! -e "${name}_source.txt" || ! -e "${name}_queries.txt" ]]; then
        >&2 echo "Couldn't find source or query file for test \"${name}\"!"
        exit 1
    fi
    ../Code21/build/src/autotester/autotester "${name}_source.txt" "${name}_queries.txt" "/tmp/autotester_test_outputs/${name}_output.xml"
done
set -e

echo
echo "All tests done!"
echo

# number of queries?
# both are kinda janky
#expected_num_queries=$(grep ' - ' *_queries.txt | wc -l)
expected_num_queries=$[ $(find . -name "*_queries.txt" -exec cat {} + | wc -l) / 5 ]

# number of passes/fails
passed=$({ find /tmp/autotester_test_outputs -name "*_output.xml" -exec grep '<passed/>' {} + || test $? = 1; } | wc -l)
failed=$({ find /tmp/autotester_test_outputs -name "*_output.xml" -exec grep '<failed>' {} + || test $? = 1; } | wc -l)
total=$[ $passed + $failed ]

echo "Total expected queries: $expected_num_queries"
echo "Passed: $passed"
echo "Failed: $failed"

if [[ $failed > 0 ]]; then
    >&2 echo
    >&2 echo "There were failed tests! Exiting with an error."
    exit 127
fi

if [[ $total == 0 ]]; then
    >&2 echo
    >&2 echo "Number of executed test cases is 0!"
    >&2 echo "Something's really wrong here... Exiting with an error."
    exit 127
fi

if [[ $expected_num_queries == 0 ]]; then
    >&2 echo
    >&2 echo "Number of expected test cases is 0!"
    >&2 echo "Something's really wrong here... Exiting with an error."
    exit 127
fi

if [[ $expected_num_queries != $total ]]; then
    >&2 echo
    >&2 echo "Number of expected test cases doesn't match total number of executed tests!"
    >&2 echo "Are there extra lines in the query files? There should only be 5 lines per query to be tested."
    exit 127
fi

