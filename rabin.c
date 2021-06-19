//Grevenitis Ioannis

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <gmp.h>

gmp_randstate_t state;

int main(){
	char buffer[50], message[50]={'0'};	
	int j=0;
	int ascii[50];
	long sd = 0;
	//N = p*q = encryption key, (p,q) = decryption key
	mpz_t p, q, N, p1, q1, seed, plainText, cipherText, tmp, m1, m2, m3, m4 ;
	
	//Read message to encrypt
	printf("Give message to encrypt: \n");
	fgets(message ,50, stdin);


	gmp_randinit(state, GMP_RAND_ALG_LC, 120);

	//Initialize the mpz_t variables with 0 value
	mpz_init(p);
	mpz_init(q);
	mpz_init(N);
	mpz_init(p1);
	mpz_init(q1);
	mpz_init(seed);
	mpz_init(plainText);
	mpz_init(cipherText);
	mpz_init(tmp);
	mpz_init(m1);
	mpz_init(m2);
	mpz_init(m3);	
	mpz_init(m4);

	srand( (unsigned) getpid());
	sd = rand();
	mpz_set_ui(seed, sd);	//Set seed value = sd
	//Pass seed value to state
	gmp_randseed(state, seed);

	//Generate two random 200-bit numbers p1, q1
	do{	
		mpz_urandomb(p1, state, 200);
	}while( !mpz_probab_prime_p(p1, 10) );
	
	do{	
		mpz_urandomb(q1, state, 200);
	}while( !mpz_probab_prime_p(q1, 10) );
	//Print p1 and q1
	printf("p1 = "); mpz_out_str(stdout, 10, p1); printf("\n");
	printf("q1 = "); mpz_out_str(stdout, 10, q1); printf("\n");

	//Do while until p and q equal to 3 modulo 4
	do{
		mpz_nextprime(p, p1);	
	}while( !(mpz_congruent_ui_p(p, 3, 4)) );
	
	printf("p = "); mpz_out_str(stdout, 10, p); printf("\n");

	do{
		mpz_nextprime(q, q1);
	}while( !(mpz_congruent_ui_p(q, 3, 4)) );	

	
	printf("q = "); mpz_out_str(stdout, 10, q); printf("\n");
	
	mpz_mul(N, p, q);	//Calculate N = p*q
	printf("N = "); mpz_out_str(stdout, 10, N); printf("\n");
	

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

	//Encrypt
	mpz_mul(tmp, plainText, plainText);	//tmp = plainText * plainText
	//cipherText = tmp modulo N = plainText*plainText modulo p*q
	mpz_mod(cipherText, tmp, N);
	printf("Ciphertext = "); mpz_out_str(stdout, 10, cipherText); printf("\n");

	//Decrypt
	//Calculate Root 1 of 4
	mpz_add_ui(p1, p, 1);	//p1 = p + 1
	mpz_div_ui(m1, p1, 4);	//m1 = p1 / 4 = p + 1 / 4
	mpz_powm(m1, cipherText, m1, p);	//m1 = cipherText ^ m1 mod p
	
	//Calculate Root 2 of 4
	mpz_add_ui(p1, p, 1);	//p1 = p + 1
	mpz_mul_ui(m2, cipherText, -1);	//m2 = -cipherText
	mpz_div_ui(p1, p1, 4); 	// p1 = p1 / 4
	mpz_powm(m2, m2, p1, p);	//m2 = m2 ^ p1 mod p

	//Calculate Root 3 of 4
	mpz_add_ui(q1, q, 1);	//q1 = q + 1
	mpz_div_ui(m3, q1, 4);	//m3 = q1 / 4
	mpz_powm(m3, cipherText, m3, q);	//m3 = cipherText ^ m3 mod q

	//Calculate Root 4 of 4
	mpz_add_ui(q1, q, 1);	//q1 = q + 1
	mpz_mul_ui(m4, cipherText, -1);	//m4 = -cipherText
	mpz_div_ui(q1, q1, 4); 	//q1 = q1 / 4
	mpz_powm(m4, m4, q1, q);	//m4 = m4 ^ q1 mod q

	printf("Root 1: "); mpz_out_str(stdout, 10, m1); printf("\n");
	printf("Root 2: "); mpz_out_str(stdout, 10, m2); printf("\n");
	printf("Root 3: "); mpz_out_str(stdout, 10, m3); printf("\n");
	printf("Root 4: "); mpz_out_str(stdout, 10, m4); printf("\n");


	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(seed);
	mpz_clear(N);
	mpz_clear(p1);
	mpz_clear(q1);
	mpz_clear(plainText);
	mpz_clear(cipherText);
	mpz_clear(tmp);
	mpz_clear(m1);
	mpz_clear(m2);
	mpz_clear(m3);
	mpz_clear(m4);

	return 0;
}
