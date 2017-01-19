//  según la versión de gcc y opciones de optimización usadas, tal vez haga falta
//  usar gcc –fno-omit-frame-pointer si gcc quitara el marco pila (%ebp)

#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <sys/time.h>	// para gettimeofday(), struct timeval

#define SIZE 1<<20	// tamaño suficiente para tiempo apreciable
#define WSIZE 8*sizeof(unsigned)
unsigned lista[SIZE];
int resultado = 0;

int paridad1(int* array, int len){
	int resultado = 0;
	int x;
	int i = 0;
	unsigned val = 0;
	
	for (i = 0; i < len; i++){
		x = array[i];
		
		do{
			val ^= (x & 0x1);
		}while(x >>= 1);
		
		resultado += val;
		val = 0;
	}
	
	return resultado;
}

int paridad2(int* array, int len){
	int resultado = 0;
	int i;
	int j;
	unsigned x;
	unsigned val = 0;
	
	for(i = 0; i < len; i++){
		x = array[i];
		
		for(j = 0; j < WSIZE; j++){
			unsigned mask = 0x1 << j;
			
			val ^= (x & mask) != 0;
		}
		
		resultado += val;
		val = 0;
	}
	
	return resultado;
}

int paridad3(int* array, int len){
	int i;
	int x;
	int val = 0;
	int resultado = 0;
	
	for (i = 0; i < len; i++){
		x = array[i];
		
		while(x){
			val ^= x;
			x >>= 1;
		}
		
		resultado += (val & 0x1);
	}
	
	return resultado;
}

int paridad4(int* array, int len){
	unsigned result = 0, val, i;
	unsigned x;
	
	for (i = 0; i < len; i++){
		x = array[i];
		val = 0;
		
		asm("\n"
		"bucle:					\n\t"	//Seguir mientras que x != 0
			"xor %[x], %[val]	\n\t"	//val ^= x realmente solo nos interesa LSB
			"shr $1, %[x]			\n\t"	//x >>= 1
			"jnz bucle			\n\t"	//Saltar si es dintito de 0
			: [val] "+r" (val)			//E/S: entrada 0, salida paridad elemento
			: [x] "r" (x)				//Entrada: valor elemento
		);
		
		result += (val & 1);
		
	}
	
	return result;
}

int paridad5(unsigned* array, int len){
	int i, j;
	int result = 0;
	int x;
	
	for(i = 0; i < len; i++){
		x = array[i];
		
		for(j = 16; j >= 1; j = j / 2){
			x ^= x >> j;
		}
		
		result += (x & 0x1);
	}
	
	return result;
}

int paridad6(unsigned* array, int len){
	int i;
	unsigned x;
	int result = 0;
	
	for (i = 0; i < len; i++){
		x=array[i];
		
		asm("\n"
			"mov %[x], %%edx	\n\t"	//Sacar copia para XOR. Controlar el registro...
			"shr $16, %[x]		\n\t"	//x >>= 1
			"xor %[x], %%edx	\n\t"
			"xor %%dh, %%dl		\n\t"
			"setpo %%dl			\n\t"
			"movzx %%dl, %[x]	\n\t"	//Devolver en 32 bits
			: [x] "+r" (x)				//E/S: entrada valor elemento, salida paridad
			:
			: "edx"						//Clobber
			);
		
		result += x;
	}
	
	return result;
}

/*
// popcount 64bit p/mejorar prestaciones
int popcount7(unsigned* array, int len){
	int i;
	unsigned x1,x2;
	int val,result=0;
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
*/

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
	
	crono(paridad1, "paridad1");
	crono(paridad2, "paridad2");
	crono(paridad3, "paridad3");
	crono(paridad4, "paridad4");
	crono(paridad5, "paridad5");
	crono(paridad6, "paridad6");
	//crono(popcount7, "popcount7");
	// printf("N*(N+1)/2 = %d\n", (SIZE-1)*(SIZE/2)); /*OF*/
	
	exit(0);
}
