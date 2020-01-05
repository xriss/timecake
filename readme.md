
UNDER CONSTRUCTION ALL WORDS ARE LIES
=====================================

![under construction](https://media.giphy.com/media/EIiJp9cQ3GeEU/giphy.gif)
![badydance](http://giphygifs.s3.amazonaws.com/media/iqBr5cmyz0t0s/giphy.gif)
![construction](https://media.giphy.com/media/WrOweqqiDcvoQszeSu/giphy.gif)



See raspi directory for scripts to build an sd card image to run on a 
PI. Or check github releases for an sd card image you could just 
download.

This PI is then used as a programmer for the pinetime connected to the 
GPIO pins and to a network so we can squirt code across remotely.


To build code, which currently just flashes the screen backlight to 
prove something is running.


1. `./apts.sh`
    * Install dependencies
2. `./nr5sdk.sh`
    * Download SDK
3. `./cmake.sh`
    * Perform out of build cmake into ./out/
4. `./make.sh`
    * Compile and link in ./out/
5. `./gdb.sh`
    * Start gdb connected to OpenOCD and squirt code across
    * Type "continue" to run the code

You will need to fix the IP of the PI found in ./gdb.gdb as right now 
this is hardcoded and will only work for me.


Hope this helps, but remember it is all just notes as I go...

