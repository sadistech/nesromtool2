#! /usr/bin/env bash

set -euo pipefail
IFS=$'\n\t'

run_test() {
  local testsrc="$1"

  local tempfile="$( mktemp /tmp/nrt_test.XXXXXX )"
  test_total=$(( test_total + 1 ))
  local testname="$( basename "$testsrc" '.c' )"

  if ! gcc -Isrc -o "$tempfile" "${sourcefiles[@]}" "$testsrc"; then
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

  rm -rf "$tempfile"

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

