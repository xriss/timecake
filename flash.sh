cd `dirname $0`

echo " you must press CTRL+X then CTRL+D to exit the first gdb session so we can flash a version without semihosting afterwards "

./cmake.sh -D SEMIHOSTING=ON && ./art.sh && ./make.sh && ./gdb.sh && ./cmake.sh && ./make.sh && ./run.sh

