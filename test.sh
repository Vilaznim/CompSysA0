#!/usr/bin/env bash

# Exit immediately if any command below fails.
set -e

make


echo "Generating a test_files directory.."
mkdir -p test_files
rm -f test_files/*


echo "Generating test files.."
printf "Hello, World!\n" > test_files/ascii.input
printf "Hello, World!" > test_files/ascii2.input
printf "Hello,\x00World!\n" > test_files/data.input
printf "" > test_files/empty.input
### TODO: Generate more test files ###

### ascii tests ###
# Only allowed control characters
printf "\x07\x08\x09\x0A\x0B\x0C\x0D" > test_files/ascii_ctrl.input

# Only ESC character
printf "\x1B" > test_files/ascii_esc.input

# Only printable ASCII
printf "ABCDEFGHIJKLMNOPQRSTUVWXYZ" > test_files/ascii_print.input

# All printable ASCII
printf "$(printf '\\x%02X' {32..126} | xxd -r -p)" > test_files/ascii_all_print.input

# Mixture of control and printable
printf "\x07Hello\x0AWorld\x1B!" > test_files/ascii_mix.input

# Single allowed character
printf "A" > test_files/ascii_single.input

# All allowed control and printable characters
printf "\x07\x08\x09\x0A\x0B\x0C\x0D\x1B$(printf '\\x%02X' {32..126} | xxd -r -p)" > test_files/ascii_full.input

# File with forbidden character (should not be ASCII)
printf "Hello\x00World" > test_files/ascii_forbidden.input

# File with only space
printf " " > test_files/ascii_space.input

# File with only tilde
printf "~" > test_files/ascii_tilde.input

# File with only exclamation mark
printf "!" > test_files/ascii_exclaim.input

# File with only numbers
printf "0123456789" > test_files/ascii_numbers.input

### utf-8 tests ###
# Valid single-byte UTF-8 (ASCII)
printf "A" > test_files/utf8_ascii.input

# Valid two-byte UTF-8 (e.g., 'æ' U+00E6)
printf "\xC3\xA6" > test_files/utf8_ae.input

# Valid three-byte UTF-8 (e.g., '€' U+20AC)
printf "\xE2\x82\xAC" > test_files/utf8_euro.input

# Valid four-byte UTF-8 (e.g., U+1F600 '😀')
printf "\xF0\x9F\x98\x80" > test_files/utf8_emoji.input

# Mixed valid UTF-8 (ASCII + multi-byte)
printf "A\xC3\xA6\xE2\x82\xAC\xF0\x9F\x98\x80" > test_files/utf8_mixed.input

# Lone continuation byte (invalid UTF-8)
printf "\x80" > test_files/utf8_lone_cont.input

# Valid two-byte followed by invalid byte
printf "\xC3\xA6\xFF" > test_files/utf8_valid_invalid.input

# Valid three-byte followed by ASCII
printf "\xE2\x82\xAC""A" > test_files/utf8_three_ascii.input



echo "Running the tests.."
exitcode=0
for f in test_files/*.input
do
  echo ">>> Testing ${f}.."
  file    ${f} | sed -e 's/ASCII text.*/ASCII text/' \
                         -e 's/Unicode text.*/Unicode text/' \
                         -e 's/UTF-8 Unicode text.*/UTF-8 Unicode text/' \
                         -e 's/ISO-8859 text.*/ISO-8859 text/' \
                         -e 's/writable, regular file, no read permission/cannot determine (Permission denied)/' \
                         > "${f}.expected"
  ./file  "${f}" > "${f}.actual"

  if ! diff -u "${f}.expected" "${f}.actual"
  then
    echo ">>> Failed :-("
    exitcode=1
  else
    echo ">>> Success :-)"
  fi
done
exit $exitcode
