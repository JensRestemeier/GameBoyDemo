; =============================================================================
; Address screen
; (C) by Jens Ch. Restemeier, <XXXXXXXXXXXX>
; =============================================================================

drw_adr:
                ; hide cursor
                ld      hl,0c000h       ; sprite-table
                ld      a, 0            ; Y
                ld      (hl),a

                call    display_off
                call    default_init

                ; init color-palette for GB-Color
                ld      a,0
                ld      hl,gray_pal
                call    set_bg_pal
                ld      a,1
                ld      hl,red_pal
                call    set_bg_pal

                ld      a,0
                ld      hl,red_pal
                call    set_ob_pal

                ldh     a, (EN_COLOR & 0FFh)
                or      a
                call    z, set_gray

                ld      hl, 09800h
                ld      b,20-4
                ld      c,18-4
                call    draw_border

                ld      hl, adr_title
                ld      de, 09800h + 6 + (2*32)
                call    copy_z

                ld      hl, adr_nam1
                ld      de, 09800h + 2 + (5*32)
                call    copy_z

                ld      hl, adr_nam2
                ld      de, 09800h + 4 + (6*32)
                call    copy_z

                ld      hl, adr_street
                ld      de, 09800h + 2 + (7*32)
                call    copy_z

                ld      hl, adr_town
                ld      de, 09800h + 2 + (9*32)
                call    copy_z

                ld      hl, adr_country
                ld      de, 09800h + 2 + (10*32)
                call    copy_z

                ld      hl, adr_email
                ld      de, 09800h + 2 + (12*32)
                call    copy_z

                ld      hl, adr_print
                ld      de, 09800h + 2 + (14*32)
                call    copy_z

                ld      hl, gen_exit
                ld      de, 09800h + 2 + (15*32)
                call    copy_z

                ldh     a,(EN_COLOR & 0FFh)
                or      a
                jr      z, ad_no_col
                ; init CGB palettes
                ld      hl, 09800h
                ld      b, 20
                ld      c, 18
                ld      d,2             ; set to palette 1
                call    set_atr_box

                ld      hl, 09800h + 1 + (3*32)
                ld      b, 18
                ld      c, 14
                ld      d,0             ; set to palette 0
                call    set_atr_box

                ld      hl, 09800h + 1 + (2*32)
                ld      b, 18
                ld      c, 1
                ld      d, 1             ; set to palette 2
                call    set_atr_box
ad_no_col:

                call    display_on

                ld      a,5
                ldh     (SCREEN & 0FFh),a
                ret

; menu-item: exit
s1_adr:
                ; show cursor
                ld      hl,0c000h       ; sprite-table
                ld      a, 16+120       ; Y
                ldi     (hl),a
                ld      a, 8+6          ; X
                ldi     (hl),a
                ld      a, 0            ; Pattern nr.
                ldi     (hl),a
                ld      a, 01000000b    ; attributes
                ldi     (hl),a

                ld      a,21
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0FFh)
                and     044h            ; if "select" or up
                ret     nz

                ld      a,0
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0FFh)
                and     00Bh            ; if "start", 'A' or 'B'
                ret     nz

                ld      a,5             ; otherwise stay at this state
                ldh     (SCREEN & 0FFh),a
                ret

; menu-item: print
s2_adr:
                ; show cursor
                ld      hl,0c000h       ; sprite-table
                ld      a, 16+112       ; Y
                ldi     (hl),a
                ld      a, 8+6          ; X
                ldi     (hl),a
                ld      a, 0            ; Pattern nr.
                ldi     (hl),a
                ld      a, 01000000b    ; attributes
                ldi     (hl),a

                ld      a,0
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0FFh)
                and     002h            ; if 'B'
                ret     nz

                ld      a,5
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0FFh)
                and     084h            ; if "select" or down
                ret     nz

                ld      a,0             ; print...
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0FFh)
                and     009h            ; if "start", 'A'
                call    nz, print_address

                ld      a,21            ; otherwise stay at this state
                ldh     (SCREEN & 0FFh),a
                ret

print_address:
                ld      hl, adr_text
                call    prn_prtbuf
                ret
                ; Yes, there should be some error-handling...
