#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file



@test "Basic command: ls" {
    run ./dsh <<EOF                
ls
EOF
    output=$(echo "$output" | tr -d '[:space:]')
    actual=$(ls | tr -d '[:space:]')
    actual+="dsh2>dsh2>cmdloopreturned0"

    [ "$status" -eq 0 ]
    [ "$output" = "$actual" ]
}



@test "Basic command with arguments: ls -l" {
    run ./dsh <<EOF
ls -l
EOF
    output=$(echo "$output" | tr -d '[:space:]')
    actual=$(ls -l | tr -d '[:space:]')
    actual+="dsh2>dsh2>cmdloopreturned0"

    [ "$status" -eq 0 ]
    [ "$output" = "$actual" ]
}



@test "cd to a valid directory" {
    mkdir tempdir
    run ./dsh <<EOF
cd tempdir
pwd
EOF
    rmdir tempdir

    [ "$status" -eq 0 ]
    [ "$(echo "$output" | grep "tempdir")" != "" ] # check if output had tempdir, meaning we successfully navigated to it
}



@test "cd with no arguments (HOME)" {
    run ./dsh <<EOF
cd
pwd
EOF
    output=$(echo "$output" | tr -d '[:space:]')
    actual=$HOME
    actual+="dsh2>dsh2>dsh2>cmdloopreturned0"

    [ "$status" -eq 0 ]
    [ "$output" = "$actual" ]
}



@test "exit" {
    run ./dsh <<EOF
exit
EOF
    output=$(echo "$output" | tr -d '[:space:]')
    actual=""
    actual+="dsh2>cmdloopreturned0"
    
    [ "$status" -eq 0 ]
    [ "$output" = "$actual" ]
}



# Test case 6: Command not found
@test "Command not found" {
    run ./dsh <<EOF
LOLIDONTEXISTKILLMENOW
EOF
    [ "$status" -eq 0 ]
    [ "$(echo "$output" | grep "execvp")" != "" ] # should have execvp error
}



@test "Empty input" {
    run ./dsh <<EOF

EOF
    [ "$status" -eq 0 ]
    [ "$(echo "$output" | grep "warning: no commands provided")" != "" ] # should have warning
}


@test "Mixed quoted and unquoted arguments" {
    run ./dsh <<EOF
echo hello "world with spaces"
EOF    
    output=$(echo "$output" | tr -d '[:space:]')
    actual=$(echo hello "world with spaces" | tr -d '[:space:]')
    actual+="dsh2>dsh2>cmdloopreturned0"
    
    [ "$status" -eq 0 ]
    [ "$output" = "$actual" ]
}



@test "Long command line (exceeds SH_CMD_MAX)" {
    run ./dsh <<EOF
ls -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l -l
EOF
    [ "$status" -ne 0 ] # return non-zero (error)
}