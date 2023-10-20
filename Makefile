# vim: tw=0 nowrap

CORE_SRC      := mcu-relay-controller-iface.h mcu-relay-controller.c
DUMMY_SRC     := $(CORE_SRC) hardware-details/dummy.c
ATTINY_SRC    := $(CORE_SRC) hardware-details/attiny.c
PIC12F675_SRC := $(CORE_SRC) hardware-details/pic12f675.c
PIC10F320_SRC := $(CORE_SRC) hardware-details/pic10f320.c

all: dummy attiny13 attiny85 pic12f675 pic10f320

attiny13: $(ATTINY_SRC)
	avr-gcc -Os -std=gnu99 -DATTINY13 -DF_CPU=1000000UL -mmcu=attiny13 -o attiny13.elf mcu-relay-controller.c hardware-details/attiny.c
	avr-objcopy -j .text -j .data -O ihex attiny13.elf attiny13.hex

attiny85: $(ATTINY_SRC)
	avr-gcc -Os -std=gnu99 -DF_CPU=1000000UL -mmcu=attiny85 -o attiny85.elf mcu-relay-controller.c hardware-details/attiny.c
	avr-objcopy -j .text -j .data -O ihex attiny85.elf attiny85.hex

clean:
	rm -f *.elf *.hex *.hxl *.o *.s *.p1 *.sdb *.sym *.cmf *.lst *.rlf *.d *~ a.out

