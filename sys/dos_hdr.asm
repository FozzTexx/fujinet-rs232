DGROUP  group   _SYS_HEADER, _INIT

_INIT   segment word public 'INIT'
        
        public  _transient_data
        public  _small_code_

_transient_data label near

_small_code_    dw      ?

_INIT   ends

_SYS_HEADER segment word public 'SYS_HEADER'

        org     0

_sys_hdr_ label near

	extrn	Strategy_:near
	extrn	Interrupt_:near

	dd	-1
	dw	08003h
	dw	Strategy_
	dw	Interrupt_
	db	'FUJI    '

_SYS_HEADER ends

        end     _sys_hdr_
