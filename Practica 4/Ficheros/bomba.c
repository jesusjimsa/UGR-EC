/*
	Compilar con -m32 para que sea más fácil
	de depurar.
*/

#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <string.h>	// para strncmp()/strlen()
#include <sys/time.h>	// para gettimeofday(), struct timeval

char password[] = "~{x\206x\202";	//oliwis encriptado
int  passcode = 1942;

void boom(){
	printf("\n");
	printf("****************************************************\n");
	printf("****** Su ubicacion ha sido enviada a la CIA *******\n");
	printf("*** Un equipo de élite se dirige a neutralizarle ***\n");
	printf("****************************************************\n");
	printf("\n");
	exit(-1);
}

void defused(){
	printf("\n");
	printf("*************************\n");
	printf("***** Por los pelos *****\n");
	printf("*************************\n");
	printf("\n");
	exit(0);
}

void encriptar(char *password){		//Encriptar contraseña introducida
	int i;
	
	for(i = 0; i < strlen(password); i++){
		password[i] = password[i] + 15;
	}
}

/*
int encrypt_num(int pass){			//Encriptar código introducido
	pass = pass * 5;
	pass += 666;
	
	return pass;
}
*/

int main(){
#define SIZE 100
	char pass[SIZE];
	int  pasv;
#define TLIM 60
	struct timeval tv1,tv2;	// gettimeofday() secs-usecs
	
	gettimeofday(&tv1,NULL);
	
	printf("Introduce la contraseña: ");
	fgets(pass, SIZE, stdin);		//Para tomar una cadena
					//Toma la cadena de stdin, con tamaño SIZE y la guarda en pass
	
	encriptar(pass);
	
	if (strncmp(pass, password, strlen(password))){	//Comparar dos cadenas (pass y password) y su longitud
	    boom();
	}
	
	gettimeofday(&tv2, NULL);
	
	if (tv2.tv_sec - tv1.tv_sec > TLIM){
	    boom();
	}
	
	printf("Introduce el código: ");
	scanf("%i", &pasv);		//Recibir entrada, un número del teclado en este caso
	
	//encrypt_num(pasv);
	
	if (pasv != passcode){
		boom();
	}
	
	gettimeofday(&tv1, NULL);
	
	if (tv1.tv_sec - tv2.tv_sec > TLIM){		//Si tardas más de 5 segundos también explota la bomba
	    boom();
	}
	
	defused();		//Si pasa todos los filtros la bomba se desactiva
}
