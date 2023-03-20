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

# because asan will cause autotester to exit with exit code 1
set +e
for i in *_queries.txt; do name="$(echo $i | cut -d'_' -f 1)"; ../Code21/build/src/autotester/autotester ${name}_source.txt ${name}_queries.txt /tmp/autotester_test_outputs/${name}_output.xml; done
set -e

echo
echo "All tests done!"
echo

# number of queries?
# both are kinda janky
#expected_num_queries=$(grep ' - ' *_queries.txt | wc -l)
expected_num_queries=$[ $(cat *_queries.txt | wc -l) / 5 ]

# number of passes/fails
passed=$({ grep '<passed/>' /tmp/autotester_test_outputs/*_output.xml || test $? = 1; } | wc -l)
failed=$({ grep '<failed>' /tmp/autotester_test_outputs/*_output.xml || test $? = 1; } | wc -l)
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

# if [[ $expected_num_queries != $total ]]; then
#     >&2 echo
#     >&2 echo "Number of expected test cases doesn't match total number of executed tests!"
#     >&2 echo "The expected number of test cases might not be accurate though, but exiting with an error anyway."
#     exit 127
# fi

