; =============================================================================
; A clone of the "robot take-over" game in Paradroid
; (C) by Jens Ch. Restemeier, <XXXXXXXXXXXX>
; =============================================================================

drw_para:
                call    display_off
                call    default_init

                ; init color-palette for GB-Color
                ld      a,0
                ld      hl,para_pal
                call    set_bg_pal

                ld      a,0
                ld      hl,para_pal
                call    set_ob_pal

                ldh     a, (EN_COLOR & 0FFh)
                or      a
                call    z, set_gray

                ld      hl, para_font   ; install the paradroid-GFX
                ld      de, 08800h
                ld      bc, 36*8*2    ; size of font
                call    copy_nv

                ; every player can set 5 signals at the beginning:
                ld      a,5
                ldh     (lsig & 0FFh),a
                ldh     (rsig & 0FFh),a

                ; Draw screen

                ; left bar:
                ld      hl, 09800h + 0 + (3*32)
                ld      e, 13
                ld      a, 134
                call    draw_bar

                ; right bar:
                ld      hl, 09800h + 19 + (3*32)
                ld      e, 13
                ld      a, 135
                call    draw_bar

                ; draw totem-pole
                ld      a,148
                call    show_state

                ld      hl, 09800h + 9 + (5*32)
                ld      bc, 30
                ld      e, 12
                ld      a, 144
pd_l3:          ldi     (hl),a
                xor     2
                ldi     (hl),a
                add     hl,bc
                xor     3
                dec     e
                jr      nz, pd_l3

                ld      a,160
                ldi     (hl),a
                inc     a
                ldi     (hl),a

                ; draw circuit

                ; for now the circuit is hardcoded

                ld      a, 142  ; terminators left
                ld      (09800h + 3 + (7*32)),a
                ld      (09800h + 3 + (9*32)),a
                ld      (09800h + 3 + (12*32)),a
                ld      a, 140  ; repeaters left
                ld      (09800h + 7 + (15*32)),a
                ld      a, 162  ; inverters left
                ld      (09800h + 7  + (16*32)),a

                ; logics left
                ld      hl,09800h + 5 + (7*32)
                ld      e, 1
                ld      a, 134
                call    draw_bar

                ld      hl,09800h + 5 + (11*32)
                ld      e, 1
                ld      a, 134
                call    draw_bar

                ; wires left
                ld      a,129
                ld      (09800h + 1 + (4*32)),a ; contact
                ld      e,12
                ld      hl, 09800h + 1 + (5*32)
pp_l6:          push    hl
                call    pl_wire
                pop     hl
                ld      bc,32
                add     hl,bc
                dec     e
                jr      nz,pp_l6

                ld      a, 141  ; terminators right
                ld      (09800h + 16 + (7*32)),a
                ld      (09800h + 14 + (10*32)),a
                ld      (09800h + 14 + (12*32)),a
                ld      a, 143  ; repeaters right
                ld      (09800h + 16 + (11*32)),a
                ld      (09800h + 16 + (16*32)),a
                ld      a, 163  ; inverters right
;FIXME (16)
                ld      (09800h + 14 + (5 *32)),a

                ; logics right
                ld      hl,09800h + 14 + (6*32)
                ld      e, 1
                ld      a, 135
                call    draw_bar

                ld      hl,09800h + 12 + (10*32)
                ld      e, 1
                ld      a, 135
                call    draw_bar

                ; wires right
                ld      a,129
                ld      (09800h + 18 + (4*32)),a ; contact
                ld      e,12
                ld      hl, 09800h + 18 + (5*32)
pp_l7:          push    hl
                call    pr_wire
                pop     hl
                ld      bc,32
                add     hl,bc
                dec     e
                jr      nz,pp_l7

                ; init sprites
                ld      hl, 0c000h
                xor     a               ; left player
                ldi     (hl),a
                ldi     (hl),a
                ld      a, 140
                ldi     (hl),a
                xor     a
                ldi     (hl),a

                xor     a               ; right player
                ldi     (hl),a
                ldi     (hl),a
                ld      a, 143
                ldi     (hl),a
                xor     a
                ldi     (hl),a

                ld      a,1
                ldh     (speed & 0FFh),a

                call    display_on

                ld      a,27
                ldh     (SCREEN & 0FFh),a
                ret

; =============================================================================
s1_para:
                ; animate powered lines
                ld      bc,13
                ld      hl, 08826h
                rrc     (hl)
                inc     hl
                rrc     (hl)
                inc     hl
                rrc     (hl)
                inc     hl
                rrc     (hl)

                add     hl,bc

                rrc     (hl)
                inc     hl
                rrc     (hl)
                inc     hl
                rrc     (hl)
                inc     hl
                rrc     (hl)

                add     hl,bc

                rlc     (hl)
                inc     hl
                rlc     (hl)
                inc     hl
                rlc     (hl)
                inc     hl
                rlc     (hl)

                add     hl,bc

                rlc     (hl)
                inc     hl
                rlc     (hl)
                inc     hl
                rlc     (hl)
                inc     hl
                rlc     (hl)

                ; if a player has no active signal, and signals are left
                ; give him a new one
                ld      hl, 0c000h      ; left player
                ld      a,(hl)
                or      a
                jr      nz, p1_nol
                ldh     a,(lsig & 0FFh)
                or      a
                jr      z, p1_nol
                dec     a
                ldh     (lsig & 0FFh),a

                ld      a, 48           
                ldi     (hl),a
                ld      a, 20
                ld      (hl),a

                ldh     a,(lsig & 0FFh)
                ld      d,a
                or      a
                ld      a,140
                ld      hl,09800h
p1_l1:
                jr      z, p1_nsl
                ldi     (hl),a                
                dec     d
                jr      p1_l1

p1_nsl:         ld      a,0
                ld      (hl),a
p1_nol:
                ld      hl, 0c004h      ; right player
                ld      a,(hl)
                or      a
                jr      nz, p1_nor

                ldh     a,(rsig & 0FFh)
                or      a
                jr      z, p1_nor
                dec     a
                ldh     (rsig & 0FFh),a
                ld      a, 48           
                ldi     (hl),a
                ld      a, 148
                ld      (hl),a
                ldh     a,(rsig & 0FFh)
                ld      d,a
                or      a
                ld      a,143
                ld      hl,09800h+19
p1_l2:
                jr      z, p1_nsr
                ldd     (hl),a                
                dec     d
                jr      p1_l2

p1_nsr:         ld      a,0
                ld      (hl),a
p1_nor:
                ; check, if the player may move his signal
                ld      a,(0c000h)
                or      a
                jr      z,p1_nb

                ; now evaluate joypad commands: up/down, A, B or start
                ldh     a,(JOY_TIPP & 0FFh)
                and     080h     ; down
                jr      z, p1_nd
                ld      a,(0c000h)
                cp      144
                jr      z, p1_nd
                add     a,8
                ld      (0c000h),a
                jr      p1_ns
p1_nd:
                ldh     a,(JOY_TIPP & 0FFh)
                and     040h    ; up
                jr      z, p1_nu
                ld      a,(0c000h)
                cp      48      ; inital position
                jr      z, p1_nu
                cp      56      ; upper position
                jr      z, p1_nu
                sub     8
                ld      (0c000h),a
                jr      p1_ns
p1_nu:
                ldh     a,(JOY_TIPP & 0FFh)
                and     003h    ; button A or B
                jr      z, p1_nb
                ld      a,(0c000h)
                cp      48      ; it's not possible to drop the signal at
                                ; the initial position
                jr      z, p1_nb
                srl     a       ; a/=8 to get Y
                srl     a
                srl     a
                dec     a       ; sprite origin is two lines to high
                dec     a
                ld      l,a     ; hl=a*32
                ld      h,0
                add     hl,hl
                add     hl,hl
                add     hl,hl
                add     hl,hl
                add     hl,hl
                ld      de,09800h+3
                add     hl,de
                ld      a,(hl)
                cp      142   ; terminated line ?
                jr      z, p1_nb
                dec     hl
                ld      a,(hl)
                cp      140     ; already a signal here ?
                jr      z, p1_nb
                ld      a,140   ; place signal  
                ldi     (hl),a
                call    pl_activate
                xor     a       ; delete sprite
                ld      (0c000h),a
p1_nb:
                ldh     a,(JOY_TIPP & 0FFh)
                and     008h     ; start
                jr      z,p1_ns
                ld      a,0
                ldh     (SCREEN & 0FFh), a
                ret
p1_ns:
                ; May player 2 move his sprite ?
                ldh     a, (speed & 0FFh)
                dec     a
                ldh     (speed & 0FFh), a
                jr      nz, p2_nx
                ld      a,5
                ldh     (speed & 0FFh), a
                ld      a,(0c004h)
                or      a
                jr      z,p2_nx

                ; The evaluation for player 2, using random values
                ldh     a,(DIV & 0FFh)
                ld      hl, random
                add     a,l
                ld      l,a
                ld      a,h
                adc     a,0
                ld      h,a
                ld      a,(hl)

                push    af
                bit     0,a
                jr      z, p2_nu

                ld      a,(0c004h)
                cp      48      ; inital position
                jr      z, p2_nu
                cp      56      ; upper position
                jr      z, p2_nu
                sub     8
                ld      (0c004h),a
                jr      p2_ns
p2_nu:
                pop     af
                push    af
                bit     1,a
                jr      z, p2_nd

                ld      a,(0c004h)
                cp      144
                jr      z, p2_nd
                add     a,8
                ld      (0c004h),a
                jr      p2_ns

p2_nd:
                pop     af
                push    af
                bit     2,a
                jr      z, p2_ns

                ld      a,(0c004h)
                cp      48      ; it's not possible to drop the signal at
                                ; the initial position
                jr      z, p2_ns
                srl     a       ; a/=8 to get Y
                srl     a
                srl     a
                dec     a       ; sprite origin is two lines to high
                dec     a
                ld      l,a     ; hl=a*32
                ld      h,0
                add     hl,hl
                add     hl,hl
                add     hl,hl
                add     hl,hl
                add     hl,hl
                ld      de,09800h+16
                add     hl,de
                ld      a,(hl)
                cp      141   ; terminated line ?
                jr      z, p2_ns
                inc     hl
                ld      a,(hl)
                cp      143     ; already a signal here ?
                jr      z, p2_ns
                ld      a,143   ; place signal  
                ldd     (hl),a
                call    pr_activate
                xor     a       ; delete sprite
                ld      (0c004h),a
p2_ns:
                pop     af
p2_nx:
                ; evaluate totem-pole
                ld      hl, 09800h + 8 + (5*32)
                ld      e, 12
pe_l1:
                ldh     a,(STAT & 0FFh)
		bit     1,a
                jr      nz,pe_l1
                ld      bc,0
                ldi     a,(hl)
                cp      130
                jr      nz, pe_l3
                ld      b,1
pe_l3:          cp      131
                jr      nz, pe_l4
                ld      b,2
pe_l4:
                inc     hl
                inc     hl
                ld      a,(hl)
                cp      133
                jr      nz, pe_l5
                ld      c,2
pe_l5:          cp      132
                jr      nz, pe_l7
                ld      c,1

pe_l7:          ld      a,b
                or      c

                cp      2
                jr      nz, pe_l2
                dec     hl
                dec     hl
                ld      a,(hl)
                or      1
                ldi     (hl),a
                ld      a,(hl)
                or      1
                ldi     (hl),a
                jr      pe_l6

pe_l2:
                cp      1
                jr      nz, pe_l6

                dec     hl
                dec     hl
                ld      a,(hl)
                and     0FEh
                ldi     (hl),a
                ld      a,(hl)
                and     0FEh
                ldi     (hl),a
pe_l6:
                cp      3
                jr      nz, pe_l10

                dec     hl
                dec     hl
                ld      a,(hl)
                xor     1
                ldi     (hl),a
                ld      a,(hl)
                xor     1
                ldi     (hl),a
pe_l10:
                ld      bc, 29
                add     hl,bc

                dec     e
                jr      nz, pe_l1

                ld      hl, 09800h + 9 + (5*32)
                ld      de, 12
pe_l8:
                ldh     a,(STAT & 0FFh)
                bit     1,a
                jr      nz,pe_l8

                ld      a,(hl)
                and     1
                jr      z, pe_l9
                inc     d
pe_l9:
                ld      bc,32
                add     hl,bc
                dec     e
                jr      nz, pe_l8

                ld      a,d
                sub     6
                or      a
                jr      nz, no_draw
                ld      a,148
                call    show_state
                ret
no_draw:        and     080h
                jr      nz, win_left
                ld      a,156
                call    show_state
                ret
win_left:       ld      a,152
                call    show_state
                ret

; =============================================================================
; draw a vertical bar beginning at hl, length e and with start-character a
; =============================================================================
draw_bar:
                ld      bc, 32
                ld      (hl),a
                add     hl,bc
                inc     a
                inc     a
pd_l2:          ld      (hl),a
                add     hl,bc
                dec     e
                jr      nz, pd_l2
                inc     a
                inc     a
                ld      (hl),a
                ret

; =============================================================================
; draw a horizontal wire at hl until a terminator is found (to left)
; =============================================================================
pl_wire:
                ld      a,(hl)
                cp      134     ; upper border of an and
                jr      nz, pnua
                ld      bc, 32 + 1
                add     hl,bc
                jr      pl_wire
pnua:
                cp      138     ; lower border of an and
                jr      nz, pnla
                ld      bc, -(32 - 1)
                add     hl,bc
                jr      pl_wire
pnla:
                cp      136     ; center of splitter
                jr      nz, pnmo
                push    hl
                ld      bc, 32+1
                add     hl, bc
                call    pl_wire
                pop     hl
                ld      bc, -(32-1)
                add     hl, bc
                call    pl_wire
                ret
pnmo:
                cp      140   ; repeater
                jr      z, plskip
                cp      162   ; inverter 
                jr      z, plskip

                or      a       ; everything else = exit
                ret     nz
                ld      a, 129
                ld      (hl),a
plskip:         inc     hl
                jr      pl_wire

; =============================================================================
; draw a horizontal wire at hl until a terminator is found (to right)
; =============================================================================
pr_wire:
                ld      a,(hl)
                cp      135     ; upper border of and-gatter:
                jr      nz, pnuar
                ld      bc, 32 - 2
                add     hl,bc
                jr      pr_wire
pnuar:
                cp      139     ; lower border of and-gatter:
                jr      nz, pnlar
                ld      bc, -(32 + 1)
                add     hl,bc
                jr      pr_wire
pnlar:
                cp      137     ; center of splitter-gatter
                jr      nz, pnmor
                push    hl
                ld      bc, 32-1
                add     hl, bc
                call    pr_wire
                pop     hl
                ld      bc, -(32 + 1)
                add     hl, bc
                call    pr_wire
                ret
pnmor:

                cp      143   ; repeater
                jr      z, prskip
                cp      163   ; inverter 
                jr      z, prskip

                or      a       ; everything else = exit
                ret     nz

                ld      a, 129  ; wire
                ld      (hl),a
prskip:         dec     hl
                jr      pr_wire

show_state:
                push    af
pw_l9:          ldh     a,(STAT & 0FFh)
                bit     1,a
                jr      nz,pw_l9
                pop     af
                ld      (09800h +  9 + (3*32)),a
                inc     a
                ld      (09800h +  9 + (4*32)),a
                inc     a
                ld      (09800h + 10 + (3*32)),a
                inc     a
                ld      (09800h + 10 + (4*32)),a

                ret

; =============================================================================
; activate wires for left player at hl
; =============================================================================
pl_activate:
                ld      a,(hl)
                cp      134     ; upper border of an and
                jr      nz, panua
                push    hl
                ld      bc, 64-1
                add     hl,bc
                ld      a,(hl)
                pop     hl
                cp      130     ; if other wire is active, too
                ret     nz
                ld      bc, 32+1
                add     hl,bc
                jr      pl_activate
panua:
                cp      138     ; lower border of an and
                jr      nz, panla
                push    hl
                ld      bc, -(64+1)
                add     hl,bc
                ld      a,(hl)
                pop     hl
                cp      130     ; if other wire is active, too
                ret     nz
                ld      bc, -(32-1)
                add     hl,bc
                jr      pl_activate
panla:
                cp      136     ; center of splitter
                jr      nz, panmo
                push    hl
                ld      bc, 32+1
                add     hl, bc
                call    pl_activate
                pop     hl
                ld      bc, -(32-1)
                add     hl, bc
                call    pl_activate
                ret
panmo:
                cp      140   ; repeater
                jr      z, palskip
                cp      162   ; inverter 
                jr      z, lfill_invert

                cp      129     ; if not a wire exit
                ret     nz
pw_4:           ldh     a,(STAT & 0FFh)
		bit     1,a
                jr      nz,pw_4
                ld      a, 130
                ld      (hl),a
palskip:        inc     hl
                jr      pl_activate

lfill_invert:
                inc     hl
lf_l1:
                ld      a,(hl)
                cp      129
                ret     nz
pw_2:           ldh     a,(STAT & 0FFh)
		bit     1,a
                jr      nz,pw_2
                ld      a, 131
                ldi     (hl),a
                jr      lf_l1

; =============================================================================
; activate wires for right player at hl
; =============================================================================
pr_activate:
                ld      a,(hl)
                cp      135     ; upper border of an and
                jr      nz, panuar
                push    hl
                ld      bc, 64+1
                add     hl,bc
                ld      a,(hl)
                pop     hl
                cp      133     ; if other wire is active, too
                ret     nz
                ld      bc, 32-1
                add     hl,bc
                jr      pr_activate
panuar
                cp      139     ; lower border of an and
                jr      nz, panlar
                push    hl
                ld      bc, -(64-1)
                add     hl,bc
                ld      a,(hl)
                pop     hl
                cp      133     ; if other wire is active, too
                ret     nz
                ld      bc, -(32+1)
                add     hl,bc
                jr      pr_activate
panlar:
                cp      137     ; center of splitter
                jr      nz, panmor
                push    hl
                ld      bc, 32-1
                add     hl, bc
                call    pr_activate
                pop     hl
                ld      bc, -(32+1)
                add     hl, bc
                call    pr_activate
                ret
panmor:
                cp      143   ; repeater
                jr      z, parskip
                cp      163   ; inverter 
                jr      z, rfill_invert

                cp      129     ; if not a wire exit
                ret     nz

pw_1:           ldh     a,(STAT & 0FFh)
		bit     1,a
                jr      nz,pw_1
                ld      a, 133
                ld      (hl),a
parskip:        dec     hl
                jr      pr_activate

rfill_invert:
                dec     hl
rf_l1:
                ld      a,(hl)
                cp      129
                ret     nz
pw_3:           ldh     a,(STAT & 0FFh)
		bit     1,a
                jr      nz,pw_3
                ld      a, 132
                ldd     (hl),a
                jr      rf_l1

; =============================================================================
; red/yellow/pink/black palette
para_pal:
.word 31
.word 7166
.word 31775
.word 0

; =============================================================================
; include the Paradroid font
#include "para.inc"
