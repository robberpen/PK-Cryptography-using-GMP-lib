//Grevenitis Ioannis

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <gmp.h>

gmp_randstate_t state;

int main(){
	char buffer[50], tmpCharArray[2], message[50]={'0'};
	int ascii[50];
	long sd;
	int j=0, flag;
	//e = encryption key, d = decryption key, p,p2 = prime Zp = generator, k random where 1 <= k <= p-2, F = phi function, gamma and delta compose the ciphertext we send c=(gamma,delta)
	mpz_t seed, plainText, cipherText, p, p2, Zp, e, d, D, k, F, tmp, tmp2, gamma, delta;
	
	
	//Read message to encrypt
	printf("Give message to encrypt: \n");
	fgets(message ,50, stdin);
	
	gmp_randinit(state, GMP_RAND_ALG_LC, 120);
	
	//Initialize the mpz_t variables with 0 value
	mpz_init(seed);
	mpz_init(plainText);
	mpz_init(cipherText);
	mpz_init(p);
	mpz_init(p2);
	mpz_init(Zp);
	mpz_init(e);
	mpz_init(d);
	mpz_init(D);
	mpz_init(k);
	mpz_init(F);
	mpz_init(tmp);
	mpz_init(tmp2);
	mpz_init(gamma);
	mpz_init(delta);
	
	srand( (unsigned) getpid());
	sd = rand();
	mpz_set_ui(seed, sd);	//Set seed value = sd
	//Pass seed value to state
	gmp_randseed(state, seed);
	
	//Generate two random 200-bit numbers
	mpz_urandomb(p, state, 200);
	printf("p = "); mpz_out_str(stdout, 10, p); printf("\n");
	
	//Looking for a prime number p where t = p - 1 = 2 * p2
	do{
		mpz_nextprime(p2,p);
		mpz_mul_ui(p, p2, 2);	//p = p2 * 2
		mpz_add_ui(p, p, 1);	//p = p + 1
	}while( !mpz_probab_prime_p(p,10) );
		
	printf("p = "); mpz_out_str(stdout, 10, p); printf("\n");
	
	mpz_sub_ui(F, p, 1);	//F = p -1
	printf("F = "); mpz_out_str(stdout, 10, F); printf("\n");
	
	//Calculate Zp
	mpz_set_ui(Zp, 1);	//Zp = 1
	
	do{
		mpz_add_ui(Zp, Zp, 1);	//Zp = Zp + 1
		mpz_div_ui(tmp, F, 2);	//tmp = F / 2
		mpz_powm(tmp2, Zp, tmp, p);
		flag = mpz_cmpabs_ui(tmp2, 1); //If the absolute values of tmp2 and 1 are equal then set flag = 1
		
		if(flag){	
			mpz_div(tmp, F, p2);	//tmp = F / p2
			mpz_powm(tmp2, Zp, tmp, p);
			flag = mpz_cmpabs_ui(tmp, 1);
		}
		
	}while(!flag);
		
	printf("Zp = "); mpz_out_str(stdout, 10, F); printf("\n");
	
	//Calculate d where 1 <= d <= p - 2 = D
	mpz_sub_ui(D, F, 1);	//D = F - 1 = p -1 - 1 = p - 2	
	mpz_urandomm(d, state, D);
	printf("d = "); mpz_out_str(stdout, 10, d); printf("\n");
	
	//Raise Zp to d modulo p
	mpz_powm(tmp, Zp, d, p);

	//Calculate random k where 1 <= k <= p-2
	mpz_set_ui(seed, 5*sd);
	gmp_randseed(state, seed);
	mpz_urandomm(k, state, D);
	printf("k = "); mpz_out_str(stdout, 10, k); printf("\n");
	

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
	//Calculate gamma
	mpz_powm(gamma, Zp, k, p);	//gamma = Zp ^ k modulo p
	printf("Gamma = "); mpz_out_str(stdout, 10, gamma); printf("\n");
	//Calculate delta
	mpz_powm(delta, gamma, d, p);	//delta = gamma^d modulo p
	mpz_mul(delta, delta, plainText);	//delta = delta * plainText
	mpz_mod(delta, delta, p);	//delta = delta modulo p
	printf("Delta = "); mpz_out_str(stdout, 10, delta); printf("\n");
	
	//Decrypt
	//Calculate F - d
	mpz_sub(tmp, F, d);	//tmp = F - d
	//Calculate gamma ^ (-d) modulo p
	mpz_powm(tmp2, gamma, tmp, p);	//tmp2 = gamma ^ tmp modulo p
	
	//Recraft the plaintext
	mpz_mul(plainText, tmp2, delta);
	mpz_mod(plainText, plainText, p);
	printf("Plaintext: "); mpz_out_str(stdout, 10, plainText); printf("\n");
	
	//Plaintext in ASCII
	mpz_get_str(buffer, 10, plainText);
	j=0;
	for(int i=0; i<= strlen(buffer); i+=2){
		tmpCharArray[0] = buffer[i];
		tmpCharArray[1] = buffer[i+1];
		ascii[j] = atoi(tmpCharArray);
		j++;
	}		

	printf("Initial message = ");
	for(int i=0; i<strlen(message); i++){
		printf("%c",message[i]);
	}

	mpz_clear(seed);
	mpz_clear(plainText);
	mpz_clear(cipherText);
	mpz_clear(p);
	mpz_clear(p2);
	mpz_clear(Zp);
	mpz_clear(e);
	mpz_clear(d);
	mpz_clear(D);
	mpz_clear(k);
	mpz_clear(F);
	mpz_clear(tmp);
	mpz_clear(tmp2);
	mpz_clear(gamma);
	mpz_clear(delta);	

	return 0;
}
