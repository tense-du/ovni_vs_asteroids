
# GOBject Introspection Tutorial 
 
# Written in 2013 by Simon Kågedal Reimer <skagedal@gmail.com>
#
# To the extent possible under law, the author have dedicated all
# copyright and related and neighboring rights to this software to
# the public domain worldwide. This software is distributed without
# any warranty.
#
# CC0 Public Domain Dedication:
# http://creativecommons.org/publicdomain/zero/1.0/

CFLAGS=`pkg-config --cflags glib-2.0 gobject-2.0` -g `pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs glib-2.0 gobject-2.0` `pkg-config --libs gtk+-3.0`

OBJECTS=car_gobject.o main.o battlefield.o
SOURCES=car_gobject.c car_gobject.h main.c battlefield.h battlefield.c

all: car
car: $(OBJECTS)     
	$(CC) -o $@ $(OBJECTS) $(LIBS)

car_gobject.o: car_gobject.c car_gobject.h
main.o: main.c car_gobject.h battlefield.h
battlefield.o: battlefield.h battlefield.c

clean:
	rm *.o crr
