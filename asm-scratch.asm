
    ; expr_codegen(e->left);
    ; expr_codegen(e->right);

	; x ^ y

	cmp	0x0	y	; Check if y is negative
	js	.return_zero
	
	mov 0x0 %rdx
	mov	0x1	%rax
 
.loop
	dec	y
	jz .exit
	mul	y
	jmp loop

.return_zero
	mov	0x0	%rax
.exit
	; e->reg = e->left->reg
	mov	%rax	x

	; scratch_free(e->righ->reg)