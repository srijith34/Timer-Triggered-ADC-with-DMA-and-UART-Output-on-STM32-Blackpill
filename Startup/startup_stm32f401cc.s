.syntax unified
.cpu cortex-m4
.thumb

.global _start
.global Reset_Handler

.section .isr_vector
.word 0x20020000
.word Reset_Handler

.section .text
Reset_Handler:
    bl main
Loop:
    b Loop
