

all : compile_to_arduino
PHONY: compile_to_arduino

build/frequncy_new.ino.hex : frequncy_new.ino
	arduino --verify frequncy_new.ino --pref build.path=`pwd -P`/build --preserve-temp-files
	
compile_to_arduino: build/frequncy_new.ino.hex
	rm *.hex
	cp build/frequncy_new.ino.hex .
	sudo avrdude -C/home/jdv/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino9/etc/avrdude.conf -v -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:frequncy_new.ino.hex:i
	
#sudo avrdude -p atmega328p -C  /home/jdv/.arduino15/packages/arduino/tools/avrdude/6.3.0-arduino9/etc/avrdude.conf -v -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:frequncy_new.ino.hex:i
