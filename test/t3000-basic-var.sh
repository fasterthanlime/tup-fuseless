#! /bin/sh -e

# Test vars that are in the db.

. ../tup.sh
cat > Tupfile << HERE
file-y = foo.c
file-@CONFIG_BAR@ += bar.c
: foreach \$(file-y) |> cat %f > %o |> %F.o
HERE
echo hey > foo.c
echo yo > bar.c
tup touch foo.c bar.c Tupfile
tup varset CONFIG_BAR=n
update
tup_object_exist . foo.c bar.c
tup_object_exist . "cat foo.c > foo.o"
tup_object_no_exist . "cat bar.c > bar.o"

tup varset CONFIG_BAR=y
update
tup_object_exist . foo.c bar.c
tup_object_exist . "cat foo.c > foo.o"
tup_object_exist . "cat bar.c > bar.o"

tup varset CONFIG_BAR=y
check_empty_tupdirs
