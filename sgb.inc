; Super gameboy utility functions

; Is a SGB present ?
check_sgb:
                call    clear_packet
                ld      a,010001001b
                ld      (sgb_packet),a
                ld      a,000000011b
                ld      (sgb_packet+1),a
                call    send_packet
                ld      a, 030h
                ldh     (P1 & 0FFh), a
                ldh     a, (P1 & 0FFh)
                ld      a, 000h
                ldh     (P1 & 0FFh), a
                ld      a, 030h
                ldh     (P1 & 0FFh), a
                ldh     a, (P1 & 0FFh)
                ret

clear_packet:
                ld      hl, sgb_packet
                xor     a
                ld      b, 16
cp_l1:          ldi     (hl),a
                dec     b
                jr      nz, cp_l1
                ret

send_packet:
                ld      c, 0
                ld      a, 000h
                ld      (c), a
                ld      d, 16
                ld      hl, sgb_packet
sp_l3:          ld      b, 8
                ldi     a, (hl)
                ld      e, a
sp_l2:          ld      a, 010h
                rr      e
                jr      c, sp_l1
                ld      a, 020h
sp_l1:          ld      (c), a
                ld      a, 030h
                ld      (c),a
                dec     b
                jr      nz, sp_l2
                dec     d
                jr      nz, sp_l3
                ld      a, 020h
                ld      (c), a
                ld      a, 030h
                ld      (c),a
                ret
