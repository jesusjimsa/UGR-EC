# suma.s sumar los elementos de una lista

## SECCIÓN DE DATOS ##

.section .data
lista:
	.int 1,2,10, 1,2,0b10, 1,2,0x10	# ejemplos binario
longlista:
	.int (.-lista)/4	# Contador de posiciones
resultado:
	.int 0x01234567		# Para ver cuándo se modifica cada byte
formato:
	.ascii "suma = %u = %x hex\n\0"	# Formato para printf()

## SECCIÓN DE CÓDIGO ##

.section .text		# PROGRAMA PRINCIPAL
_start: .global _start
main: .global main

	mov $lista, %ebx		# Dirección del array lista
	mov longlista, %ecx		# Número de elementos a sumar
	call suma			# Llamar suma (&lista, longlista)
	mov %eax, resultado		# Salvar resultado

# Escribir resultado en pantalla #

	push resultado
	push resultado
	push $formato
	call printf
	add $12, %esp

# Salir #
	
	mov $1, %eax			# exit
	mov $0, %ebx
	int $0x80			 # llamar _exit(0);

# Suma #

suma:
	push %edx			# Preservar %edx, se usa como índice
	mov $0, %eax			# Poner a 0 acumulador
	mov $0, %edx			# Poner a 0 índice

bucle:
	add (%ebx, %edx, 4), %eax	# Acumular i-esimo elemento
	inc %edx			# Incrementar índice
	cmp %edx, %ecx			# Comparar con longitud
	jne bucle			# Si no son iguales, seguir acumulando

	pop %edx			# Recuperar %edx antiguo
	ret
