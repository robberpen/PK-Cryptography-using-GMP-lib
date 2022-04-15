all: elGamal rsa
elGamal.o: elGamal.c
rsa.o: rsa.c
clean:
	rm -f elGamal rsa
elGamal: elGamal.o
	$(CC) $@.c -lgmp -o $@
rsa: rsa.o
	$(CC) $@.c -lgmp -o $@
