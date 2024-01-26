object_files = ./build/test_setup_and_cleanup.o ./build/test_execution.o
object_file_options = -fPIC -g -c -I./src -Wall

all: ./build/libtest_framework.so

./build/libtest_framework.so: $(object_files)
	gcc -shared $(object_files) -o ./build/libtest_framework.so

./build/test_setup_and_cleanup.o: ./src/test_setup_and_cleanup.c
	gcc $(object_file_options) -I../lib-hash-table/src ./src/test_setup_and_cleanup.c -o ./build/test_setup_and_cleanup.o

./build/test_execution.o: ./src/test_execution.c
	gcc $(object_file_options) -I../lib-hash-table/src ./src/test_execution.c -o ./build/test_execution.o

clean:
	rm -f ./build/*.o
	rm -f ./build/*.so