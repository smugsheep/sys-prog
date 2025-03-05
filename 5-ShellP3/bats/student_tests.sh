#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "basic pipe: ls | wc -l" {
    run ./dsh <<EOF
ls | wc -l
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="7dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [[ "$stripped_output" == "$expected_output" ]]
    [ "$status" -eq 0 ]
}

@test "basic pipe: echo test | cat" {
    run ./dsh <<EOF
echo test | cat
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="testdsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [[ "$stripped_output" == "$expected_output" ]]
    [ "$status" -eq 0 ]
}

@test "2 pipes: ls | grep .c | wc -l" {
    run ./dsh <<EOF
ls | grep .c | wc -l
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="2dsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [[ "$stripped_output" == "$expected_output" ]]
    [ "$status" -eq 0 ]
}

@test "3 pipes: echo test | cat | cat | cat" {
    run ./dsh <<EOF
echo test | cat | cat | cat
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="testdsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [[ "$stripped_output" == "$expected_output" ]]
    [ "$status" -eq 0 ]
}

@test "maximum (8) pipes: echo test | cat | cat..." {
    run ./dsh <<EOF
echo test | cat | cat | cat | cat | cat | cat | cat
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="testdsh3>dsh3>cmdloopreturned0"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [[ "$stripped_output" == "$expected_output" ]]
    [ "$status" -eq 0 ]
}

@test "over maximum (9) pipes: echo test | cat | cat..." {
    run ./dsh <<EOF
echo test | cat | cat | cat | cat | cat | cat | cat | cat
EOF

    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh3>error:pipinglimitedto8commandsdsh3>cmdloopreturned-2"

    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    [[ "$stripped_output" == "$expected_output" ]]
    [ "$status" -eq 0 ]
}
