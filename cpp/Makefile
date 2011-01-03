test: os_test strutil_test

os.o: os.cc os.h
	g++ os.cc -c -o os.o

strutil_test: strutil.h strutil_test.cc
	g++ strutil_test.cc -o strutil_test -lgtest -lpthread -lgtest_main
	./strutil_test || rm strutil_test

os_test: os_test.cc os.o
	g++ os_test.cc os.o -o os_test -lgtest -lpthread -lgtest_main
	./os_test || rm os_test

clean:
	rm -f *.o *_test
