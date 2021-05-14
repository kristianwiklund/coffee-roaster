
coffee-roaster.ino.hex: coffee-roaster.ino
	~/bin/arduino-cli compile -v --output-dir . -b arduino:avr:uno coffee-roaster.ino 

upload: coffee-roaster.ino.hex
	$(eval PORT=$(shell ./findport.sh))
	echo $(PORT)
	~/bin/arduino-cli upload --input-dir . -p $(PORT) -v -b arduino:avr:uno coffee-roaster.ino
	touch upload

connect: upload
	$(eval PORT=$(shell ./findport.sh))
	screen $(PORT) 115200

reset:
	$(eval PORT=$(shell ./findport.sh))
	avrdude -p m328p -P $(PORT) -c arduino

~/.arduino15/arduino-cli.yaml:
	arduino-cli config init

arduino-deps: ~/.arduino15/arduino-cli.yaml
	arduino-cli core install arduino:avr
	arduino-cli config set library.enable_unsafe_install true
	arduino-cli lib install --git-url https://github.com/kristianwiklund/BrewpiLibs
	arduino-cli lib install DigitalIO
	arduino-cli lib install "Adafruit MAX31855 library"
	arduino-cli lib install "Adafruit MAX31865 library"

FORCE:
