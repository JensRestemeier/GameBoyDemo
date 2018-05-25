; =============================================================================
; Gameboy DEMO
; (C) by Jens Ch. Restemeier, <XXXXXXXXXXXX>
; =============================================================================

#include "gameboy.inc"

lorambase       =       0c0a0h  ; some space for the sprite-table
hirambase       =       0ff80h  

#DEFINE LOBYTEVAR(X)  X = lorambase \lorambase  .set (lorambase + 1)
#DEFINE HIBYTEVAR(X)  X = hirambase \hirambase  .set (hirambase + 1)
#DEFINE LOWORDVAR(X)  X = lorambase \lorambase  .set (lorambase + 2)
#DEFINE HIWORDVAR(X)  X = hirambase \hirambase  .set (hirambase + 2)

; == variables in hi-ram ==
HIBYTEVAR(GB_VERSION)

HIBYTEVAR(JOY_TIPP)
HIBYTEVAR(JOY_HOLD)
HIBYTEVAR(OLD_JOY_HOLD)

HIWORDVAR(IRQ_HANDLER)

HIBYTEVAR(SEM_VBLANK)
HIBYTEVAR(SCREEN)
HIBYTEVAR(EN_SOUND)
HIBYTEVAR(EN_COLOR)
HIBYTEVAR(HAS_COLOR)

; all variables for the sound-player
HIWORDVAR(VOICE_1)
HIBYTEVAR(DUR_1)
HIWORDVAR(VOICE_2)
HIBYTEVAR(DUR_2)
HIWORDVAR(VOICE_3)
HIBYTEVAR(DUR_3)
HIBYTEVAR(PLAY_VOICES)

; variables for APA-demonstration
; rotation-angle
HIBYTEVAR(rot)
; matrix
HIBYTEVAR(m00)
HIBYTEVAR(m01)
HIBYTEVAR(m10)
HIBYTEVAR(m11)

HIBYTEVAR(x1)
HIBYTEVAR(y1)
HIBYTEVAR(x2)
HIBYTEVAR(y2)
HIBYTEVAR(radius)
HIBYTEVAR(color)

HIBYTEVAR(xc)
HIBYTEVAR(yc)
HIBYTEVAR(i1)
HIBYTEVAR(i2)
HIBYTEVAR(sx)
HIBYTEVAR(sy)
HIWORDVAR(d)

; some screens need a "slow-down" counter
HIBYTEVAR(speed)

; variables for S_FLI.INC
HIBYTEVAR(tabidx)
HIWORDVAR(textpos)

; variables for the printer driver
HIWORDVAR(crc)
HIBYTEVAR(stat_1)
HIBYTEVAR(stat_2)

; variables for the tiny game
HIBYTEVAR(xpos)
HIBYTEVAR(hpos)
HIBYTEVAR(heli_x)
HIBYTEVAR(heli_y)
HIWORDVAR(g_points)

; variables for paradroid-game
HIBYTEVAR(lsig)
HIBYTEVAR(rsig)

; allocate some memory for the SGB packet-buffer
sgb_packet      = lorambase
lorambase       .set (lorambase + 16)

; allocate some memory for the projected points
t_points        = lorambase
lorambase       .set (lorambase + 256)

; == end of variables ==

vblank_address = hirambase      ; last item in hiram is the Sprite-DMA function

; == constants: ==

; gameboy versions:
UNKNOWN         = 0
CLASSIC_GAMEBOY = 1
SUPER_GAMEBOY   = 2
POCKET_GAMEBOY  = 3
COLOR_GAMEBOY   = 4

; =============================================================================
.org 0
; Try "TYPE GBDEMO.GB" to see this:
.text "GameBoy DEMO"
; This is: LF, CR, EOF
.byte 00ah,00dh,01ah
; =============================================================================

.org 020h
; =============================================================================
; Wait for VBL - destroys AF. To be called with "rst 20h"
; =============================================================================
                ldh     a,(LCDC & 0FFh)
                bit     7,a
                ret     z
waitvbl:        ldh     a,(LCDC & 0FFh)      ; Wait for VBL
                add     a,a
		ret     nc
notyet:         ldh     a,(LY & 0FFh) 
                cp      90h
                jr      nz,$-4
		ret

; = IRQ vectors =
.org 040h                ; VBlank IRQ
                ; jump to where we copied the DMA-function to...
                jp      vblank_address       
.org 048h                ; LCDC Status IRQ
                ; all FLI-effects are done here:
                jr      s_irq
.org 050h                ; Timer Owerflow IRQ
                reti
.org 058h                ; Serial Transfer Completion IRQ
                reti                    ; Do nothing
.org 060h                ; Joypad action (?) IRQ
                reti                    ; Do nothing
; Irqs done..

.org 070h
bitmask:
; A table for a index-to-bitmask translation
; The address (070h) is hardcoded
.byte 080h,040h,020h,010h,008h,004h,002h,001h,000h

.org 080h
; A table for a fast conversion from a nibble to a hex char
hextable:
.byte "0123456789ABCDEF"

.org 090h
; A table with cosinus values
cos_table:
.byte  64,  62,  59,  53,  45,  35,  24,  12, 0, -12, -24, -35, -45, -53, -59, -62
.byte -64, -62, -59, -53, -45, -35, -24, -12, 0,  12,  24,  35,  45,  53,  59,  62

; - Additional IRQ functions ----------------------------------------------

; indirect FLI function 
s_irq:
                push    af
                push    bc
                push    de
                push    hl
                call    sub_irq         ; put adr of irq-exit on stack
                pop     hl
                pop     de
                pop     bc
                pop     af
                reti
sub_irq:        ld      hl,IRQ_HANDLER
                ldi     a,(hl)
                ld      d,a
                ld      a,(hl)
                ld      e,a
                push    de      ; push handler-address on stack
                ret

; = Gameboy Header ============================================================
.org 100h
		nop
		jp      start

.byte $CE,$ED,$66,$66,$CC,$0D,$00,$0B,$03,$73,$00,$83 ; Nintendo Logo
.byte $00,$0C,$00,$0D,$00,$08,$11,$1F,$88,$89,$00,$0E
.byte $DC,$CC,$6E,$E6,$DD,$DD,$D9,$99,$BB,$BB,$67,$63
.byte $6E,$0E,$EC,$CC,$DD,$DC,$99,$9F,$BB,$B9,$33,$3E

.byte "GAMEBOY DEMO   "         ; Cart name   14bytes
.byte 080h                      ; Color GB
.word 0FFFFh                    ; No  Vendor
.byte 003h                      ; Super GB functions
.byte 0                         ; Cart type   ROM Only
.byte 0                         ; ROM Size    32k
.byte 0                         ; RAM Size     0k
.byte 0                         ; destination: non-japanese
.byte 033h                      ; Maker ID -> new scheme
.byte 1                         ; Version
.byte 0FFh                      ; Complement check
.word 0ffffh                    ; Cheksum
; =============================================================================

; == startup code ==

start:          di

                ; init stack
                ld      sp,0fffeh

                ; let's find out, what kind of GB we have
                ; first the easy stuff: GB Color and GB pocket
                cp      011h         
                jr      nz, no_CGB
                ld      a, COLOR_GAMEBOY
                jr      gb_found
no_CGB:         cp      0FFh
                jr      nz, no_PGB
                ld      a, POCKET_GAMEBOY
                jr      gb_found

no_PGB:         cp      001h
                jr      nz, no_GB

                ; to distinguish Super GB and classic GB try a MLT_REQ
                call    clear_packet
                ld      a,010001001b    ; MLT_REQ
                ld      (sgb_packet),a
                ld      a,000000011b    ; request 5 players
                ld      (sgb_packet+1),a
                call    send_packet
                ld      a, 030h
                ldh     (P1 & 0FFh), a
                ldh     a, (P1 & 0FFh)
                ldh     a, (P1 & 0FFh)
                ldh     a, (P1 & 0FFh)
                ldh     a, (P1 & 0FFh)
                ldh     a, (P1 & 0FFh)
                and     00fh
                cp      00fh
                jr      nz, sgb_found
                ld      a, 000h
                ldh     (P1 & 0FFh), a
                ld      a, 030h
                ldh     (P1 & 0FFh), a
                ldh     a, (P1 & 0FFh)
                ldh     a, (P1 & 0FFh)
                ldh     a, (P1 & 0FFh)
                ldh     a, (P1 & 0FFh)
                ldh     a, (P1 & 0FFh)
                and     00fh
                cp      00fh
                jr      nz, sgb_found

                ld      a, CLASSIC_GAMEBOY
                jr      gb_found
sgb_found:
                ; we don't need 5-player mode, disable it again
                ld      a,010001001b
                ld      (sgb_packet),a
                ld      a,000000000b    ; no request
                ld      (sgb_packet+1),a
                call    send_packet

                ld      a, SUPER_GAMEBOY
                jr      gb_found
no_GB:          ld      a, UNKNOWN
gb_found:       ld      (GB_VERSION),a  ; store GB-version

                ; clear ram
                ld      hl, 0C000h
                ld      de, 02000h
clear_ram:      xor     a
                ldi     (hl),a
                dec     de
                ld      a,d
                or      e
                jp      nz,clear_ram

                ; clear hiram, except GB-version
                ld      c, 081h
                ld      d, 07Eh
clear_hiram:    xor     a
                ld      (c),a
                inc     c
                dec     d
                jp      nz, clear_hiram

                ; copy vblank handler into hiram
                ld      c,(vblank_address & 0FFh)  
                ld      hl,vblank_start
                ld      de,(vblank_stop-vblank_start)
copy_func:      ldi     a,(hl)
                ld      (c),a
                inc     c
                dec     de
                ld      a,d
                or      e
                jr      nz, copy_func

                call    default_init

                ld      hl, text_font   ; install the text-font
                ld      de, 09000h
                ld      bc, 128*8*2
                call    copy_nv

                ; init configuration
                ld      a,1
                ldh     (EN_SOUND & 0FFh),a     ; enable sound
                ldh     (HAS_COLOR & 0FFh),a
                ldh     (EN_COLOR & 0FFh),a     ; enable color (if present)

                ld      a,(GB_VERSION)
                cp      COLOR_GAMEBOY           ; Is it a GB-Color ?
                jr      z, gb_color
; ... no time to support SGB-color ...
;                cp      SUPER_GAMEBOY           ; Is it a Super-GB ?
;                jr      z, gb_color

                ld      a,0                     ; all other cases: color n/a
                ldh     (HAS_COLOR & 0FFh),a
                ldh     (EN_COLOR & 0FFh),a
gb_color:
                call    display_on
                call    init_sound

                ei      ; let's go !

restart:        ; set start-screen
                ld      a,0
                ldh     (SCREEN & 0FFh),a

; == main loop ==
main:
                call    readjoy

                ldh     a, (JOY_HOLD & 0FFh) ; check for start+select+a+b=reset
                cp      00fh
                jr      z, restart

                call    dispatch        ; execute slice of the game

sleep:          halt                    ; save engergy
                nop                     ; avoid hlt-bug

                ld      a,(SEM_VBLANK)  ; The only IRQ we want is VBLANK
                or      a               
                jr      z,sleep         
                xor     a
                ld      (SEM_VBLANK),a

                jr      main

; =============================================================================
; Execute one slice of the game. Could be extended to switch banks
; =============================================================================
dispatch:
                ldh     a,(SCREEN & 0FFh)
                sla     a
                ld      hl,jump_table
                add     a,l
                ld      l,a
                ld      a,h
                adc     a,0
                ldi     a,(hl)
                ld      c,a
                ld      a,(hl)
                ld      b,a
                push    bc
nothing:        ret

jump_table:     .word drw_start ; 0
                .word s1_start  ; 1
                .word s2_start  ; 2
                .word s3_start  ; 3
                .word drw_adr   ; 4
                .word s1_adr    ; 5
                .word drw_opt   ; 6
                .word s1_opt    ; 7
                .word s2_opt    ; 8
                .word s3_opt    ; 9
                .word s4_opt    ; 10
                .word drw_test  ; 11
                .word hnd_test  ; 12
                .word rdrw_test ; 13
                .word drw_fli   ; 14
                .word s1_fli    ; 15
                .word s2_fli    ; 16
                .word s3_fli    ; 17
                .word drw_apa   ; 18
                .word s1_apa    ; 19
                .word s2_apa    ; 20
                .word s2_adr    ; 21
                .word drw_game  ; 22
                .word s1_game   ; 23
                .word s2_game   ; 24
                .word s3_game   ; 25
                .word drw_para  ; 26
                .word s1_para   ; 27
                .word nothing

; =============================================================================
; VBLANK IRQ-function, to be copied into HiRAM
; =============================================================================
vblank_start:
                push    af
                ld      a,1             ; set vblank semaphore
                ldh     (SEM_VBLANK & 0FFh),a
                ld      a,0c0h          ; start sprite-DMA
                ldh     (SPRDMA & 0FFh),a
                ld      a,28h           ; wait until DMA is over
                dec     a          
                jr      nz,$-1
                pop     af         
                jp      timer_irq
;                reti
vblank_stop:

; =============================================================================
; Initialise GB into a known state
; =============================================================================
default_init:
                ld      hl, sprite_a    ; install the sprites
                ld      de, 08000h
                ld      bc, 24*8*2
                call    copy_nv

                xor     a               ; Misc standard init things..
                ldh     (STAT & 0FFh),a ; LCDC Status
                ldh     (SCX & 0FFh),a  ; Screen scroll Y=0
                ldh     (SCY & 0FFh),a  ; Screen scroll X=0

                ld      a, 0FFh
                ldh     (LYC & 0FFh), a ; LYC outside of used area
                ldh     (WX & 0FFh),a   ; Window X - not visible
                ldh     (WY & 0FFh),a   ; Window Y - not visible

                call    display_off
                ld      a,01000011b     ; LCD Controller = Off (No picture on screen)
                                        ; WindowBank = $9C00
					; Window = OFF
                                        ; BG Chr = $8800
					; BG Bank= $9800
					; OBJ    = 8x8
                                        ; OBJ    = On
					; BG     = On
                ldh     (LCDC & 0FFh),a

		ld      a,11100100b     ; grey 3=11 (Black)
					; grey 2=10 (Dark grey)
					; grey 1=01 (Light grey)
					; grey 0=00 (Transparent)
                ldh     (BGP & 0FFh),a  ; background
                ldh     (OBP0 & 0FFh),a ; sprites a
                ldh     (OBP1 & 0FFh),a ; sprites b

                ld      a,000000000b    ; disable all IRQ
                ldh     (IE & 0FFh),a

                ; install dummy-IRQ-handler
                ld      de, nothing
                ld      hl, IRQ_HANDLER
                ld      a, d
                ldi     (hl),a
                ld      a, e
                ld      (hl), e

                ld      a,000000001b    ; enable VBLANK and Timer IRQ 
                ldh     (IE & 0FFh),a

                ; hide/clear all sprites
                ld      c,09fh
                xor     a
                ld      hl, 0c000h
dil1:           ldi     (hl),a
                dec     c
                jr      nz, dil1

                ; set attributes to 0 (screen 1 and screen 2)
                ld      a, 1            ; switch to attribute RAM
                ldh     (VBK & 0FFh),a

                ld      hl, 09800h
                ld      bc, 0800h
                xor     a               ; no atr/palette 0
                call    fill_nv

                ld      a, 0            ; switch to normala RAM
                ldh     (VBK & 0FFh),a

                ; clear bg map (screen 1 and screen 2)
                ld      hl, 09800h
                ld      bc, 0800h
                xor     a               ; should be blank in a font
                call    fill_nv

                ret

; =============================================================================
; Switch off LCD-display
; =============================================================================
display_off:
                rst     20h             ; wait for vbl
                ld      hl,LCDC
                res     7,(hl)          ; display off
                res     1,(hl)          ; bg off
                res     0,(hl)          ; ob off
                ret

; =============================================================================
; Switch on LCD-display
; =============================================================================
display_on:
                ld      hl,LCDC
                set     7,(hl)          ; display on
                set     1,(hl)          ; bg on
                set     0,(hl)          ; ob on
                ret

; =============================================================================
; wait for one frame
; =============================================================================
wait_frame:     ldh     a,(STAT & 0FFh) ; wait for vbl
                and     3
                cp      001b            
                jr      nz, wait_frame
wf_l1:          ldh     a,(STAT & 0FFh) ; wait for vbl
                and     3
                cp      001b            
                jr      z, wf_l1
                ret

; =============================================================================
; Wait for a key
; =============================================================================
wait_key:       call    readjoy
		ld      a,(JOY_TIPP)
		or      a
		jp      z,wait_key
		ret

; =============================================================================
; Read joypad
; =============================================================================
readjoy:        ld      a,020h  ; read first nibble
                ldh     (JOYPAD & 0FFh),a
                ldh     a,(JOYPAD & 0FFh)
                ldh     a,(JOYPAD & 0FFh)
                ldh     a,(JOYPAD & 0FFh)
                ldh     a,(JOYPAD & 0FFh)
                ldh     a,(JOYPAD & 0FFh)
                ldh     a,(JOYPAD & 0FFh)
                ldh     a,(JOYPAD & 0FFh)
                ldh     a,(JOYPAD & 0FFh)
		cpl
		and     00fh
		swap    a
		ld      b,a
		ld      a,010h		; read second nibble
                ldh     (JOYPAD & 0FFh),a
                ldh     a,(JOYPAD & 0FFh)
                ldh     a,(JOYPAD & 0FFh)
                ldh     a,(JOYPAD & 0FFh)
                ldh     a,(JOYPAD & 0FFh)
                ldh     a,(JOYPAD & 0FFh)
                ldh     a,(JOYPAD & 0FFh)
                ldh     a,(JOYPAD & 0FFh)
                ldh     a,(JOYPAD & 0FFh)
		cpl
		and     00fh
		or      b               ; combine them
		ld      c,a
                ldh     a,(JOY_HOLD & 0FFh)
                ldh     (OLD_JOY_HOLD & 0FFh),a
		xor     c
		and     c
                ldh     (JOY_TIPP & 0FFh),a
		ld      a,c
                ldh     (JOY_HOLD & 0FFh),a
		ld      a,030h
                ldh     (JOYPAD & 0FFh),a
		ret

; =============================================================================
; copy bc, no wait for vblank
; =============================================================================
copy_nv:
                ldi     a,(hl)
                ld      (de),a
                inc     de
                dec     bc
                ld      a,b
                or      c
                jr      nz, copy_nv
                ret

; =============================================================================
; copyz, waiting for vblank, hl=source, de=adr end='0'
; =============================================================================
copy_z:
                ldh     a,(STAT & 0FFh)
		bit     1,a
                jr      nz,copy_z
                ldi     a,(hl)
                or      a
                ret     z
                ld      (de),a
                inc     de
                jr      copy_z

; =============================================================================
; Fill bc bytes from hl with a, no wait for vblank
; =============================================================================
fill_nv:        ld      d,a
f1_l1:          ld      a,d
		ldi     (hl),a
		dec     bc
		ld      a,b
		or      c
                jp      nz,f1_l1
		ret

; =============================================================================
; Set attrib "box", position hl, width b, height c, d attrib
; =============================================================================
set_atr_box:
                ld      a, 1            ; switch to attribute RAM
                ldh     (VBK & 0FFh),a
sab_l2:
                push    bc
                push    hl
                ld      a,d
sab_l1:         ldi     (hl),a
                dec     b
                jr      nz, sab_l1
                pop     hl

                ld      bc,32
                add     hl,bc

                pop     bc

                dec     c
                jr      nz, sab_l2

                ld      a, 0
                ldh     (VBK & 0FFh),a
                ret

; =============================================================================
; draw a nice border, position hl, width-4 b, height-4 c
; =============================================================================
draw_border:
                ; draw upper line
                push    hl
                push    bc
                ld      a, 1
                ldi     (hl),a
                ld      a, 3
                ldi     (hl),a
db_l1:          ld      a, 16
                ldi     (hl),a
                dec     b
                ld      a,b
                or      a
                jr      nz, db_l1
                ld      a, 5
                ldi     (hl),a
                ld      a, 4
                ld      (hl),a
                pop     bc
                pop     hl
                ld      de,32
                add     hl,de

                ; second line
                push    hl
                push    bc
                ld      a, 2
                ldi     (hl),a
                ld      a, 2
                add     a, b
                add     a, l
                ld      l,a
                ld      a,h
                adc     a,0
                ld      h,a
                ld      a, 6
                ld      (hl),a
                pop     bc
                pop     hl
                ld      de,32
                add     hl,de

db_l2:          ; middle lines
                push    hl
                push    bc
                ld      a, 15
                ldi     (hl),a
                ld      a, 2
                add     a, b
                add     a, l
                ld      l,a
                ld      a,h
                adc     a,0
                ld      h,a
                ld      a, 13
                ld      (hl),a
                pop     bc
                pop     hl

                ld      de,32
                add     hl,de

                dec     c
                ld      a,c
                or      a
                jr      nz,db_l2

                ; one-before-last line
                push    hl
                push    bc
                ld      a, 12
                ldi     (hl),a
                ld      a, 2
                add     a, b
                add     a, l
                ld      l,a
                ld      a,h
                adc     a,0
                ld      h,a
                ld      a, 8
                ld      (hl),a
                pop     bc
                pop     hl

                ld      de,32
                add     hl,de

                ; draw last line
                push    hl
                push    bc
                ld      a, 10
                ldi     (hl),a
                ld      a, 11
                ldi     (hl),a
db_l3:          ld      a, 14
                ldi     (hl),a
                dec     b
                ld      a,b
                or      a
                jr      nz, db_l3
                ld      a, 9
                ldi     (hl),a
                ld      a, 7
                ld      (hl),a
                pop     bc
                pop     hl
                ret

; =============================================================================
; setup background palette entry, hl=address, a=slot
; =============================================================================
set_bg_pal:
                sla     a               ; entry-nr a*8
                sla     a
                sla     a
                or      010000000b      ; auto-increment, from 0
                ldh     (BCPS & 0FFh), a
                ldi     a,(hl)
                ldh     (BCPD & 0FFh), a
                ldi     a,(hl)
                ldh     (BCPD & 0FFh), a
                ldi     a,(hl)
                ldh     (BCPD & 0FFh), a
                ldi     a,(hl)
                ldh     (BCPD & 0FFh), a
                ldi     a,(hl)
                ldh     (BCPD & 0FFh), a
                ldi     a,(hl)
                ldh     (BCPD & 0FFh), a
                ldi     a,(hl)
                ldh     (BCPD & 0FFh), a
                ldi     a,(hl)
                ldh     (BCPD & 0FFh), a
                ret

; =============================================================================
; setup object palette entry, hl=address, a=slot
; =============================================================================
set_ob_pal:
                sla     a               ; entry-nr a*8
                sla     a
                sla     a
                or      010000000b      ; auto-increment, from 0
                ldh     (OCPS & 0FFh), a
                ldi     a,(hl)
                ldh     (OCPD & 0FFh), a
                ldi     a,(hl)
                ldh     (OCPD & 0FFh), a
                ldi     a,(hl)
                ldh     (OCPD & 0FFh), a
                ldi     a,(hl)
                ldh     (OCPD & 0FFh), a
                ldi     a,(hl)
                ldh     (OCPD & 0FFh), a
                ldi     a,(hl)
                ldh     (OCPD & 0FFh), a
                ldi     a,(hl)
                ldh     (OCPD & 0FFh), a
                ldi     a,(hl)
                ldh     (OCPD & 0FFh), a
                ret
; =============================================================================
; set complete palette to gray
; =============================================================================
set_gray:
                ld      a,0
                ld      hl, gray_pal
                call    set_bg_pal
                ld      a,1
                ld      hl, gray_pal
                call    set_bg_pal
                ld      a,2
                ld      hl, gray_pal
                call    set_bg_pal
                ld      a,3
                ld      hl, gray_pal
                call    set_bg_pal
                ld      a,4
                ld      hl, gray_pal
                call    set_bg_pal
                ld      a,5
                ld      hl, gray_pal
                call    set_bg_pal
                ld      a,6
                ld      hl, gray_pal
                call    set_bg_pal
                ld      a,7
                ld      hl, gray_pal
                call    set_bg_pal
                ld      a,0
                ld      hl, gray_pal
                call    set_ob_pal
                ld      a,1
                ld      hl, gray_pal
                call    set_ob_pal
                ld      a,2
                ld      hl, gray_pal
                call    set_ob_pal
                ld      a,3
                ld      hl, gray_pal
                call    set_ob_pal
                ld      a,4
                ld      hl, gray_pal
                call    set_ob_pal
                ld      a,5
                ld      hl, gray_pal
                call    set_ob_pal
                ld      a,6
                ld      hl, gray_pal
                call    set_ob_pal
                ld      a,7
                ld      hl, gray_pal
                call    set_ob_pal
                ret
; =============================================================================

; = a simple sound-player =
#include "SOUND.INC"

; = actions for start screen =
#include "S_START.INC"

; = actions for address screen =
#include "S_ADR.INC"

; = actions for options screen =
#include "S_OPT.INC"

; = actions for joypad test =
#include "S_TEST.INC"

; = actions for FLI demo =
#include "S_FLI.INC"

; = graphics demo =
#include "S_GBAPA.INC"

; = game =
#include "S_GAME.INC" 

; = paradroid logic-game =
#include "S_PARA.INC" 

; = super-gb utilities =
#include "SGB.INC"

; = GB-printer =
#include "PRINTER.INC"

; = Decompression of packed data =
;#include "DECOMP.INC"

; = text_strings =
#include "TEXT.INC"

; = some random data =
#include "RANDOM.INC"

; = game data =
; an 8x8 4-color font... nothing beautifull 
#include "FONT.INC"

; a few ugly sprites
#include "SPRITES.INC"

; color_palettes
gray_pal:
.word 32767
.word 21140
.word 10570
.word 0

red_pal:
.word 32767
.word 21151
.word 10591
.word 31

gold_pal:
.word 32767
.word 1023
.word 11263
.word 21503

blue_pal:
.word 32767
.word 32404
.word 32074
.word 31744

; =============================================================================

; Single bank/second bank: check for ROM-overflow
#if ($ > 07FFFh)
		ROM_OVERFLOW_ERROR
#endif

; First bank: check for bank-overflow
;#if ($ > 03FFFh)
;               BANK_OVERFLOW_ERROR
;#endif

; =============================================================================

.end
