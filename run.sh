
# use gdb to upload and then quit

gdb-multiarch --command=./gdb.gdb --eval-command " monitor reset " --eval-command " quit "

