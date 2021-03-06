CC=pdp11-aout-gcc
AS=pdp11-aout-as


%.o:%.c
	$(CC) -c -Os -m10 -msoft-float -nostartfiles  -nodefaultlibs  -nostdlib -o $@ $^

%.o:%.s
	$(AS) -o $@ $^

.PHONY: clean

clean:
	@rm -f crt0.o printf.o test1.o ashlhi3.o divmulmod.o
	@rm -f test1.out test1.data test1.text test1.textabs test1.dataabs test1.abs

test1.abs: test1.textabs test1.dataabs
	@cat $^ > $@

test1.textabs: test1.text test1.out
	CODE=`pdp11-aout-objdump  -t test1.out | cut -d ' ' -f 1,13 | grep code | cut -d ' ' -f 1`; \
	bin2abs $< 0x$$CODE > $@

test1.dataabs: test1.data test1.out
	DATA=`pdp11-aout-objdump  -t test1.out | cut -d ' ' -f 1,13 | grep data | cut -d ' ' -f 1`; \
	bin2abs $< 0x$$DATA > $@

test1.text: test1.out 
	pdp11-aout-objcopy -j .text -O binary $^ $@

test1.data: test1.out 
	pdp11-aout-objcopy -j .data -O binary $^ $@

# to get the rigt order in the binary crt0.s has to be the first file

test1.out: crt0.o test1.o printf.o divmulmod.o ashlhi3.o
	pdp11-aout-ld   -T linker-script   -nostartfiles  -nodefaultlibs  -nostdlib  $^  -o $@


