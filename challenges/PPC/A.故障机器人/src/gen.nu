#!/usr/bin/env nu

def run [problem: string] {
    print $"=== Processing ($problem) ==="

    cd $problem

    # Compile
    print "Compiling gen.cpp..."
    clang++ -o gen.bin gen.cpp -std=c++23 -Wall -Wextra -pedantic -O2

    print "Compiling validator.cpp..."
    clang++ -o validator.bin validator.cpp -std=c++23 -Wall -Wextra -pedantic -O2

    print "Compiling sol.cpp..."
    clang++ -o sol.bin sol.cpp -std=c++23 -Wall -Wextra -pedantic -O2

    # Generate inputs
    print "Generating inputs..."
    if ("samples" | path exists) {
        # rm -rf samples
    }
    mkdir samples
    ./gen.bin

    # Validate and Generate Outputs
    ls samples/*.in | sort-by name | each { |file|
        let name = ($file.name | path basename)
        let stem = ($name | str replace ".in" "")
        let ans_file = ($file.name | str replace ".in" ".ans")

        # Validate
        let val_res = (open $file.name | ./validator.bin | complete)
        if $val_res.exit_code != 0 {
            print $"(ansi red)Validation FAILED(ansi reset)"
            print $val_res.stderr
            exit 1
        }

        # Generate Output
        let std_res = (open $file.name | ./sol.bin | complete)
        if $std_res.exit_code == 0 {
            $std_res.stdout | save -f $ans_file
        } else {
            print $"(ansi red)Runtime Error(ansi reset)"
            print $std_res.stderr
            exit 1
        }
    }

    print "\nAll test cases generated and validated successfully."
}

def run_spj [problem: string] {
    print $"=== Processing ($problem) ==="

    cd $problem

    # Compile
    print "Compiling gen.cpp..."
    clang++ -o gen.bin gen.cpp -std=c++23 -Wall -Wextra -pedantic -O2

    print "Compiling validator.cpp..."
    clang++ -o validator.bin validator.cpp -std=c++23 -Wall -Wextra -pedantic -O2

    print "Compiling sol.cpp..."
    clang++ -o sol.bin sol.cpp -std=c++23 -Wall -Wextra -pedantic -O2

    print "Compiling gen_answer.cpp..."
    clang++ -o gen_answer.bin gen_answer.cpp -std=c++23 -Wall -Wextra -pedantic -O2

    print "Compiling checker.cpp..."
    clang++ -o checker.bin checker.cpp -std=c++23 -Wall -Wextra -pedantic -O2

    # Generate inputs
    print "Generating inputs..."
    if ("samples" | path exists) {
        rm -rf samples
    }
    mkdir samples
    ./gen.bin

    # Validate and Generate Outputs
    ls samples/*.in | sort-by name | each { |file|
        let name = ($file.name | path basename)
        let stem = ($name | str replace ".in" "")
        let out_file = ($file.name | str replace ".in" ".out")
        let ans_file = ($file.name | str replace ".in" ".ans")

        print $"\nProcessing test case: ($name)"

        # Validate
        let val_res = (open $file.name | ./validator.bin | complete)
        if $val_res.exit_code != 0 {
            print $"(ansi red)Validation FAILED(ansi reset)"
            print $val_res.stderr
            exit 1
        }

        # Generate Answer
        let std_res = (open $file.name | ./gen_answer.bin | complete)
        if $std_res.exit_code == 0 {
            $std_res.stdout | save -f $ans_file
        } else {
            print $"(ansi red)Runtime Error(ansi reset)"
            print $std_res.stderr
            exit 1
        }

        # Generate Output
        let std_res = (open $file.name | ./sol.bin | complete)
        if $std_res.exit_code == 0 {
            $std_res.stdout | save -f $out_file
        } else {
            print $"(ansi red)Runtime Error(ansi reset)"
            print $std_res.stderr
            exit 1
        }

        # Check Output
        let check_res = (./checker.bin $file.name $out_file $ans_file | complete)
        if $check_res.exit_code != 0 {
            print $"(ansi red)Checker FAILED(ansi reset)"
            print $check_res.stderr
            exit 1
        }
    }

    print "\nAll test cases generated, validated, and checked successfully."
}

run defect
run qwqwqwq
# run shiroko
run_spj cats
run border_of_orb