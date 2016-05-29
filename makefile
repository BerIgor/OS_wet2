CC = g++
CFLAGS = -c -Wall
CCLINK = $(CC)
OBJS = Bank.o ATM.o commission.o logger.o screenPrinter.o
RM = rm -f
LIBS = -lpthread



all: Bank

Bank: $(OBJS)
	$(CC) -o Bank $(OBJS) $(LIBS)

ATM.o: ATM.cpp ATM.h account.h logger.h
	$(CC) $(CFLAGS) -o $@ ATM.cpp

Bank.o: Bank.cpp ATM.h account.h screenPrinter.h commission.h logger.h 
	$(CC) $(CFLAGS) -o $@ Bank.cpp $(LIBS)

commission.o: commission.cpp commission.h ATM.h account.h logger.h
	$(CC) $(CFLAGS) -o $@ commission.cpp

logger.o: logger.cpp logger.h
	$(CC) $(CFLAGS) -o $@ logger.cpp

screenPrinter.o: screenPrinter.cpp screenPrinter.h account.h ATM.h
	$(CC) $(CFLAGS) -o $@ screenPrinter.cpp

clean:
	$(RM) Bank *.o *~ "#"* core.*
