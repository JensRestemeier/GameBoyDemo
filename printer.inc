; GameBoy printer access
; thanks to "ClapOn/ClapOff" I know how to program the printer...

; send byte and wait for result
send_byte:
                ldh     (SB & 0FFh),a  
                ld      a,081h          ; start transfer
                ldh     (SC & 0FFh),a  
sb_l1:
                ldh     a,(SC & 0FFh)   ; check status
                and     080h
                jr      nz, sb_l1

                ldh     a,(SB & 0FFh)
                ret

; open packet and clear CRC
prn_open:
                xor     a                       ; clear CRC
                ldh     (crc & 0FFh), a
                ldh     ((crc+1) & 0FFh), a
                ld      a, 088h                 ; access printer
                call    send_byte
                ld      a, 033h
                call    send_byte
                ret

; write CRC and get status
prn_close:
                ldh     a,(crc & 0FFh)
                call    send_byte
                ldh     a,((crc+1) & 0FFh)
                call    send_byte
                xor     a
                call    send_byte
                ldh     (stat_1 & 0FFh),a
                xor     a
                call    send_byte
                ldh     (stat_2 & 0FFh),a
                ret
        
; send "bc" bytes starting from "hl" to printer and update CRC
prn_send:
                ldi     a,(hl)          ; get byte from string
                ld      e,a             ; add to CRC
                ldh     a,(crc & 0FFh)
                add     a,e
                ldh     (crc & 0FFh),a
                ldh     a,((crc+1) & 0FFh)
                adc     a,0
                ldh     ((crc+1) & 0FFh),a
                ld      a,e
                call    send_byte
                dec     bc
                ld      a,b
                or      c
                jr      nz, prn_send
                ret

; Prepare printer for printing
prn_init:
                call    prn_open
                ld      hl, cmd_init
                ld      bc, 4
                call    prn_send
                call    prn_close
                ret

; Tell the printer to print the buffer
prn_start:
                call    prn_open
                ld      hl, cmd_start
                ld      bc, 8
                call    prn_send
                call    prn_close
                ret

; Ask printer for status
prn_status:
                call    prn_open
                ld      hl, cmd_status
                ld      bc, 4
                call    prn_send
                call    prn_close
                ret

; Send last packet of data to printer
prn_last:
                call    prn_open
                ld      hl, cmd_eod
                ld      bc, 4
                call    prn_send
                call    prn_close
                ret

; send a memory-buffer to the printer and print it, hl=32*18 memory buffer
prn_prtbuf:
                push    hl

                ; first display the status-window
                ld      hl, 09C00h
                ld      b,20-4
                ld      c,6-4
                call    draw_border

                ld      a, 7
                ldh     (WX & 0FFh),a
                ld      a, 96
                ldh     (WY & 0FFh),a
                ldh     a,(LCDC & 0FFh)
                set     5,a
                res     1,a
                ldh     (LCDC & 0FFh),a

                ld      hl, prn_con
                ld      de, 09C00h + 1 + (2*32)
                call    copy_z

                ld      hl, prn_none
                ld      de, 09C00h + 1 + (3*32)
                call    copy_z

                call    prn_init
                pop     hl

                ldh     a,(stat_1 & 0FFh) ; check if the printer responded
                and     0f0h
                cp      080h
                jp      nz, no_ack

                ldh     a,(stat_2 & 0FFh) ; exit, if an error is found
                and     0f0h
                jp      nz, p_error

                ld      c,1
pp_l2:          push    bc

                push    hl
                ; display status-bar
                ld      b,7
                ld      hl, 09C00h + 1 + (3*32)
pp_l4:
                ldh     a,(STAT & 0FFh)
		bit     1,a
                jr      nz,pp_l4
                ld      a, 'O'
                ldi     (hl),a
                dec     b
                dec     c
                jr      nz, pp_l4
pp_l5:
                ldh     a,(STAT & 0FFh)
		bit     1,a
                jr      nz,pp_l5
                ld      a, '-'
                ldi     (hl),a
                dec     b
                jr      nz, pp_l5

                call    prn_open
                ld      hl,cmd_2row
                ld      bc,4
                call    prn_send
                pop     hl

                ld      b,2
pp_l3:          ld      c,20
pp_l1:          push    bc
                ldi     a,(hl)
                push    hl
                ld      l,a
                ld      h,0
                add     hl,hl   ; hl*=16
                add     hl,hl
                add     hl,hl
                add     hl,hl
                ld      de, text_font
                add     hl,de
                ld      bc,16
                call    prn_send
                pop     hl
                pop     bc
                dec     c
                jr      nz,pp_l1
                ld      de,12
                add     hl,de
                dec     b
                jr      nz,pp_l3
                push    hl
                call    prn_close
                pop     hl
                pop     bc

                ldh     a,(stat_1 & 0FFh) ; check if the printer responded
                and     0f0h
                cp      080h
                jp      nz, no_ack

                ldh     a,(stat_2 & 0FFh) ; exit, if an error is found
                and     0f0h
                jp      nz, p_error

                inc     c
                ld      a,c
                cp      7
                jr      nz,pp_l2

                call    prn_last
                call    prn_start
     
done_print:     ; hide status-window
                ldh     a,(LCDC & 0FFh)
                res     5,a
                set     1,a
                ldh     (LCDC & 0FFh),a

                ret

; error condition:
no_ack:
                ld      hl, prn_link
                ld      de, 09C00h + 1 + (3*32)
                call    copy_z

                call    wait_key

                jp      done_print

p_error:
                ld      de, 09C00h + 1 + (3*32)
                ld      hl, prn_bat
                cp      080h
                call    nz, copy_z
                ld      hl, prn_temp
                cp      040h
                call    nz, copy_z
                ld      hl, prn_jam
                cp      020h
                call    nz, copy_z

                call    wait_key

                jp      done_print


; A few codes:
cmd_init:
.byte  001h     ; Print Init Command
.byte  000h
.word  0

cmd_eod:
.byte  004h     ; Data Packet Command
.byte  000h
.word  0

cmd_status:
.byte  00fh     ; Status Check Command
.byte  000h
.word  0

cmd_start:
.byte  002h     ; Start Print Command
.byte  000h
.word  4        ; 4 bytes data
.byte  001h
.byte  013h     
.byte  0e4h     
.byte  040h     

cmd_2row:
.byte   004h    ; Data Packet Command
.byte   000h
.word   640     ; 640 bytes data
