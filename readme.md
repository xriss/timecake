
TIMECAKE
========

My plan is to get as far as some sort of watch display, so far it looks 
like we can do most things at the hardware level.

See raspi directory for scripts to build an sd card image to run on a 
PI. Or check github releases for an sd card image you could just 
download.

This PI is then used as a programmer for the pinetime connected to the 
GPIO pins and to a network so we can squirt code across remotely.


To prepare.

1. `./apts.sh`
    * Install dependencies
2. `./nr5sdk.sh`
    * Download SDK
3. `./libfixmath.sh`
    * Download libfixmath ( not sure we need it but it is part of the build for now )


To build the code.


1. `./cmake.sh`
    * Perform out of build cmake, a clean project in ./out/ will be created
2. `./make.sh`
    * Compile and link in ./out/
3. `./run.sh`
    * Start gdb, connect to OpenOCD and squirt code across then exit.


If that works you may also try building a semihosting version which 
will only work while gdb is connected.


1. `./cmake.sh -D SEMIHOSTING=ON`
	* Enable semihosting (printf) to the openocd console
2. `./make.sh`
    * Compile and link in ./out/
3. `./gdb.sh`
    * Start gdb, connect to OpenOCD and squirt code across

...then in another console while gdb is still running...

1. `./tail.sh`
	* View the printf output (openocd logs) Note that gdb must still be connected or semihosting will fail.


After running this semi hosting version successfully it will have set 
the clock correctly so you can flash back to the normal version *and* 
retain the correct time. This is how I am currently setting the time :)



PINEWATCH FEATURES WORKING
--------------------------

- [x] Remote Build
- [x] Remote printf (semihosting)
- [x] Remote GDB
- [ ] Local build 
- [x] Sleep
- [x] LCD
- [x] Battery percent
- [x] Time
- [ ] Accelerometer
- [ ] Touch
- [x] Button
- [ ] Buzz
- [ ] Heart
- [ ] Storage
- [ ] BlueTooth

