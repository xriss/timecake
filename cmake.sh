
echo
echo " run this script with -D SEMIHOSTING=ON to enable printf output to the openocd console "
echo

rm -rf out
mkdir out
cd out
cmake .. -LH $*

