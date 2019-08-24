
UNDER CONSTRUCTION ALL WORDS ARE LIES
=====================================

![under construction](https://media.giphy.com/media/EIiJp9cQ3GeEU/giphy.gif)






Basic use, tested with Ubuntu 19.04. Setup with the following

	git clone https://github.com/xriss/phantom-raspberry.git
	cd phantom-raspberry
	./box-apts

Put the video you want to play as video.mkv in the phantom-raspberry 
directory. if that file does not exist we will auto download a video 
from youtube to use as a test

	./box-format
	./install-auto-video-player

This will setup an sd card image that autoplays video.mkv on loop.

Copy the output file, "blower.img", to an sd card like so 
https://www.raspberrypi.org/documentation/installation/installing-images/ 
and you will have an autoplaying video setup, just plug into a 
Raspberry PI and apply power and a screen.

---

Edit box-settings if you wish to change some of the defaults and 
checkout what the provided scripts do below.

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

	./install-auto-video-player

This will setup the image that ./box-format created to auto play a 
video, see ./box-settings for simple changes or modify this script if 
you need even more control.

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
