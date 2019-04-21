
Basic use, tested with Ubuntu 19.04.

	git clone https://github.com/xriss/phantom-raspberry.git
	cd phantom-raspberry
	./box-apts
	./box-format
	./install-auto-video-player

Copy the output blower.img to your sd card and it should just work.

Edit box-settings if you wish to change some of the defaults and of 
course you will want to provide your own video.mkv file to play. 

Checkout what you can do with the included scripts.

	./box-apts

Install the required dependencies using apt ( eg qemu ) This should be 
run once before doing anything else or you need to source qemu yourself 
for the rest of the scripts to work.

	./box-settings

Tweakable values, you should read and edit this file.

	./box-format

This will download raspbian (first run only) and create an output image 
with some basic system modifications, mostly to get qemu working. Take 
a look at the script for the tweaks it performs.

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




