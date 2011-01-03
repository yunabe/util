test: strutil_test

strutil_test: strutil.h strutil_test.cc
	g++ strutil_test.cc -o strutil_test -lgtest -lpthread -lgtest_main
	./strutil_test || rm strutil_test

clean:
	rm -f *.o *_test
