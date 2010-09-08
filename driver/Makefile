# http://www.devdrv.co.jp/linux/kernel26-makefile.htm
# こちらのmakefileを元にしました。

ifndef LINUX_HEADERS
LINUX_HEADERS:=$(shell if [ -e /usr/src/linux-headers-`uname -r` ] ;	then echo /usr/src/linux-headers-`uname -r` ; else echo /usr/src/linux-`uname -r` ; fi)
endif

TARGET:= louise_love.ko userland_louise_love

all: ${TARGET}

louise_love.ko: louise_love.c
	make -C ${LINUX_HEADERS} M=`pwd` V=1 modules

userland_louise_love: louise_love.c
	gcc -o userland_louise_love louise_love.c

clean:
	make -C ${LINUX_HEADERS} M=`pwd` V=1 clean
	rm -f userland_louise_love

install: all
	sudo mknod /dev/louise_love c 0x0721 0
	sudo chmod 0666 /dev/louise_love
	sudo insmod louise_love.ko

test: userland_louise_love
	./userland_louise_love

uninstall:
	sudo rmmod louise_love
	sudo rm -f /dev/louise_love

obj-m:= louise_love.o

clean-files := *.o *.ko *.mod.[co] *~

