
Tested with Ubuntu 19.10 run the scripts in this order.

	./box-apts
	./box-format
	./install-openocd

This will download apt dependencies then create and setup an sd card 
image and finally build and install openocd so that it runs at boot 
using the default raspberry GPIO pins.

	3v3      ->   3,3V VCC
	GND      ->   GND
	BCM 25   ->   SWDIO
	BCM 11   ->   SWDCLK

Write the raspocd.img created by these scripts to an sd card. You can 
also find a prebuilt raspocd.img.gz under releases to download gunzip and 
write as this will take some time to build.

Boot the pi. I personally prefer to plug in a network cable between my 
ubuntu laptop and the PI. I set the ethernet port to shared internet on 
my laptop ( nm-connection-editor , wired connection , ipv4 ,  method = 
shared to other computers ) and just use nmap to find the address the 
pi got assigned. Hopefully raspocd.local will ork as the 
hostname, but you may need to just track down the IP and use that 
instead.

Check the PI is available

	ping raspocd.local
	nmap -v -T5 10.42.0.0/24 -oG - | grep Up	

In theory you can

	telnet raspocd.local 4444

and connect to a running OpenOCD server, the pi should continuously try 
and connect using this script

	/home/pi/openocd.boot.sh
	
Logs can be found in /home/pi/swd.log which will be full of errors 
until you connect the GPIO pins.

For more advanced settings rename this script so it does not run at 
boot and continuously try to connect. ( all the .boot.sh files in the 
pi home directory will auto run. ) then just ssh in yourself to run 
things manually.

---

The scripts for handling the qemu image are documented bellow if you
run into any problems or wish to tweak things.

	./box-apts

Install the required dependencies using apt ( eg qemu ) This should be 
run once before doing anything else or you need to source qemu yourself 
for the rest of the scripts to work.

	./box-settings

Tweakable values, you should read and edit this file.

	./box-format

This will download raspbian (first run only) and create a fresh output 
image with some basic system modifications, mostly to get qemu working. 
Take a look at the script for the tweaks it performs.

Run this script again to reset the image back to base, it is quite 
quick as we do not do an apt upgrade which can take a very long time. 
If you want to make sure the image has the latest patches be sure to 
apt upgrade before using.

	./box-mount

Mount the image in boot and root folders, after this you can copy files 
into the image, eg root/home/pi is the home directory for the pi user.

This must not be run while the box is up or bad things may happen.

	./box-umount

Un mount the image, again please allways run this after mounting to 
avoid possible issues.

	./box-up

Start a qemu box, this script will take over the current console so you 
will need to switch to a new one to ./ssh into it.

THis must not be run while the image has been mounted or bad things may 
happen.

	./box-up-wait

Start a qemu box in the background and wait until ssh is working, after 
running this you can use ./ssh to run commands on it.

	./ssh

If you have created an image and it is currentl up (booted in a qemu 
box) then this script will ssh you into it.

	./box-down

Shut down the running qemu box, this must be run for a clean shutdown 
otherwise your image will not boot on a real raspberry pi.
