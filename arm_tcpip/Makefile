all: TCPservice TCPclient

TCPservice: TCPservice.o errexit.o
	arm-openwrt-linux-gcc -o $@ $^

TCPclient: TCPclient.o errexit.o
	arm-openwrt-linux-gcc -o $@ $^

%.o: %.c
	arm-openwrt-linux-gcc -c -g -o $@ $<

clean:
	rm *.o TCPservice TCPclient
