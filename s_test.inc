; =============================================================================
; Test screen
; (C) by Jens Ch. Restemeier, <XXXXXXXXXXXX>
; =============================================================================

drw_test:
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

                ldh     a, (EN_COLOR & 0FFh)
                or      a
                call    z, set_gray

                ld      hl, 09800h
                ld      b,20-4
                ld      c,18-4
                call    draw_border

                ld      hl, tst_title
                ld      de, 09800h + 4 + (2*32)
                call    copy_z

                ld      hl, tst_value
                ld      de, 09800h + 2 + (4*32)
                call    copy_z

                ld      hl, tst_up
                ld      de, 09800h + 2 + (6*32)
                call    copy_z
                ld      hl, tst_down
                ld      de, 09800h + 2 + (7*32)
                call    copy_z
                ld      hl, tst_left
                ld      de, 09800h + 2 + (8*32)
                call    copy_z
                ld      hl, tst_right
                ld      de, 09800h + 2 + (9*32)
                call    copy_z
                ld      hl, tst_start
                ld      de, 09800h + 2 + (10*32)
                call    copy_z
                ld      hl, tst_select
                ld      de, 09800h + 2 + (11*32)
                call    copy_z
                ld      hl, tst_a
                ld      de, 09800h + 2 + (12*32)
                call    copy_z
                ld      hl, tst_b
                ld      de, 09800h + 2 + (13*32)
                call    copy_z

                ld      hl, tst_exit
                ld      de, 09800h + 2 + (15*32)
                call    copy_z

                ldh     a,(EN_COLOR & 0FFh)
                or      a
                jr      z, te_no_col
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
te_no_col:


                call    display_on

                ld      a,13
                ldh     (SCREEN & 0FFh),a
                ret

; handle user-input
hnd_test:
                ld      a,6
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_HOLD & 0ffh)
                cp      003h            ; if 'A' + 'B'
                ret     z

                ld      a,13
                ldh     (SCREEN & 0FFh),a

                ld      hl, OLD_JOY_HOLD
                ldh     a,(JOY_HOLD & 0ffh)
                cp      (hl)
                ret     nz

                ld      a,12
                ldh     (SCREEN & 0FFh),a
                ret

; fill in values into screen, wait for h/v retrace !
rdrw_test:
                ; print joy_hold value in HEX
                ldh     a,(JOY_HOLD & 0FFh)
                ld      de, 09800h + 10 + (4*32)
                swap    a
                and     00fh
                ld      h,0
                add     a,hextable & 0FFh
                ld      l,a
                ld      a,(hl)
                ld      (de),a
                inc     de
                ldh     a,(JOY_HOLD & 0FFh)
                and     00fh
                add     a,hextable & 0FFh
                ld      l,a
                ld      a,(hl)
                ld      (de),a

                ; print on/off depending on value
                ldh     a,(JOY_HOLD & 0ffh)
                ld      hl, gen_on
                ld      de, 09800h + 10 + (6*32)
                bit     6,a
                jr      nz, no_up
                ld      hl, gen_off
no_up:          call    copy_z
                ld      hl, gen_on
                ldh     a,(JOY_HOLD & 0ffh)
                ld      de, 09800h + 10 + (7*32)
                bit     7,a
                jr      nz, no_down
                ld      hl, gen_off
no_down:        call    copy_z
                ld      hl, gen_on
                ldh     a,(JOY_HOLD & 0ffh)
                ld      de, 09800h + 10 + (8*32)
                bit     5,a
                jr      nz, no_left
                ld      hl, gen_off
no_left:        call    copy_z
                ld      hl, gen_on
                ldh     a,(JOY_HOLD & 0ffh)
                ld      de, 09800h + 10 + (9*32)
                bit     4,a
                jr      nz, no_right
                ld      hl, gen_off
no_right:       call    copy_z
                ld      hl, gen_on
                ldh     a,(JOY_HOLD & 0ffh)
                ld      de, 09800h + 10 + (10*32)
                bit     3,a
                jr      nz, no_start
                ld      hl, gen_off
no_start:       call    copy_z
                ld      hl, gen_on
                ldh     a,(JOY_HOLD & 0ffh)
                ld      de, 09800h + 10 + (11*32)
                bit     2,a
                jr      nz, no_select
                ld      hl, gen_off
no_select:      call    copy_z
                ld      hl, gen_on
                ldh     a,(JOY_HOLD & 0ffh)
                ld      de, 09800h + 10 + (12*32)
                bit     0,a
                jr      nz, no_a
                ld      hl, gen_off
no_a:           call    copy_z
                ld      hl, gen_on
                ldh     a,(JOY_HOLD & 0ffh)
                ld      de, 09800h + 10 + (13*32)
                bit     1,a
                jr      nz, no_b
                ld      hl, gen_off
no_b:           call    copy_z

                ld      a,12
                ldh     (SCREEN & 0FFh),a
                ret
