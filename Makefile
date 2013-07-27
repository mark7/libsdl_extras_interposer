CFLAGS+=`sdl-config --cflags` -fPIC

all: libsdl_extras-x86_64.so libsdl_extras-i386.so

clean:
	rm -f libsdl_extras-x86_64.o libsdl_extras-x86_64.so libsdl_extras-i386.o \
	libsdl_extras-i386.so

%-i386.o: %.c
	$(CC) -m32 $(CPPFLAGS) $(CFLAGS) -c $< -o $@

%-x86_64.o: %.c
	$(CC) -m64 $(CPPFLAGS) $(CFLAGS) -c $< -o $@

libsdl_extras-x86_64.so: libsdl_extras-x86_64.o
	$(CC) -m64 -o $@ -shared -fPIC $<

libsdl_extras-i386.so: libsdl_extras-i386.o
	$(CC) -m32 -o $@ -shared -fPIC $<
