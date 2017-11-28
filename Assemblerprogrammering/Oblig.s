  	.globl sprinter
	.data

# Manhal Al_khayyat
# Oblig3 INF2270

# mangler (%x).
	
teller:		.long 0
arg:		.long 0
u_nr:		.long 10
x_nr:		.long 16
	
sprinter:
	pushl	%ebp		#setter opp stakk
	movl	%esp, %ebp	#
	
 	movl	$0, %eax	# nullstiller %eax
	movl	$0, %ebx	# nullstiller %ebx
	movl	$0, %ecx	# nullstiller %ecx , teller
	movl	$0, %edx	# nullstiller %edx
	
	movl	$16, arg	# starter med 16 på arg
		
	movl	8(%ebp), %edi	# første parameter, res
 	movl	12(%ebp), %esi	# andre parameter, format

loop:
	movb	(%esi), %al	# legger char fra string til %al
	incl	%esi		# flytter peker

	cmpb	$0, %al		# if al == 0
	je 	slutt		# hopp over til slutt

	cmpb	$'%', %al	# if al == %
 	je 	check		# hopp over til check
	
 	movb	%al, (%edi)	# legger byten fra %al i %edi
 	incl	%edi		# øke %edi
	incl	%ecx		# øker teller
	jmp	loop		# hopp over til loop

check:
 	movb	(%esi), %al	# legger char fra string til %al
	incl	%esi		# flytter peker
	
	cmpb	$'%', %al	# if al == %
 	je	add_p		# hopp over til add_%
	
 	cmpb	$'c', %al	# if al == c
	je	add_c		# hopp over til add_c
	
 	cmpb	$'d', %al	# if al == d
	je	add_d		# hopp over til add_d
	
 	cmpb	$'s', %al	# if al == s
	je	add_s		# hopp over til add_s

 	cmpb	$'u', %al	# if al == u
	je	add_u		# hopp over til add_u

 	cmpb	$'x', %al	# if al == x
 	je	add_x		# hopp over til add_x

add_p:
 	movb	%al, (%edi)	# flytter %al to %edi
   	incl	%edi		# flytter peker
	incl	%ecx		# øker teller
	jmp	loop		# hopp over til loop

add_c:
	movl	%ebp, %ebx	# kopi i ebx
	addl	arg, %ebx	# ny peker til ebx
	addl	$4, arg		# vi øker med4 får å hente neste
	movb	(%ebx), %al	# kopiere byt	
	movb	%al, (%edi)
	incl	%edi		# flytter peker
	incl	%ecx		# øker teller
	jmp	loop		# hopp over til loop

add_d:	
	movl	%ebp, %ebx	# kopi i ebx
	addl	arg, %ebx	# ny peker til ebx
	movl	(%ebx), %eax	# kopiere byt til %eax
	addl	$4, arg		# vi øker med4 får å hente neste
	incl 	%ebx		
	testl	%eax, %eax	# teste %eax (negativ,positiv eller 0)
	jns	add_uOGu	# hvis positiv
	negl	%eax		# fra positiv til negativ
	incl	%ecx		# øker teller
	movb	$45, (%edi)	# legg minus till
	incl	%edi		# flytter peker
	jmp	add_uOGu	# hopp over til add_uOGu for å behandle resten som unsigned positiv int

add_s:

 	movl	%ebp, %ebx	# kopi i ebx
 	addl	arg, %ebx	# ny peker til ebx
 	movl	(%ebx), %ebx	# kopiere byt
	addl	$4, arg		# vi øker med4 får å hente neste
 	jmp	add_sOGs	# hopp over til add_sOGs
	
add_sOGs:
	movb	(%ebx), %al	# kopi ebx til al
	incl	%ebx		# flytte peker
	cmpb	$0, %al		# sammenligne med 0 byte
	jz	loop		# hopp over til loop
	movb	%al, (%edi)	# flytte byten til %edi
	incl	%edi		# flytter peker
	incl	%ecx		# øker teller
	jmp	add_sOGs	# hopp over til add_sOGs
	
add_u:
	movl	%ebp, %ebx	# kopi i ebx
	addl	arg, %ebx	# ny peker til ebx
	movl	(%ebx), %eax	# kopiere byt
	addl	$4, arg		# vi øker med4 får å hente neste
	jmp	add_uOGu	# hopp over til add_uOGu
	
add_uOGu:
	movl	$0, %edx	# %edx = 0
	divl	u_nr		# deler med 10
	addl	$48, %edx 	# konvertere til ascii
	pushl	%edx		#
	incl	teller		# øker teller
	cmpl	u_nr, %eax	# sjekke om %eax større eller mindre en ti
	jge	add_uOGu	# hopp over til add_uOGu
	cmpl	$0, %eax	# %eax = 0
	je 	u_ferdig	# 
	addl	$48, %eax	# konvertere til ascii
	pushl	%eax		# 
	incl	teller		# øker teller
	jmp	u_ferdig	# 

u_ferdig:
	popl 	%eax		# fra stack
	movb 	%al, (%edi)	# kopiere %al in %edi
	decl	teller		# øker teller
	incl	%ecx		# øker teller
	incl	%edi		# flytter peker
	cmpb 	$0, teller	# teller=0
	je 	loop		# hopp over til loop
	jne 	u_ferdig	# 

add_x:
#	movl	%ebp, %ebx	# kopi i ebx
#	addl	arg, %ebx	# ny peker til ebx
#	movl	(%ebx), %eax	# kopiere byt
#	addl	$4, arg		#  vi øker med4 får å hente neste
#	jmp	add_xOGx	# hopp over til add_xOGx
	
#add_xOGx:	
#	movl	$0, %edx
#	divl	x_nr
#	pushl	%edx
#	incl	teller
#	cmpl	x_nr, %eax
#	jge	add_xOGx
	jmp	loop		# hopp over til loop

slutt:
	movb	$0, (%edi)	# nullstill resultate
	movl	%ecx, %eax	# flytte teller inn i %eax
 	popl	%ebp		#
	ret			# slutt
	
