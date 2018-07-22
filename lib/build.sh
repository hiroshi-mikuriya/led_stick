gcc -shared -fPIC -I/usr/local/include  -o stick_sdk.so stick_sdk.c /usr/local/lib/libbcm2835.a
g++ -I/usr/local/include -c stick_sdk.c
ar rcs stick_sdk.a stick_sdk.o /usr/local/lib/libbcm2835.a