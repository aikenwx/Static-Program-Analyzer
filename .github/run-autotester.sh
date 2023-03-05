#!/bin/bash
# el crapo script
# make sure autotester has already been built

cd Team*/Tests*
rm -rf /tmp/test_outputs
mkdir /tmp/test_outputs
for i in *_queries.txt; do name="$(echo $i | cut -d'_' -f 1)"; ../Code21/build/src/autotester/autotester ${name}_source.txt ${name}_queries.txt /tmp/test_outputs/${name}_output.xml; done
echo
echo "All tests done!"
echo

# number of queries?
# both are kinda janky
#expected_num_queries=$(grep ' - ' *_queries.txt | wc -l)
expected_num_queries=$[ $(cat *_queries.txt | wc -l) / 5 ]

# number of passes/fails
passed=$(grep '<passed/>' /tmp/test_outputs/*_output.xml | wc -l)
failed=$(grep '<failed>' /tmp/test_outputs/*_output.xml | wc -l)
total=$[ $passed + $failed ]

if [[ $expected_num_queries != $total ]]; then
    >&2 echo "Number of expected test cases doesn't match total number of executed tests!"
    >&2 echo "The expected number of test cases might not be accurate though, so..."
    >&2 echo
fi

echo "Total expected queries: $expected_num_queries"
echo "Passed: $passed"
echo "Failed: $failed"

if [[ $failed > 0 ]]; then
    exit 127
fi

