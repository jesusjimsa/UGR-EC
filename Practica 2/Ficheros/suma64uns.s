.section .data
	lista:		.int 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000,0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x10000000, 0x20000000, 0x40000000,0x80000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x10000000,0x20000000, 0x40000000, 0x80000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000
	longlista:	.int (.-lista)/4
	resultado:	.quad 0x0123456789ABCDEF

.section .text
_start:	.global _start

	mov $lista, %ebx		#Dirección de lista
	mov longlista, %ecx		#Número de elementos
	call suma				#Llamar a suma
	mov %eax, resultado		#Guardar el resultado
	mov %eax, resultado+4

	#Llamada a write

	mov $4, %eax
	mov $1, %ebx
	mov resultado, %ecx
	mov $8, %esi
	int $0x80

	mov $1, %eax			#exit: servicio 1 kernel Linux
	mov $0, %ebx			#status
	int $0x80				#llamar a exit

suma:
	push %esi

	#Se ponen a 0 los siguientes registros

	mov $0, %eax
	mov $0, %edx
	xor %edx, %edx
	xor %edi, %edi

bucle:
	add (%ebx,%esi,4), %eax
	adc $0, %edx			#Acumular el i-ésimo elemento
	inc	%esi				#Incrementar el índice
	cmp %esi,%ecx			#Se compara con el tamaño de la lista
	jne bucle				#Si no son iguales se hace bucle

	pop %esi
	ret
