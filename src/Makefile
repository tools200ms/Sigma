#DIR_Core     = ./core
DIR_Lib      = ./lib
DIR_Shell    = ./shell
DIR_Drivers  = ./drivers
DIR_FONTS    = ./Fonts


OBJ_C = $(wildcard ${DIR_Lib}/*.c ${DIR_Shell}/*.c ${DIR_Drivers}/*.c ${DIR_GUI}/*.c ${DIR_FONTS}/*.c)
OBJ_O := ${patsubst %.c, %.o, ${OBJ_C}}

TARGET = sigma
BUILD_DIR = ../build

ifdef DEBUG
	ifeq ("${DEBUG}","y")
		#CFLAGS += -g1 -O0
		MSG = -g -O0 -Wall
		MAC = -DDEBUG=1
	else ifeq ("${DEBUG}","Y")
		#CFLAGS += -g3 -Og
		MSG = -g -O0 -Wall
		MAC = -DDEBUG=1
	endif
else
	#CFLAGS += -O2
	MSG = -g -O0 -Wall
endif

LIBS = -lwiringPi -linih

CC = gcc
# Important ! Important ! Important !

CFLAGS += $(MSG) $(MAC)


${TARGET}: ${OBJ_O}
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ${TARGET}.c $(OBJ_O) \
			$(LIBS) \
			-I $(DIR_FONTS) \
			-I $(DIR_Lib) \
			-I $(DIR_Shell) \
			-I $(DIR_Drivers) \
			-o $(BUILD_DIR)/$@
ifdef DEBUG
	@echo ""
	@echo "Debugging symbols and definitions included."
	@echo "To see program's DEBUG messages add '-d' flag to program call"
	@echo "    e.g.: ./sigma -d ..."
endif

${DIR_FONTS}/%.o:$(DIR_FONTS)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@

${DIR_Lib}/%.o:$(DIR_Lib)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@

${DIR_Shell}/%.o:$(DIR_Shell)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@

${DIR_Core}/%.o:$(DIR_Core)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@

${DIR_Drivers}/%.o:$(DIR_Drivers)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIBS)


clean:
	rm -f ${OBJ_O}
	rm -f $(BUILD_DIR)/$(TARGET)

deepclean: clean
	find . -type f -name "*~" -exec rm '{}' \;
	find . -type f -name "*.bak" -exec rm '{}' \;
	find . -type f -name "*.o" -exec rm '{}' \;
	find . -type f -name "*.so" -exec rm '{}' \;
