; =============================================================================
; A small game. Should be good enough for a job at THQ :-)
; (C) by Jens Ch. Restemeier, <XXXXXXXXXXXX>
; =============================================================================

drw_game:
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
                ld      hl,blue_pal
                call    set_ob_pal
                ld      a,1
                ld      hl,red_pal
                call    set_ob_pal
                ld      a,2
                ld      hl,gold_pal
                call    set_ob_pal

                ldh     a, (EN_COLOR & 0FFh)
                or      a
                call    z, set_gray

                ld      hl, game_font   ; install the game-font
                ld      de, 08800h
                ld      bc, 24*8*2    ; size of font
                call    copy_nv

                ld      hl, text_font   ; install the text-font
                ld      de, 08000h      ; into sprite-memory
                ld      bc, 128*8*2
                call    copy_nv

                ; enable 8x16 sprites
                ld      hl, LCDC
                set     2, (hl)

                ; enable window
                set     5,(hl)

                ld      a,7
                ldh     (WX & 0FFh),a
                ld      a,136
                ldh     (WY & 0FFh),a

                ; place heli
                ld      a,32
                ldh     (heli_x & 0FFh), a
                ld      a,77
                ldh     (heli_y & 0FFh), a

                ld      hl,0c000h       ; sprite-table 
                ldh     a, (heli_y & 0FFh)
                ldi     (hl),a
                ldh     a, (heli_x & 0FFh)
                ldi     (hl),a
                ld      a, 128          ; first frame of heli
                ldi     (hl),a
                ld      a, 000000000b
                ldi     (hl),a

                ; initial scroller-position
                xor     a
                ldh     (xpos & 0FFh),a
                ; zero points
                ldh     (g_points & 0FFh),a
                ldh     (g_points+1 & 0FFh),a

                ; position of hole
                ld      a,5
                ldh     (hpos & 0FFh),a

                ld      a,1
                ldh     (speed & 0FFh), a

                call    display_on

                ld      a,23
                ldh     (SCREEN & 0FFh),a
                ret

; play "game"
s1_game:
                ; evaluate joypad commands
                ld      hl, JOY_HOLD
                ldh     a, (heli_y & 0FFh)
                bit     6, (hl) ; up
                jr      z, g1_l5
                cp      28
                jr      z, g1_l6
                dec     a
g1_l5:          bit     7, (hl) ; down
                jr      z, g1_l6
                cp      140
                jr      z, g1_l6
                inc     a
g1_l6:          ldh     (heli_y & 0FFh), a
                ldh     a,(heli_x & 0FFh)
                bit     5, (hl) ; left
                jr      z, g1_l7
                cp      12
                jr      z,g1_l8
                dec     a
g1_l7:          bit     4, (hl) ; right
                jr      z, g1_l8
                cp      148
                jr      z,g1_l8
                inc     a
g1_l8:          ldh     (heli_x & 0FFh),a

                ld      a,26
                ldh     (SCREEN & 0FFh),a
                ldh     a, (JOY_TIPP & 0FFh) 
                and     008h                    ; if start
                ret     nz
                ld      a,23
                ldh     (SCREEN & 0FFh),a

                ldh     a, (JOY_TIPP & 0FFh) 
                and     003h                       ; if "A" or "B"
                jr      z,g1_l9
                ; find unallocated sprite and make it a bullet
                ld      c, 5            ; maximum 5 sprites
                ld      hl,0c008h       ; sprite-table from 3rd sprite
g1_l11:         ld      a,(hl)
                or      a
                jp      z, new_sprite
                ld      de,4
                add     hl,de
                dec     c
                jr      nz,g1_l11
g1_l9:
                ; position the heli
                ld      hl,0c000h       ; sprite-table 
                ldh     a, (heli_y & 0FFh)
                ld      d,a
                ldi     (hl),a
                ldh     a, (heli_x & 0FFh)
                ld      e,a
                ldi     (hl),a

                ldh     a,(speed & 0FFh)
                dec     a
                ldh     (speed & 0FFh),a

                jr      nz, g1_no_anim

                ld      a,5
                ldh     (speed & 0FFh),a
                ld      a, (hl)
                add     a,4
                and     08Ch
                ld      (hl),a
g1_no_anim:     inc     hl
                ld      a, 000000000b
                ldi     (hl),a

                ld      a, (0c000h)
                ldi     (hl),a
                ld      a, (0c001h)
                add     a, 8
                ldi     (hl),a
                ld      a, (0c002h)
                add     a, 2
                ldi     (hl),a
                ld      a, 000000000b
                ldi     (hl),a

                ; collision-detect heli, "hot" spot x = x+xpos, y = y-8
                ld      a,d
                sub     8
                ld      d,a

                ldh     a,(xpos & 0FFh) ; scroll-position must be added
                add     a,e     
                ld      e,a

                ; position= (d/8)*32 + (e/8)
                srl     e       ; e/=8
                srl     e
                srl     e
                srl     d       ; d/=8
                srl     d
                srl     d

                ld      l,d     ; hl=d*16
                ld      h,0
                add     hl,hl
                add     hl,hl
                add     hl,hl
                add     hl,hl
                add     hl,hl
                ld      a,l     ; hl |= e
                or      e
                ld      l,a
                ld      de,09800h
                add     hl,de
                ld      a,(hl)
                or      a
                jr      z, g1_no_coll
                ld      a,24
                ldh     (SCREEN & 0FFh),a
g1_no_coll:
                ld      hl, 0c008h
                ; position, collision-detect and move bullets
                ld      c,5
g1_l13:         ld      d,(hl)
                inc     hl
                ld      e,(hl)
                dec     hl

                ; both x and y are set to 0 for a finished sprite
                ld      a,d
                or      e
                jr      z, g1_done

                ; move sprite
                inc     e
                inc     e

                ; check if tile at position d/e is > 0
                push    de
                push    hl

                ; "hot" spot is at x=x+xpos and y=y-12
                ld      a,d
                sub     12
                ld      d,a

                ldh     a,(xpos & 0FFh) ; scroll-position must be added
                add     a,e     
                ld      e,a

                ; position= (d/8)*32 + (e/8)
                srl     e       ; e/=8
                srl     e
                srl     e
                srl     d       ; d/=8
                srl     d
                srl     d

                ld      l,d     ; hl=d*16
                ld      h,0
                add     hl,hl
                add     hl,hl
                add     hl,hl
                add     hl,hl
                add     hl,hl
                ld      a,l     ; hl |= e
                or      e
                ld      l,a
                ld      de,09800h
                add     hl,de
                ld      a,(hl)
                push    af
                xor     a
                ld      (hl),a
                pop     af

                pop     hl
                pop     de
                or      a
                jr      nz, g1_finish

                ld      a,e
                cp      170     ; sprite out of sight ?
                jr      nz, g1_done
g1_finish:      ld      d,0     ; finish sprite
                ld      e,d
g1_done:        ld      (hl),d
                inc     hl
                ld      (hl),e
                inc     hl
                inc     hl      ; skip code
                inc     hl      ; skip attribs
                dec     c
                jr      nz,g1_l13

                ; scroll the background
                ldh     a,(xpos & 0FFh)
                inc     a
                ldh     (xpos & 0FFh),a
                ldh     (SCX & 0FFh),a

                and     7
                ret     nz

                ; increment points
                ldh     a,(g_points & 0FFh)       ; points are stored in BCD
                inc     a
                daa
                ld      e,a
                ldh     (g_points & 0FFh),a
                ldh     a,(g_points+1 & 0FFh)
                adc     a,0
                daa
                ld      d,a
                ldh     (g_points+1 & 0FFh),a

                ld      b, 8    ; 8 digits
                ld      hl, 09c08h
g1_pd:
                ldh     a,(STAT & 0FFh)
		bit     1,a
                jr      nz,g1_pd

                ld      a,e
                and     00Fh
                add     a,'0'
                ldd     (hl),a
                srl     d               ; de/=16
                rr      e
                srl     d
                rr      e
                srl     d
                rr      e
                srl     d
                rr      e
                dec     b
                jr      nz, g1_pd

                ; randomize hpos
                ld      hl, random
                ldh     a, (DIV & 0FFh)
                ld      c, a
                ld      b, 0
                add     hl,bc

                ldh     a, (hpos & 0FFh)
                bit     0, (hl)
                jr      nz, g1_l2
                cp      10
                jr      z, g1_l2
                inc     a
g1_l2:
                bit     1, (hl)
                jr      nz, g1_l3
                cp      2
                jr      z, g1_l3
                dec     a
g1_l3:
                ldh     (hpos & 0FFh), a

                ; calculate first VRAM address
                ldh     a,(xpos & 0FFh)
                srl     a               ; a/=8
                srl     a
                srl     a
                add     a,20            ; outside the visible area
                and     01Fh            ; mask to width of virtual screen

                ; draw a new border with a hole to fly through
                ld      hl,09800h
                ld      b,0
                ld      c,a
                add     hl,bc

                ldh     a,(hpos & 0FFh)
                ld      e,a

g1_l1:
                ldh     a,(STAT & 0FFh)
		bit     1,a
                jr      nz,g1_l1

                ld      a,150   ; upper border
                ld      (hl),a
                ld      bc,32
                add     hl,bc
                dec     e
                jr      nz, g1_l1

                ld      e,5     ; hole
g1_l4:
                ldh     a,(STAT & 0FFh)
		bit     1,a
                jr      nz,g1_l4
                xor     a

                ld      (hl),a
                ld      bc,32
                add     hl,bc
                dec     e
                jr      nz, g1_l4

                ldh     a, (hpos & 0FFh)
                ld      e, a
                ld      a, 13
                sbc     a, e
                ld      e, a

g1_l12:
                ldh     a,(STAT & 0FFh)
		bit     1,a
                jr      nz,g1_l12

                ld      a,150   ; lower border
                ld      (hl),a
                ld      bc,32
                add     hl,bc
                dec     e
                jr      nz, g1_l12

                ret

new_sprite:
                ldh     a, (heli_y & 0FFh)
                add     a,8
                ldi     (hl),a
                ldh     a, (heli_x & 0FFh)
                add     a,8
                and     07eh    ; round to even 
                ldi     (hl),a
                ld      a, 148
                ldi     (hl),a
                ld      a, 000000001b
                ldi     (hl),a
                jp      g1_l9

; draw "Game Over"
s2_game:
                ; disable 8x16 sprites
                ld      hl, LCDC
                res     2, (hl)

                ; "explode" Heli
                ld      a,144
                ld      (0c002h),a
                ld      a,146
                ld      (0c006h),a
                ld      a,145           ; 8x8 mode needs 2 extra sprites
                ld      (0c00Ah),a
                ld      a,147
                ld      (0c00Eh),a
                ld      a,(0c000h)
                add     a,8
                ld      (0c008h),a
                ld      a,(0c004h)
                add     a,8
                ld      (0c00Ch),a
                ld      a,(0c001h)
                ld      (0c009h),a
                ld      a,(0c005h)
                ld      (0c00Dh),a
                ld      a, 000000010b
                ld      (0c003h),a
                ld      (0c007h),a
                ld      (0c00Bh),a
                ld      (0c00Fh),a

                ; print message
                ld      hl, 0c010h
                ld      de, game_over
                ld      c,44
g2_go:          ld      a, 70
                ldi     (hl),a
                ld      a, c
                ldi     (hl),a
                add     a,8
                ld      c,a
                ld      a,(de)
                ldi     (hl),a
                ld      a, 000000001b
                ldi     (hl),a
                ld      a,(de)
                inc     de
                or      a
                jr      nz, g2_go

                ld      a,25
                ldh     (SCREEN & 0FFh),a
                ret

; wait for any key
s3_game:
                ld      a,26
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0FFh)
                and     00Fh
                ret     nz
                ld      a,25
                ldh     (SCREEN & 0FFh),a
                ret

#include "game.inc"
