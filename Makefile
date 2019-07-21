SRCS     = $(wildcard ./src/*.c ./*.c)
SRCSM    = $(wildcard ./*.c)
OBJS     = $(patsubst %.c,%.o, $(SRCS))
OBJSM    = $(patsubst %.c,%.o,$(SRCSM))
TARGETS  = $(SRCSM:%.c=%)
#MCU      =arm-none-eabi-
ifeq ($(CC),cc)
CC=gcc-
CCC=gcc
else
CCC	 =$(CC)gcc
endif
all : $(TARGETS)

$(TARGETS): %: %.o  
	$(CCC)  -Wall -I ./src  -o $@ $<  -L. -lFunc 
	
$(OBJS): %.o: %.c  
	$(CCC) -Wall -I ./src -c $(CPU) -o $@ $<
lib-pc:
	$(CCC) -Wall -I ./src  $(PARA) -c $(CPU) $(wildcard ./src/*.c) -o libFunc$(CPU).o
	$(CC)ar crv ./lib/libFunc$(PARA).a libFunc$(CPU).o 	
lib-arm:
	$(CCC) -Wall -I ./src  $(PARA) -c $(CPU) $(wildcard ./src/*.c) -o libFunc$(CPU).o
	$(CC)ar crv ./lib/libFunci"$(PARA)".a libFunc$(CPU).o
clean :
	    @rm -rf $(TARGETS) $(OBJS) libFunc*
