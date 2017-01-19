.section .data
lista:		.int 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
longlista:	.int (.-lista)/4
resultado:	.quad 0x0123456789ABCDEF

.section .text
_start:	.global _start

	mov $lista, %ebx		#Dirección de lista
	mov longlista, %ecx		#Número de elementos
	call suma				#Llamar a suma
	mov %eax, resultado		#Guardar el resultado
	mov %eax, resultado+4

	mov $1, %eax			#exit: servicio 1 kernel Linux
	mov $0, %ebx			#status
	int $0x80				#llamar a exit

suma:
	push %ebp

	#Se ponen a 0 los siguientes registros

	mov $0, %eax
	mov $0, %ebp
	mov $0, %edx
	mov $0, %esi
	mov $0, %edi

bucle:
	mov (%ebx,%esi,4), %eax
	cltd
	add %eax, %esi
	adc $edx, %edi			#Acumular el i-ésimo elemento
	inc	%ebp				#Incrementar el índice
	cmp %ebp,%ecx			#Se compara con el tamaño de la lista
	jne bucle				#Si no son iguales se hace bucle

	mov %edi, %edx
	mov %esi, %eax

	idivl %ecx				#Se calcula la media de los N números

	pop %esi
	ret
