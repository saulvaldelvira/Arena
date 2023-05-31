.PHONY: lib install uninstall clean

CC = gcc
CCFLAGS = -Wall -Wextra -Werror -pedantic -g -fPIC -O3

AR = ar
ARFLAGS = rcs

INSTALL_PATH ?= /usr/local

lib: arena.c arena.h
	$(CC) $(CCFLAGS) -shared -o libarena.so arena.c
	$(AR) $(ARFLAGS) libarena-static.a arena.c

install: lib
	$(info Installing in $(INSTALL_PATH) ...)
	@ sudo su -c ' \
	  install -d $(INSTALL_PATH)/lib ;\
	  install -m 644 libarena.so libarena-static.a $(INSTALL_PATH)/lib ;\
	  install -d $(INSTALL_PATH)/include ;\
	  install -m 644 arena.h $(INSTALL_PATH)/include ;\
	  ldconfig $(INSTALL_PATH)/lib '

uninstall:
	@ sudo su -c ' \
	  rm -f $(INSTALL_PATH)/lib/libarena.so ;\
	  rm -f $(INSTALL_PATH)/lib/libarena-static.a ;\
	  rm -f $(INSTALL_PATH)/include/arena.h '

clean:
	rm -f *.a *.so *.o
