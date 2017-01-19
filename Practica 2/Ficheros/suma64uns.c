#include <stdio.h>
#include <stdlib.h>

int suma(unsigned* lista, int tam_lista){
	int suma = 0;

	for(int i = 0; i < tam_lista; i++){
		suma += lista[i];
	}

return suma;
}

int main(){
	unsigned lista [] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
	int tam_lista = sizeof(lista) / 4;

	printf("\nLa suma de todos los elementos de la lista es: %d\n", suma(lista, tam_lista));
}
