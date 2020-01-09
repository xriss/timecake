while true; do
# try pi1
sudo /home/pi/openocd/src/openocd -d2 -s /home/pi/openocd/tcl -f interface/raspberrypi-native.cfg  -c " bindto 0.0.0.0 ; transport select swd " -f target/nrf52.cfg -c " init ; arm semihosting enable ; reset hal>
# try pi2
sudo /home/pi/openocd/src/openocd -d2 -s /home/pi/openocd/tcl -f interface/raspberrypi2-native.cfg -c " bindto 0.0.0.0 ; transport select swd " -f target/nrf52.cfg -c " init ; arm semihosting enable ; reset hal>
# wait to try again
echo " Well that did not work... will try again in 10 secs. " | tee -a /home/pi/swd.log
sleep 10
done
