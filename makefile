DEVICE     = atmega328p
FUSES      = -U lfuse:w:0xce:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m
FILENAME   = main
FLAGS	   =  -Wl,-relax -g0 -O3 -mmcu=$(DEVICE)
INCLUDE	   = C:\DEV\AVR\atmega328p\libraries

default: compile upload clean

compile:
	avr-gcc $(FLAGS) -o $(FILENAME).elf $(FILENAME).c -I $(INCLUDE)
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex 
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf

upload:
	avrdude -V -F -B 0.1 -p $(DEVICE) -c usbasp -U flash:w:$(FILENAME).hex:i 
	
clean:
	del -f $(FILENAME).elf
	del -f $(FILENAME).hex
