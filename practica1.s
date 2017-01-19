.data
msg:	.string "Hola mundo!\n"
tam: .quad . - msg

.text
	.global _start

_start:

write:	movl $1, %rax
		movl $1, %rdi
		movl $msg, %rsi
		movl tam, %rdx
		syscall

exit:	movl %rax, %rdi
		movl $60, %rax
		syscall