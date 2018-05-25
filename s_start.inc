; =============================================================================
; Start screen
; (C) by Jens Ch. Restemeier, <XXXXXXXXXXXX>
; =============================================================================

drw_start:
                call    display_off
                call    default_init

                ; init color-palette for GB-Color
                ld      a,0
                ld      hl,gray_pal
                call    set_bg_pal
                ld      a,1
                ld      hl,red_pal
                call    set_bg_pal
                ld      a,2
                ld      hl,blue_pal
                call    set_bg_pal

                ld      a,0
                ld      hl,gold_pal
                call    set_ob_pal
                ld      a,1
                ld      hl,blue_pal
                call    set_ob_pal

                ldh     a, (EN_COLOR & 0FFh)
                or      a
                call    z, set_gray

                ld      hl, 09800h
                ld      b,20-4
                ld      c,18-4
                call    draw_border
                call    draw_logo
                ld      hl, man_start
                ld      de, 09800h + 2 + (9*32)
                call    copy_z
                ld      hl, man_options
                ld      de, 09800h + 2 + (10*32)
                call    copy_z
                ld      hl, man_contact
                ld      de, 09800h + 2 + (11*32)
                call    copy_z
                ld      hl, man_copy1
                ld      de, 09800h + 3 + (15*32)
                call    copy_z
                ld      hl, man_copy2
                ld      de, 09800h + 5 + (16*32)
                call    copy_z

                ldh     a,(EN_COLOR & 0FFh)
                or      a
                jr      z, st_no_col

                ; init CGB palettes
                ld      hl, 09800h
                ld      b, 20
                ld      c, 18
                ld      d,1             ; set to palette 1
                call    set_atr_box

                ld      hl, 09800h + 1 + (5*32)
                ld      b, 18
                ld      c, 12
                ld      d,0             ; set to palette 0
                call    set_atr_box

                ld      hl, 09800h + 1 + (1*32)
                ld      b, 18
                ld      c, 5
                ld      d, 2             ; set to palette 2
                call    set_atr_box
st_no_col:

                ; init "sparkle" effect
                ld      d, 00ah
                ld      hl,0c004h       ; sprite-table from 2nd sprite
ds1:            xor     a
                ldi     (hl),a
                ldi     (hl),a
                ld      a,d
                and     3
                add     a, 1
                and     3
                ldi     (hl),a
                ld      a, 01000000b
                ldi     (hl),a
                dec     d
                jr      nz, ds1

                ld      a, 1
                ldh     (speed & 0FFh), a

                call    display_on

                ld      a,1
                ldh     (SCREEN & 0FFh),a
                ret

; do sprite-effect on logo
sparkle:
                ldh     a, (speed & 0ffh)
                dec     a
                ldh     (speed & 0ffh), a
                ret     nz
                ld      a,5
                ldh     (speed & 0ffh), a

                ld      d, 00ah
                ld      hl,0c006h       ; sprite-table from 2nd sprite
sl1:            ld      a,(hl)
                dec     a
                ld      (hl),a
                jr      z, sl_rep
sl_next:        inc     hl
                inc     hl
                inc     hl
                inc     hl
                dec     d
                jr      nz, sl1
                ret
sl_rep:
                dec     hl
                dec     hl
                ld      bc, random      ; pick element from random data
                ldh     a, (DIV & 0FFh) 
                add     a, c
                ld      c, a
                ld      a, b
                adc     a, 0
                ld      b, a
                ld      a, (bc)         ; randomize Y, X and priority
                and     00Fh
                add     a, 32
                ldi     (hl), a
                inc     bc
                ld      a, (bc)
                and     07Fh
                add     a, 24
                ldi     (hl), a
                ld      a, 3
                ldi     (hl), a
                inc     bc
                ld      a, (bc)
                and     0E0h            ; random: priority, X/Y flip
                ld      (hl), a
                dec     hl
                jr      sl_next

; menu item: start
s1_start:
                call sparkle

                ; show cursor
                ld      hl,0c000h       ; sprite-table
                ld      a, 16+72        ; Y
                ldi     (hl),a
                ld      a, 8+6          ; X
                ldi     (hl),a
                ld      a, 0            ; Pattern nr.
                ldi     (hl),a
                ld      a, 01000001b    ; attributes
                ldi     (hl),a

                ld      a,2             
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     084h            ; if down or select
                ret     nz
                ld      a,14
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     009h            ; if "start" or 'A'
                ret     nz
                ld      a,1             ; otherwise stay at this state
                ldh     (SCREEN & 0FFh),a
                ret

; menu item: options
s2_start:
                call sparkle

                ; show cursor
                ld      hl,0c000h       ; sprite-table
                ld      a, 16+80        ; Y
                ldi     (hl),a
                ld      a, 8+6          ; X
                ldi     (hl),a
                ld      a, 0            ; Pattern nr.
                ldi     (hl),a
                ld      a, 01000001b    ; attributes
                ldi     (hl),a

                ld      a,1
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     040h            ; if up
                ret     nz
                ld      a,3
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     084h            ; if down or select
                ret     nz
                ld      a,6
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     009h            ; if "start" or 'A'
                ret     nz
                ld      a,2             ; otherwise stay at this state
                ldh     (SCREEN & 0FFh),a
                ret

; menu item: contact me
s3_start:       
                call sparkle

                ; show cursor
                ld      hl,0c000h       ; sprite-table
                ld      a, 16+88        ; Y
                ldi     (hl),a
                ld      a, 8+6          ; X
                ldi     (hl),a
                ld      a, 0            ; Pattern nr.
                ldi     (hl),a
                ld      a, 01000001b    ; attributes
                ldi     (hl),a

                ld      a,2
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     040h            ; if up
                ret     nz
                ld      a,1
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     004h            ; if select
                ret     nz
                ld      a,4
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     009h            ; if "start" or 'A'
                ret     nz
                ld      a,3             ; otherwise stay at this state
                ldh     (SCREEN & 0FFh),a
                ret

; =============================================================================
; draws the title, i.e. upload the logo-font and draw a matrix
; =============================================================================
draw_logo:
                ld      hl, logo_font   ; install the logo-picture
                ld      de, 08800h
                ld      bc, 18*4*8*2    ; size of font
                call    copy_nv
                ld      hl, 09800h + 1 + (1*32)      ; position of logo
                ld      c,18
                ld      b,128
dl_l1:          ld      a,b
                ldi     (hl),a
                inc     b
                inc     b
                inc     b
                inc     b
                dec     c
                ld      a,c
                or      a
                jr      nz,dl_l1
                ld      hl, 09800h + 1 + (2*32)      ; position of logo
                ld      c,18
                ld      b,129
dl_l2:          ld      a,b
                ldi     (hl),a
                inc     b
                inc     b
                inc     b
                inc     b
                dec     c
                ld      a,c
                or      a
                jr      nz,dl_l2
                ld      hl, 09800h + 1 + (3*32)      ; position of logo
                ld      c,18
                ld      b,130
dl_l3:          ld      a,b
                ldi     (hl),a
                inc     b
                inc     b
                inc     b
                inc     b
                dec     c
                ld      a,c
                or      a
                jr      nz,dl_l3
                ld      hl, 09800h + 1 + (4*32)      ; position of logo
                ld      c,18
                ld      b,131
dl_l4:          ld      a,b
                ldi     (hl),a
                inc     b
                inc     b
                inc     b
                inc     b
                dec     c
                ld      a,c
                or      a
                jr      nz,dl_l4
                ret

; = data required for the startup-screen =
#include "logo.inc"
