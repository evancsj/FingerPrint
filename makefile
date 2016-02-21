FingerPrint:FingerPrint.o UART.o Command.o
	gcc -o $@ $^
FingerPrint.o:FingerPrint.c UART.h Command.h
	gcc -c $<
UART.o:UART.c UART.h
	gcc -c $<
Command.o:Command.c Command.h UART.h
	gcc -c $<
clean:
	rm FingerPrint.o UART.o Command.o  FingerPrint
