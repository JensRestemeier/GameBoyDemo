; =============================================================================
; Options screen
; (C) by Jens Ch. Restemeier, <XXXXXXXXXXXX>
; =============================================================================

drw_opt:
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

                ld      hl, opt_title
                ld      de, 09800h + 6 + (2*32)
                call    copy_z

                ld      hl, opt_sound
                ld      de, 09800h + 2 + (6*32)
                call    copy_z

                ld      hl, opt_color
                ld      de, 09800h + 2 + (7*32)
                call    copy_z

                ld      hl, opt_test
                ld      de, 09800h + 2 + (9*32)
                call    copy_z

                ld      hl, opt_type
                ld      de, 09800h + 2 + (11*32)
                call    copy_z

                ldh     a,(GB_VERSION & 0FFh)
                ld      hl, opt_gb
                cp      CLASSIC_GAMEBOY
                jr      z, ok
                ld      hl, opt_sgb
                cp      SUPER_GAMEBOY
                jr      z, ok
                ld      hl, opt_pgb
                cp      POCKET_GAMEBOY
                jr      z, ok
                ld      hl, opt_cgb
                cp      COLOR_GAMEBOY
                jr      z, ok
                ld      hl, opt_unknown
ok:             ld      de, 09800h + 3 + (12*32)
                call    copy_z

                ld      hl, gen_exit
                ld      de, 09800h + 2 + (15*32)
                call    copy_z

                ldh     a,(EN_COLOR & 0FFh)
                or      a
                jr      z, op_no_col
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
op_no_col:

                call    display_on

                call    rdrw_opt

                ld      a,7
                ldh     (SCREEN & 0FFh),a
                ret

; menu item: sound
s1_opt:
                ; show cursor
                ld      hl,0c000h       ; sprite-table
                ld      a, 16+48        ; Y
                ldi     (hl),a
                ld      a, 8+6          ; X
                ldi     (hl),a
                ld      a, 0            ; Pattern nr.
                ldi     (hl),a
                ld      a, 01000000b    ; attributes
                ldi     (hl),a

                ld      a,8
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     084h            ; if down or select
                ret     nz
                ld      a,0
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     002h            ; if 'B'
                ret     nz
                ld      a,7             ; otherwise stay at this state
                ldh     (SCREEN & 0FFh),a

                ldh     a,(JOY_TIPP & 0ffh)
                and     020h            ; if right
                jr      nz,snd_off

                ldh     a,(JOY_TIPP & 0ffh)
                and     010h            ; if left
                jr      nz,snd_on

                ldh     a,(JOY_TIPP & 0ffh)
                and     009h            ; if start or 'A'
                ret     z

                ldh     a, (EN_SOUND & 0FFh)
                xor     1
                ldh     (EN_SOUND & 0FFh),a
                call    rdrw_opt
                ret

snd_on:         ld      a,1
                ldh     (EN_SOUND & 0FFh),a
                call    rdrw_opt
                ret

snd_off:        ld      a,0
                ldh     (EN_SOUND & 0FFh),a
                call    rdrw_opt
                ret

; menu item: color
s2_opt:
                ; show cursor
                ld      hl,0c000h       ; sprite-table
                ld      a, 16+56        ; Y
                ldi     (hl),a
                ld      a, 8+6          ; X
                ldi     (hl),a
                ld      a, 0            ; Pattern nr.
                ldi     (hl),a
                ld      a, 01000000b    ; attributes
                ldi     (hl),a

                ld      a,9
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     084h            ; if down or select
                ret     nz
                ld      a,7
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     040h            ; if up
                ret     nz
                ld      a,0
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     002h            ; if 'B'
                ret     nz
                ld      a,8             ; otherwise stay at this state
                ldh     (SCREEN & 0FFh),a

                ldh     a,(HAS_COLOR & 0ffh)
                or      a
                ret     z

                ldh     a,(JOY_TIPP & 0ffh)
                and     020h            ; if right
                jr      nz,clr_off

                ldh     a,(JOY_TIPP & 0ffh)
                and     010h            ; if left
                jr      nz,clr_on

                ldh     a,(JOY_TIPP & 0ffh)
                and     009h            ; if start or 'A'
                ret     z

                ldh     a, (EN_COLOR & 0FFh)
                xor     1
                ldh     (EN_COLOR & 0FFh),a
                call    rdrw_opt
                ret

clr_on:         ld      a,1
                ldh     (EN_COLOR & 0FFh),a
                call    rdrw_opt
                ret

clr_off:        ld      a,0
                ldh     (EN_COLOR & 0FFh),a
                call    rdrw_opt
                ret

; menu item: test
s3_opt:
                ; show cursor
                ld      hl,0c000h       ; sprite-table
                ld      a, 16+72        ; Y
                ldi     (hl),a
                ld      a, 8+6          ; X
                ldi     (hl),a
                ld      a, 0            ; Pattern nr.
                ldi     (hl),a
                ld      a, 01000000b    ; attributes
                ldi     (hl),a

                ld      a,10             
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     084h            ; if down or select
                ret     nz
                ld      a,8
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     040h            ; if up
                ret     nz
                ld      a,0
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     002h            ; if 'B'
                ret     nz
                ld      a,11
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     009h            ; if start or 'A'
                ret     nz
                ld      a,9             ; otherwise stay at this state
                ldh     (SCREEN & 0FFh),a
                ret

; menu item: exit
s4_opt:
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

                ld      a,7
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     004h            ; if select
                ret     nz
                ld      a,9
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     040h            ; if up
                ret     nz
                ld      a,0             
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     00Bh            ; if "start", 'A' or 'B'
                ret     nz
                ld      a,10             ; otherwise stay at this state
                ldh     (SCREEN & 0FFh),a
                ret

rdrw_opt:
                ldh     a,(EN_SOUND & 0FFh)
                ld      hl, gen_on
                ld      de, 09800h + 10 + (6*32)
                or      a
                jr      nz, no_sound
                ld      hl, gen_off
no_sound:       call    copy_z

                ld      de, 09800h + 10 + (7*32)
                ldh     a,(HAS_COLOR & 0FFh)
                ld      hl, gen_na
                or      a
                jr      z, no_color
                ldh     a,(EN_COLOR &0FFh)
                ld      hl, gen_on
                or      a
                jr      nz, no_color
                ld      hl, gen_off
no_color:       call    copy_z
                ret
