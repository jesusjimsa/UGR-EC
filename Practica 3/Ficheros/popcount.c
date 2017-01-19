//  según la versión de gcc y opciones de optimización usadas, tal vez haga falta
//  usar gcc –fno-omit-frame-pointer si gcc quitara el marco pila (%ebp)

#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <sys/time.h>	// para gettimeofday(), struct timeval

#define SIZE 1<<20	// tamaño suficiente para tiempo apreciable
#define WSIZE 8*sizeof(unsigned)
unsigned lista[SIZE];
int resultado = 0;

int popcount1(int* array, int len){
	int resultado = 0;
	int x;
	int i = 0;

	for (i = 0; i < len; i++){
		x = array[i];

		do{
			resultado += x & 0x1;
			x >>= 1;
		}while(x);
	}

	return resultado;
}

int popcount2(int* array, int len){
	int resultado = 0;
	int i;
	int j;
	unsigned x;

	for(i = 0; i < len; i++){
		x = array[i];
		
		for(j = 0; j < WSIZE; j++){
			unsigned mask = 1 << j;
		
			if((x & mask)){
				resultado++;
			}
		}
	}

	return resultado;
}

int popcount3(int* array, int len){
	int i;							// inicializar array
	int x;
	int resultado = 0;
	
	for (i = 0; i < len; i++){
		x = array[i];
		
		asm("\n"
		"ini3:  \n\t"				//seguir mientras que x != 0
			"shr %[x]		\n\t"	//LSB en CF
			"adc $0x0, %[r]		\n\t"
			"test %[x], %[x]	\n\t"
			"jnz ini3		\n\t"	//Saltar si es distinto de 0
			: [r] "+r" (resultado)	// e/s: añadir a lo acumulado por el momento
			: [x] "r" (x)			//entrada: valor elemento
		);
	}
	
	return resultado;
}

int popcount4(int* array, int len){
	int result = 0, val, i, j;
	unsigned x;
	
	for (i = 0; i < len; i++){
		val = 0;
		x = array[i];

		for (j = 0;j < 8; j++){
			val += x & 0x1010101;
			x >>= 1;
		}

		val += (val >> 16);
		val += (val >> 8);
		result += (val & 0xFF);
	}
	
	return result;
}

int popcount5(unsigned* array, int len){
	int i, val=0, result=0;
	int SSE_mask[] = {0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f};
	int SSE_LUTb[] = {0x02010100, 0x03020201, 0x03020201, 0x04030302};
	
	if (len & 0x3){
		printf("Leyendo 128 bits pero len no multiplo de 4?\n");
	}
	
	for (i = 0; i < len; i += 4){
		asm (
			"movdqu     %[x], %%xmm0 \n\t"
			"movdqa   %%xmm0, %%xmm1 \n\t" // dos copias de x
			"movdqu     %[m], %%xmm6 \n\t" // mascara
			"psrlw       $4 , %%xmm1 \n\t"
			"pand     %%xmm6, %%xmm0 \n\t" //; xmm0 â€“ nibbles inferiores
			"pand     %%xmm6, %%xmm1 \n\t" //; xmm1 â€“ nibbles superiores
			
			"movdqu     %[l], %%xmm2 \n\t" //; ...como pshufb sobrescribe LUT
			"movdqa   %%xmm2, %%xmm3 \n\t" //; ...queremos 2 copias
			"pshufb   %%xmm0, %%xmm2 \n\t" //; xmm2 = vector popcount inferiores
			"pshufb   %%xmm1, %%xmm3 \n\t" //; xmm3 = vector popcount superiores
			
			"paddb    %%xmm2, %%xmm3 \n\t" //; xmm3 -vector popcount bytes
			"pxor     %%xmm0, %%xmm0 \n\t" //; xmm0 =0,0,0,0
			"psadbw   %%xmm0, %%xmm3 \n\t" //; xmm3 =[pcnt bytes0..7|pcnt bytes8..15]
			"movhlps  %%xmm3, %%xmm0 \n\t" //; xmm0 = [ 0 |pcnt bytes0..7 ]
			"paddd    %%xmm3, %%xmm0 \n\t" //; xmm0 = [ no usado |pcnt bytes0..15]
			"movd     %%xmm0, %[val] \n\t"
			:	[val]	"=r" (val)
			:	[x]	"m" (array[i]),
				[m]	"m" (SSE_mask[0]),
				[l]	"m" (SSE_LUTb[0])
		);
		
		result +=val;
	}
	
	return result;
}

// Versión SSE4. 2 (popcount)
int popcount6(unsigned* array, int len){
	int i;
	unsigned x;
	int val, result = 0;

	for (i = 0; i < len; i++){
		x = array[i];
		
		asm(
			"popcnt %[x], %[val]"
			:	[val] "=r" (val)
			:	[x] "r" (x)
		);
		
		result+=val;
	}
	
	return result;
}

// popcount 64bit p/mejorar prestaciones
int popcount7(unsigned* array, int len){
	int i;
	unsigned x1,x2;
	int val, result = 0;
	
	if (len & 0x1)
		printf( "leer 64b y len impar?\n");
	
	for (i=0; i<len; i+=2){
		x1 = array[i]; x2 = array[i+1];
		
		asm(
			"popcnt %[x1], %[val] \n\t"
			"popcnt %[x2], %%edi \n\t"
			"add %%edi, %[val] \n\t"
			: 	[val] "=&r" (val)
			:	[x1] "r" (x1),
				[x2] "r" (x2)
			: "edi"
		);
		
		result += val;
	}
	
	return result;
}

void crono(int (*func)(), char* msg){
    struct timeval tv1,tv2;	// gettimeofday() secs-usecs
    long           tv_usecs;	// y sus cuentas

    gettimeofday(&tv1,NULL);
    resultado = func(lista, SIZE);
    gettimeofday(&tv2,NULL);

    tv_usecs=(tv2.tv_sec -tv1.tv_sec )*1E6+
             (tv2.tv_usec-tv1.tv_usec);
    printf("resultado = %d\t", resultado);
    printf("%s:%9ld us\n", msg, tv_usecs);
}

int main(){
    int i;			// inicializar array
	
	for (i = 0; i < SIZE; i++)	// se queda en cache
		lista[i] = i;

    crono(popcount1, "popcount1");
    crono(popcount2, "popcount2");
    crono(popcount3, "popcount3");
	crono(popcount4, "popcount4");
	crono(popcount5, "popcount5");
	crono(popcount6, "popcount6");
	crono(popcount7, "popcount7");
   // printf("N*(N+1)/2 = %d\n", (SIZE-1)*(SIZE/2)); /*OF*/

    exit(0);
}
