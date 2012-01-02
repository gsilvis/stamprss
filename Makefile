#CFLAGS = -g -pthread -I/usr/include/libgrss -I/usr/include/atk-1.0 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/include/gtk-2.0 -I/usr/lib/gtk-2.0/include -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/webkit-1.0 -I/usr/include/libsoup-2.4 -I/usr/include/libxml2  
#LIBS = -pthread -lwebkitgtk-1.0 -lgtk-x11-2.0 -lsoup-2.4 -lgdk-x11-2.0 -latk-1.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lm -lcairo -lpango-1.0 -lfreetype -lfontconfig -lgio-2.0 -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lglib-2.0 -lgrss-1.0  

CFLAGS = `pkg-config --cflags gtk+-2.0 webkit-1.0)`
LIBS = `pkg-config --libs gtk+-2.0 webkit-1.0` -lgrss-1.0

default:
	gcc -o stamprss $(CFLAGS) -Wall stamprss.c $(LIBS)


clean:
	rm stamprss
