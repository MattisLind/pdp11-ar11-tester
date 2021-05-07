CC=pdp11-aout-gcc
AS=pdp11-aout-as


%.o:%.c
	$(CC) -c -O3 -m10 -msoft-float -nostartfiles  -nodefaultlibs  -nostdlib -o $@ $^

%.o:%.s
	$(AS) -o $@ $^

.PHONY: clean

clean:
	@rm -f crt0.o printf.o *.o ashlhi3.o divmulmod.o
	@rm -f *.out *.data *.text *.textabs *.dataabs *.abs

%.abs: %.textabs %.dataabs
	@cat $^ > $@

%.textabs: %.text %.out
	CODE=`pdp11-aout-objdump  -t $(word 2,$^)  | cut -d ' ' -f 1,13 | grep code | cut -d ' ' -f 1`; \
	bin2abs $< 0x$$CODE > $@

%.dataabs: %.data %.out
	DATA=`pdp11-aout-objdump  -t $(word 2,$^) | cut -d ' ' -f 1,13 | grep data | cut -d ' ' -f 1`; \
	bin2abs $< 0x$$DATA > $@

%.text: %.out 
	pdp11-aout-objcopy -j .text -O binary $^ $@

%.data: %.out 
	pdp11-aout-objcopy -j .data -O binary $^ $@

# to get the rigt order in the binary crt0.s has to be the first file

#%.out: crt0.o %.o printf.o divmulmod.o ashlhi3.o
#	pdp11-aout-ld   -T linker-script   -nostartfiles  -nodefaultlibs  -nostdlib  $^  -o $@

test4.out: crt0.o test4.o
	pdp11-aout-ld   -T linker-script   -nostartfiles  -nodefaultlibs  -nostdlib $^  -o $@

%.out: crt0.o %.o printf.o divmulmod.o ashlhi3.o
	pdp11-aout-ld   -T linker-script   -nostartfiles  -nodefaultlibs  -nostdlib  $^  -o $@
