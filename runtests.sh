#! /usr/bin/env bash

## Requires bash 4.x

## Run all tests. Normally this would be run via `make test`
##
## This accomplishes the task of running our tests by iterating over each
## .c file in the tests/ directory and building it, adding 'src' to the include
## directory path.
## It should probably rely more on the makefile in some way, but I haven't
## figured that out, yet.
##
## When running tests, be sure to include the "test.h" header which defines
## some test helpers.
##
## 3 environment variables will be made available to all tests being run via
## this script:
##   * NRT_TEMP -- a temp directory created for this test and deleted at the end of the run
##   * NRT_FIXTURE_DIR -- the path to the fixtures directory, where test files live
##   * NRT_TEST_DIR -- the path to the 'tests' directory. for reasons?

set -euo pipefail
IFS=$'\n\t'

if [[ "$BASH_VERSION" != 4* && "$BASH_VERSION" != 5* ]]; then
  echo "Requires bash version 4.x. You're running $BASH_VERSION" >&2
  echo "Please install bash 4.x with your OS's package manager (ie: brew install bash)" >&2
  exit 1
fi

run_test() {
  local testsrc="$1"

  export NRT_TEMP="$( mktemp -d "/tmp/nrt_test.XXXXXX" )"

  local tempfile="$( mktemp /tmp/nrt_test.XXXXXX )"
  test_total=$(( test_total + 1 ))
  local testname="$( basename "$testsrc" '.c' )"

  if ! gcc -Isrc -o "$tempfile" "${sourcefiles[@]}" "$testsrc" -lpng; then
    printf "[ FAIL ] %s\n" "$testname"
    error_total=$(( error_total + 1 ))
    failed_tests+=("$testname")
    rm -rf "$tempfile"
    return
  fi

  if ! "$tempfile"; then
    printf "[ FAIL ] %s\n" "$testname"
    error_total=$(( error_total + 1 ))
    failed_tests+=("$testname")
    rm -rf "$tempfile"
    return
  fi

  rm -rf "$tempfile" "$NRT_TEMP"

  printf "[ PASS ] %s\n" "$testname"
}

test_total=0
error_total=0
failed_tests=()

export NRT_TEST_DIR="$( realpath tests )"
export NRT_FIXTURE_DIR="$( realpath tests/fixtures )"

mapfile -t sourcefiles < <( find src -name '*.c' -not -name '*_main.c' )
mapfile -t testfiles < <( find tests -name '*.c' )

if [[ "$#" -eq 0 ]]; then
  for testsrc in "${testfiles[@]}"; do
    run_test "$testsrc"
  done
else
  for testsrc in "$@"; do
    run_test "$testsrc"
  done
fi

printf "\n"
printf "Total tests: %d\nErrors: %d\n\n" "$test_total" "$error_total"

printf "Failures:\n"
printf " - %s\n" "${failed_tests[@]}"
printf "\n"

if (( error_total )); then
  exit 1
fi

