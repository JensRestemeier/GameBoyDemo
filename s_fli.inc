; =============================================================================
; A scroller, lower area is covered by a "water" effect with mirror
; (C) by Jens Ch. Restemeier, <XXXXXXXXXXXX>
; =============================================================================

drw_fli:
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
                ld      hl,gold_pal
                call    set_ob_pal

                ldh     a, (EN_COLOR & 0FFh)
                or      a
                call    z, set_gray

                ; setup irq handler
                ld      a,001000000b    ; enable FLI IRQ
                ldh     (STAT & 0ffh),a
                ld      a,000000111b    ; enable VBLANK, Timer and FLI IRQ
                ldh     (IE & 0ffh),a

                ; enable 8x16 sprites
                ld      hl, LCDC
                set     2, (hl)

                di
                ld      de, fli_irq
                ld      hl, IRQ_HANDLER
                ld      a, d
                ldi     (hl), a
                ld      (hl), e
                ei

                xor     a
                ldh     (tabidx & 0FFh),a
                ldh     (textpos & 0FFh),a
                ldh     ((textpos+1) & 0FFh),a

                ld      a,(lyc_table)
                ld      (LYC), a

                ld      a, 1
                ldh     (speed & 0FFh),a

                ; display ducky...
                ld      hl,0c000h
                ld      a, 112
                ldi     (hl),a
                ld      a, 100
                ldi     (hl),a
                ld      a, 8
                ldi     (hl),a
                ld      a, 00000000b
                ldi     (hl),a
                ld      a, 112
                ldi     (hl),a
                ld      a, 108
                ldi     (hl),a
                ld      a, 10
                ldi     (hl),a
                ld      a, 00000000b
                ldi     (hl),a
                ld      a, 128
                ldi     (hl),a
                ld      a, 100
                ldi     (hl),a
                ld      a, 12
                ldi     (hl),a
                ld      a, 00000000b
                ldi     (hl),a
                ld      a, 128
                ldi     (hl),a
                ld      a, 108
                ldi     (hl),a
                ld      a, 14
                ldi     (hl),a
                ld      a, 00000000b
                ldi     (hl),a
                ld      a, 112
                ldi     (hl),a
                ld      a, 116
                ldi     (hl),a
                ld      a, 16
                ldi     (hl),a
                ld      a, 00000000b
                ldi     (hl),a
                ld      a, 112
                ldi     (hl),a
                ld      a, 124
                ldi     (hl),a
                ld      a, 18
                ldi     (hl),a
                ld      a, 00000000b
                ldi     (hl),a
                ld      a, 128
                ldi     (hl),a
                ld      a, 116
                ldi     (hl),a
                ld      a, 20
                ldi     (hl),a
                ld      a, 00000000b
                ldi     (hl),a
                ld      a, 128
                ldi     (hl),a
                ld      a, 124
                ldi     (hl),a
                ld      a, 22
                ldi     (hl),a
                ld      a, 00000000b
                ldi     (hl),a

                ld      a, 128
                ldi     (hl),a
                ld      a, 130
                ldi     (hl),a
                ld      a, 4
                ldi     (hl),a
                ld      a, 00000000b
                ldi     (hl),a
                ld      a, 128
                ldi     (hl),a
                ld      a, 138
                ldi     (hl),a
                ld      a, 6
                ldi     (hl),a
                ld      a, 00000000b
                ldi     (hl),a

                call    display_on
                ; first state
                ld      a,15
                ldh     (SCREEN & 0FFh),a
                ret

s1_fli:
                ; check buttons
                ld      a,18
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     009h            ; if Start or 'A'
                ret     nz

                ld      a,17
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     040h            ; if up
                ret     nz

                ld      a,15            ; otherwise stay here
                ldh     (SCREEN & 0FFh),a

                ; Do the scroller "down"
                ldh     a, (speed & 0FFh)
                dec     a
                ldh     (speed & 0FFh),a
                ret     nz
                ld      a, 4
                ldh     (speed & 0FFh),a

                ; scroll position
                ld      hl, textpos
                ld      e,(hl)
                inc     hl
                ld      d,(hl)
                ld      a,e
                cp      (((((e_scroll_1 - s_scroll_1)/32)*8)+40) & 255)
                jr      nz, s1fl2
                ld      a, d
                cp      (((((e_scroll_1 - s_scroll_1)/32)*8)+40) / 256)
                ; if we are at the end of the scroller, stop scrolling
                ret     z

s1fl2:          inc     de
                ld      (hl),d
                dec     hl
                ld      (hl),e

                ld      a,e
                and     007h
                ret     nz

                ; calculate the screen-position into "hl"
                ldh     a, (textpos & 0FFh)
                sra     a       ; a / 8
                sra     a
                sra     a       
                add     a,13    ; invisible area
                and     31      ; wrap-around at 32 lines
                ld      c,a     ; bc*32
                ld      b,0
                sla     c
                rl      b
                sla     c
                rl      b
                sla     c
                rl      b
                sla     c
                rl      b
                sla     c
                rl      b       
                ld      l, c
                ld      a, 098h
                add     a, b
                ld      h,a

                ; calculate the source-address into "de"
                ld      bc, textpos
                ld      a,(bc)
                ld      e,a
                inc     bc
                ld      a,(bc)
                ld      d,a

                sra     d       ; de / 8
                rr      e       
                sra     d
                rr      e
                sra     d
                rr      e

                ld      a,e
                cp      ((e_scroll_1 - s_scroll_1)/32)
                jp      nc, sf_do_fill

                sla     e       ; de * 32
                rl      d
                sla     e
                rl      d
                sla     e
                rl      d
                sla     e
                rl      d
                sla     e
                rl      d
                ; add source of text
                ld      a,s_scroll_1 & 0FFh
                add     a,e
                ld      e,a
                ld      a,s_scroll_1 / 256
                adc     a,d
                ld      d,a

; now copy 20 bytes from de to hl
sf_do_copy: 
                ld      c,20
sf1la:          ldh     a,(STAT & 0FFh)         ; wait for h/vblank
                and     010b
                jr      z, sf1la
sf1lb:          ldh     a,(STAT & 0FFh)         ; wait for h/vblank
                and     010b
                jr      nz, sf1lb
                ld      a,(de)
                ldi     (hl),a
                inc     de
                dec     c
                jr      nz, sf1la
                ret
sf_do_fill:
                ld      c,20
sf2la:          ldh     a,(STAT & 0FFh)         ; wait for h/vblank
                and     010b
                jr      z, sf2la
sf2lb:          ldh     a,(STAT & 0FFh)         ; wait for h/vblank
                and     010b
                jr      nz, sf2lb
                xor     a
                ldi     (hl),a
                inc     de
                dec     c
                jr      nz, sf2la
                ret

s2_fli:
                ; check buttons
                ld      a,18
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     009h            ; if Start or 'A'
                ret     nz

                ld      a,17
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     080h            ; if up
                ret     nz

                ld      a,16            ; otherwise stay here
                ldh     (SCREEN & 0FFh),a

                ; Do the scroller "up"
                ldh     a, (speed & 0FFh)
                dec     a
                ldh     (speed & 0FFh),a
                ret     nz
                ld      a, 4
                ldh     (speed & 0FFh),a

                ; scroll position
                ld      hl, textpos
                ld      e,(hl)
                inc     hl
                ld      d,(hl)
                ld      a,e
                or      a
                jr      nz, s2fl2
                ld      a, d
                or      a
                ; if we are at the beginning of the scroller, stop scrolling
                ret     z

s2fl2:          dec     de
                ld      (hl),d
                dec     hl
                ld      (hl),e

                ld      a,e
                and     007h
                ret     nz

                ; calculate the screen-position into "hl"
                ldh     a, (textpos & 0FFh)
                sra     a       ; a / 8
                sra     a
                sra     a       
                add     a,-1    ; invisible area
                and     31      ; wrap-around at 32 lines
                ld      c,a     ; bc*32
                ld      b,0
                sla     c
                rl      b
                sla     c
                rl      b
                sla     c
                rl      b
                sla     c
                rl      b
                sla     c
                rl      b       
                ld      l, c
                ld      a, 098h
                add     a, b
                ld      h,a

                ; calculate the source-address into "de"
                ld      bc, textpos
                ld      a,(bc)
                ld      e,a
                inc     bc
                ld      a,(bc)
                ld      d,a

                sra     d       ; de / 8
                rr      e       
                sra     d
                rr      e
                sra     d
                rr      e

                ld      a,e
                sub     14
                ld      e,a
                ld      a,d
                sbc     a,0
                ld      d,a

                bit     7,e     ; do we have a negative result ?
                jp      nz, sf_do_fill

                sla     e       ; de * 32
                rl      d
                sla     e
                rl      d
                sla     e
                rl      d
                sla     e
                rl      d
                sla     e
                rl      d
                ; add source of text
                ld      a,s_scroll_1 & 0FFh
                add     a,e
                ld      e,a
                ld      a,s_scroll_1 / 256
                adc     a,d
                ld      d,a

                jp      sf_do_copy

s3_fli:
                ; check buttons
                ld      a,18
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     009h            ; if Start or 'A'
                ret     nz

                ld      a,16
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     040h            ; if up
                ret     nz

                ld      a,15
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     080h            ; if down
                ret     nz

                ld      a,17            ; otherwise stay here
                ldh     (SCREEN & 0FFh),a
                ret

fli_irq:
                ld      hl, bgp_table           ; Set BGP for this line
                ldh     a, (tabidx & 0ffh)
                add     a,l
                ld      l,a
                ld      a,h
                adc     a,0
                ld      h,a
                ld      a,(hl)
                ldh     (BGP & 0FFh), a

                ld      hl, scx_table           ; Set SCX for this line
                ldh     a, (tabidx & 0ffh)
                add     a,l
                ld      l,a
                ld      a,h
                adc     a,0
                ld      h,a
                ld      a,(hl)
                ldh     (SCX & 0FFh), a

                ld      hl, scy_table           ; Set SCY for this line
                ldh     a, (tabidx & 0ffh)
                add     a,l
                ld      l,a
                ld      a,h
                adc     a,0
                ld      h,a
                ldh     a, (textpos & 0FFh)
                add     a,(hl)
                ldh     (SCY & 0FFh), a

                ld      hl, bgc_table           ; set gb-color bg
                ldh     a, (tabidx & 0ffh)
                sla     a                       ; should be <128
                add     a,l
                ld      l,a
                ld      a,h
                adc     a,0
                ld      h,a
                ld      a,010000000b            
                ldh     (BCPS & 0FFh), a
                ldi     a,(hl)
                ldh     (BCPD & 0FFh), a
                ldi     a,(hl)
                ldh     (BCPD & 0FFh), a

                ld      hl, lyc_table           ; configure next FLI
                ldh     a, (tabidx & 0ffh)
                inc     a
                cp      (lyc_table_end-lyc_table)
                jr      nz, fi_j1
                xor     a
fi_j1:          ldh     (tabidx & 0ffh),a
                add     a,l
                ld      l,a
                ld      a,h
                adc     a,0
                ld      h,a
                ld      a,(hl)
                ldh     (LYC & 0FFh), a

                ret

; table with positions of Y-irqs and SCX values
lyc_table:
.byte    0,   1,   3,   5, 95,100,102,104,106,108,110,112,114,116,118,120,122,124,126,128,130,132,134,136,138,140,142,144
lyc_table_end:
scx_table:
.byte    0,   0,   0,   0,  0, -1,  0,  1,  0, -1,  0,  1,  0, -1,  0,  1,  0, -1,  0,  1,  0, -1,  0,  1,  0, -1,  0,  1
scy_table:
.byte    0,   0,   0,   0,  0, -4, -8,-12,-16,-20,-24,-28,-32,-36,-40,-44,-48,-52,-56,-60,-64,-68,-72,-76,-80,-84,-88,-92
bgp_table:
.byte 000h,054h,0A4h,0e4h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h,0e5h
bgc_table:
.word 00000h,0294ah,05294h,07fffh,07c00h,07800h,07400h,07000h,06c00h,06800h,06400h,06000h,05c00h,05800h,05400h,05000h,04c00h,04800h,04400h,04000h,03c00h,03800h,03400h,03000h,02c00h,02800h,0,0
