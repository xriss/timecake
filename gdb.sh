cd `dirname $0`

# use gdb and automatically run after loading

gdb-multiarch --command=./gdb.gdb --eval-command " continue "


