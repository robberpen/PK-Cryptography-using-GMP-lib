//Grevenitis Ioannis

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <gmp.h>
#include <time.h>

gmp_randstate_t state;

int main(int argc, char *argv[]){
	char buffer[50], buffer2[50], message[50]={'0'}, tmpCharArray[2]={'0'};	
	int j=0;
	int ascii[50];
	long sd = 0;
	int ksize = 256, round = 10000;
	//e = encryption key, d = decryption key
	mpz_t p, q, N, e, d, p1, q1, F, plainText, cipherText, fBound, myGCD ;
	mpz_t seed;
	
	if (argc > 1)
		ksize = atoi(argv[1]);
	if (argc > 2)
		round = atoi(argv[2]);

	printf("key size: %d round: %d\n", ksize, round);
	//Read message to encrypt
	printf("Give message to encrypt: \n");
	//fgets(message ,50, stdin);
	sprintf(message, "1234567890");


	gmp_randinit(state, GMP_RAND_ALG_LC, 120);

	//Initialize the mpz_t variables with 0 value
	mpz_init(p);
	mpz_init(q);
	mpz_init(N);
	mpz_init(e);
	mpz_init(d);
	mpz_init(p1);
	mpz_init(q1);
	mpz_init(seed);
	mpz_init(F);
	mpz_init(plainText);
	mpz_init(cipherText);
	mpz_init(fBound);
	mpz_init(myGCD);
	
	srand( (unsigned) getpid());
	sd = rand();
	mpz_set_ui(seed, sd);	//Set seed value = sd
	//Pass seed value to state
	gmp_randseed(state, seed);

	//Generate two random 512-bit numbers 
	do{	
		mpz_urandomb(p, state, ksize);
	}while( !mpz_probab_prime_p(p, 10) );
	
	do{	
		mpz_urandomb(q, state, ksize);
	}while( !mpz_probab_prime_p(q, 10) );
	//Print p and q
	printf("p = "); mpz_out_str(stdout, 10, p); printf("\n");
	printf("q = "); mpz_out_str(stdout, 10, q); printf("\n");

	mpz_mul(N, p, q);	//Calculate N = p*q

	mpz_sub_ui(p1, p, 1); 	//p1 = p-1
	mpz_sub_ui(q1, q, 1);	//q1 = q-1
	mpz_lcm(F, p1, q1);	//Lowest common multiple of p1 and q1

	printf("F = "); mpz_out_str(stdout, 10, F); printf("\n");
	
	mpz_sub_ui(fBound, F, 1);	//fBound = F-1

	//Find e where 1 < e < F & gcd(e,F) = 1
	do{
		mpz_urandomm(e, state, fBound);
		mpz_gcd(myGCD, e, F);
	}while(mpz_cmpabs_ui(myGCD, 1));	//Compare absolute values of myGCD & 1

	printf("e = "); mpz_out_str(stdout, 10, e); printf("\n");

	mpz_invert(d, e, F);	//Invert e(encryption key) to find d(decryption key)
	printf("d = "); mpz_out_str(stdout, 10, d); printf("\n");	

	//Convert message to an int
	for(int i=0; i<strlen(message); i++){
		ascii[i] = (int) message[i];
	}

	for(int i=0; i<strlen(message)*2; i+=2){
		sprintf(&buffer[i], "%ld", ascii[j]/10);
		sprintf(&buffer[i+1], "%ld", ascii[j]%10);
		j++;
	}

	mpz_set_str(plainText, buffer, 10);
	printf("Plaintext: "); mpz_out_str(stdout, 10, plainText); printf("\n");
	clock_t t0 = clock();

	for (int i = 0; i < round; i++) {	// PTM
	//Encrypt
	mpz_powm(cipherText, plainText, e, N);
	//printf("Ciphertext = "); mpz_out_str(stdout, 10, cipherText); printf("\n");

	//Decrypt
	mpz_powm(plainText, cipherText, d, N);
	//printf("Plaintext = "); mpz_out_str(stdout, 10, plainText); printf("\n");
	}
	clock_t t1 = clock();

	printf("KPI %lu ms\n", (t1 - t0)/1000);
	//Plaintext in ASCII
	mpz_get_str(buffer, 10, plainText);
	j=0;
	for(int i=0; i<= strlen(buffer); i+=2){
		tmpCharArray[0] = buffer[i];
		tmpCharArray[1] = buffer[i+1];
		ascii[j] = atoi(tmpCharArray);
		j++;
	}

	j=0;
	do{
		message[j] = (char) ascii[j];
		j++;
	}while(!ascii[j]);
	
	
	printf("PTM Initial message = ");
	puts(message);

	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(e);
	mpz_clear(d);
	mpz_clear(seed);
	mpz_clear(N);
	mpz_clear(p1);
	mpz_clear(q1);
	mpz_clear(F);
	mpz_clear(plainText);
	mpz_clear(cipherText);
	mpz_clear(fBound);
	mpz_clear(myGCD);
	
	return 0;
}
