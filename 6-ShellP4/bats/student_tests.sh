#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

setup() {
    # start server
    ./dsh -s &
    SERVER_PID=$!
}

teardown() {
    if [[ "${BATS_TEST_NAME}" == *"stop-server"* ]]; then
        kill $SERVER_PID
    fi
}

@test "remote: check ls runs without errors" {
    run ./dsh -c <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "remote: basic pipe" {
    run ./dsh -c <<EOF
ls | wc -l
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == *"dsh4> 10"* ]]
}

@test "remote: 2 pipes" {
    run ./dsh -c <<EOF
ls | grep .c | wc -l
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == *"dsh4> 4"* ]]
}

@test "remote: spaces in args" {
    run ./dsh -c <<EOF
echo "hello world"
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"dsh4> hello world"* ]]
}

@test "remote: cd moves directories" {
    run ./dsh -c <<EOF
cd bats
pwd
EOF
    [ "$status" -eq 0 ]
    [[ "$output" == *"6-ShellP4/bats"* ]]
}

@test "remote: exit command terminates client" {
    run ./dsh -c <<EOF
exit
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == *"server appeared to terminate - exiting"* ]]
}

@test "remote: stop-server command terminates server and client" {
    run ./dsh -c <<EOF
stop-server
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == *"server appeared to terminate - exiting"* ]]
}
