cd `dirname $0`

rm -rf nr5sdk
wget https://www.nordicsemi.com/-/media/Software-and-other-downloads/SDKs/nRF5/Binaries/nRF5SDK160098a08e2.zip
mkdir nr5sdk
cd nr5sdk
unzip ../nRF5SDK160098a08e2.zip
cd ..
rm nRF5SDK160098a08e2.zip
