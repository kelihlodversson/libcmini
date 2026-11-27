/* Standard pTOS ELF entry. This note describes the CRT, not the launch role. */
	.pushsection .note.ptos.entry,"a",%note
	.balign 4
	.long 5, 4, 0x50544f53
	.asciz "pTOS"
	.balign 4
	.long 1
	.popsection
