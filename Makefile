CC=pdp11-aout-gcc
AS=pdp11-aout-as


%.o:%.c
	$(CC) -c -Os -m10 -msoft-float -nostartfiles  -nodefaultlibs  -nostdlib -o $@ $^

%.o:%.s
	$(AS) -o $@ $^

.PHONY: clean

clean:
	@rm -f $(OBJS)

test1.abs: papertape-pdp11.textabs papertape-pdp11.dataabs
	@cat $^ > $@

test1.textabs: papertape-pdp11.text
	@CODE=`pdp11-aout-objdump  -t papertape-pdp11.o | cut -d ' ' -f 1,13 | grep code | cut -d ' ' -f 1`; \
	bin2abs $^ 0x$$CODE 0 > $@

test1.dataabs: papertape-pdp11.data
	@DATA=`pdp11-aout-objdump  -t papertape-pdp11.o | cut -d ' ' -f 1,13 | grep data | cut -d ' ' -f 1`; \
	bin2abs $^ 0x$$DATA 1 > $@

test1.text: papertape-pdp11.o 
	@pdp11-aout-objcopy -j .text -O binary $^ $@

test1.data: papertape-pdp11.o 
	pdp11-aout-objcopy -j .data -O binary $^ $@

# to get the rigt order in the binary crt0.s has to be the first file

test1.o: crt0.o papertape.o pdp11-main.o
	@pdp11-aout-ld   -T linker-script   -nostartfiles  -nodefaultlibs  -nostdlib  $^  -o $@


