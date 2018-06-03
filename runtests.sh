#! /usr/bin/env bash

set -euo pipefail
IFS=$'\n\t'

test_total=0
error_total=0

mapfile -t sourcefiles < <( find src -name '*.c' -not -name 'main.c' )

for testsrc in tests/*; do
  tempfile="$( mktemp /tmp/nrt_test.XXXXXX )"
  test_total=$(( test_total + 1 ))
  testname="$( basename "$testsrc" '.c' )"

  if ! gcc -Isrc -o "$tempfile" "${sourcefiles[@]}" "$testsrc"; then
    printf "[ FAIL ] %s\n" "$testname"
    error_total=$(( error_total + 1 ))
    continue
  fi

  if ! "$tempfile"; then
    printf "[ FAIL ] %s\n" "$testname"
    error_total=$(( error_total + 1 ))
    rm -rf "$tempfile"
    continue
  fi

  printf "[ PASS ] %s\n" "$testname"
done

printf "\n"
printf "Total tests: %d\nErrors: %d\n\n" "$test_total" "$error_total"

if (( error_total )); then
  exit 1
fi

