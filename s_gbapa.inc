; =============================================================================
; Gameboy "APA" (All-Points-Addressable) Demo
; (C) by Jens Ch. Restemeier, <XXXXXXXXXXXX>
; =============================================================================

drw_apa:
                ; hide cursor
                ld      hl,0c000h       ; sprite-table
                ld      a, 0            ; Y
                ld      (hl),a

                call    display_off
                call    default_init

                ; init color-palette for GB-Color
                ld      a,0
                ld      hl,apa_pal
                call    set_bg_pal

                ldh     a, (EN_COLOR & 0FFh)
                or      a
                call    z, set_gray

                ld      hl, LCDC
                set     4,(hl)          ; BG=8000-8FFF

                ld      hl, 09800h      ; First clear the screen
cloop1:         ld      (hl),0ffh       ; This char (0ffh) isn't used for GFX.
                inc     hl              ; Chars from 0f0h-0ffh may be used for a frame or something...
		ld      a,h
		cp      09ch
		jp      nz,cloop1
		ld      b,0
                ld      hl, 09822h        ; Now draw the 16*16 matrix
		ld      e,10h
loop1:          ld      d,10h
loop2:          ld      a,b
		swap    a               ; The screen must be rotated by 90ø.
		ldi     (hl),a
		inc     b
		dec     d
		jp      nz,loop2
		push    de
                ld      de,00010h
		add     hl,de
		pop     de
		dec     e
		jp      nz,loop1

                ; clear back-buffer
                xor     a
                ld      hl,0D000h
                ld      bc,01000h
                call    fill_nv

                ; clear font
                xor     a
                ld      hl,08000h
                ld      bc,01000h
                call    fill_nv

                call    display_on

                xor     a
                ldh     (rot & 0FFh), a

                ; first state
                ld      a,19
                ldh     (SCREEN & 0FFh),a

                ret

; =============================================================================

mul16u:         ; hl=bc*de      (16*16 multiplikation)
                xor     a
                ld      h,a
                ld      l,a
m8u_1:          srl     d       ; de/=2
                rr      e
                jr      nc, m8u_2
                ld      a,l     ; hl+=bc
                add     a,c
                ld      l,a
                ld      a,h
                adc     a,b
                ld      h,a
m8u_2:          sla     c       ; bc*=2
                rl      b
                ld      a,c
                or      b
                jr      nz, m8u_1
                ret

; =============================================================================

mul8s:          ; a=(b*c)/64      (8*8 signed multiplikation fixkomma)
                push    de
                push    hl

                ld      a,b     ; d=sign(a)*sign(b)
                xor     c
                and     080h
                ld      d,a

                bit     7,b     ; b=abs(b)
                jr      z, ms_l1
                ld      a,b
                cpl   
                inc     a
                ld      b,a
ms_l1:
                bit     7,c     ; c=abs(c)
                jr      z, ms_l2
                ld      a,c
                cpl    
                inc     a
                ld      c,a
ms_l2:
                push    de
                ld      d,0
                ld      e,b
                ld      b,0
                call    mul16u  ; hl = b * c
                pop     de

                srl     h       ; hl/=64
                rr      l
                srl     h      
                rr      l
                srl     h       
                rr      l
                srl     h       
                rr      l
                srl     h       
                rr      l
                srl     h       
                rr      l

                ld      a,l

                bit     7,d
                jr      z, ms_l3
                cpl     
                inc     a
ms_l3:
                pop     hl
                pop     de
                ret

; =============================================================================

s1_apa:
                ld      a,22
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     009h            ; if start or 'A'
                ret     nz

                ld      a,20
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     010h            ; if left
                ret     nz

                ld      a,19
                ldh     (SCREEN & 0FFh),a
                call    draw_vectors

                ; rotate
                ldh     a,(rot & 0FFh)
                inc     a
                and     01Fh
                ldh     (rot & 0FFh), a

                ret

; =============================================================================

s2_apa:
                ld      a,22
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     009h            ; if start or 'A'
                ret     nz

                ld      a,19
                ldh     (SCREEN & 0FFh),a
                ldh     a,(JOY_TIPP & 0ffh)
                and     020h            ; if right
                ret     nz

                ld      a,20
                ldh     (SCREEN & 0FFh),a
                call    draw_vectors

                ; rotate
                ldh     a,(rot & 0FFh)
                dec     a
                and     01Fh
                ldh     (rot & 0FFh), a

                ret

; =============================================================================

print_text:
                ldi     a,(hl)
                or      a
                ret     z
                push    hl

                ld      l,a     ; hl=a*16
                ld      h,0

                add     hl,hl
                add     hl,hl
                add     hl,hl
                add     hl,hl

                ld      bc,text_font    ; hl=text_font+bc
                add     hl,bc

                ld      c,16
acc:
                ldi     a,(hl)
                ld      (de),a
                inc     de
                dec     c
                jr      nz, acc

                ld      hl, 120*2
                add     hl,de
                ld      e,l
                ld      d,h

                pop     hl
                jr      print_text

; =============================================================================

draw_vectors:
                ; prepare matrix
                ldh     a,(rot & 0FFh)
                ; sin(rot)
                sub     008h    ; -0.5*pi
                and     01Fh
                ld      hl, cos_table
                ld      e,a
                ld      d,0
                add     hl,de
                ld      a,(hl)
                ldh     (m00 & 0FFh),a
                ldh     (m11 & 0FFh),a
                ; cos (rot)
                ldh     a,(rot & 0FFh)
                ld      hl, cos_table
                ld      e,a
                ld      d,0
                add     hl,de
                ld      a,(hl)
                ldh     (m10 & 0FFh),a
                ; -cos(rot)
                cpl     
                inc     a
                ldh     (m01 & 0FFh),a

                ; transform points
                ld      de, points
                ld      hl, t_points
                ld      bc, (e_points-points)/2
s1_l3:          push    bc
                ld      a,(de)  ; x
                ld      b,a
                ldh     a,(m00 & 0FFh)
                ld      c,a
                call    mul8s
                ld      (hl),a  ; x'=x*m00
                inc     de
                ld      a,(de)  ; y
                ld      b,a
                ldh     a,(m01 & 0FFh)
                ld      c,a
                call    mul8s
                add     a,(hl)  
                add     a,64
                ld      (hl),a  ; x'=x'+y*m01+64
                inc     hl

                dec     de

                ld      a,(de)  ; x
                ld      b,a
                ldh     a,(m10 & 0FFh)
                ld      c,a
                call    mul8s
                ld      (hl),a  ; x'=x*m00
                inc     de
                ld      a,(de)  ; y
                ld      b,a
                ldh     a,(m11 & 0FFh)
                ld      c,a
                call    mul8s
                add     a,(hl)  
                add     a,64
                ld      (hl),a  ; x'=x'+y*m01+64
                inc     hl
                inc     de

                pop     bc
                dec     bc
                ld      a,b
                or      c
                jr      nz, s1_l3

                ; print text
                ld      hl,apa_start
                ld      de,0D000h
                call    print_text

                ; draw vectors
                ld      a,1
		ld      (color),a
                ld      d, (e_strokes-strokes)/2
                ld      hl, strokes
s1_l1:          ld      b, (hl)
                inc     hl
                ld      c, (hl)
                inc     hl
                push    de
                push    hl

                ld      de, t_points    ; hl=t_points+2*b
                ld      l, b
                ld      h, 0
                add     hl,hl
                add     hl,de

                ldi     a,(hl)
                ldh     (x1 & 0FFh),a
                ldi     a,(hl)
                ldh     (y1 & 0FFh),a

                ld      de, t_points    ; hl=t_points+2*c
                ld      l, c
                ld      h, 0
                add     hl,hl
                add     hl,de

                ldi     a,(hl)
                ldh     (x2 & 0FFh),a
                ldi     a,(hl)
                ldh     (y2 & 0FFh),a

		call    line

                pop     hl
                pop     de
                dec     d
                jr      nz, s1_l1

                ld      a,2
		ld      (color),a
                ld      d, (e_circles-circles)/2
                ld      hl, circles
s1_l2:          ld      b, (hl)
                inc     hl
                ld      c, (hl)
                inc     hl
                push    de
                push    hl

                ld      de, t_points    ; hl=t_points+2*b
                ld      l, b
                ld      h, 0
                add     hl,hl
                add     hl,de

                ldi     a, (hl)
                ldh     (x1 & 0FFh),a
                ldi     a, (hl)
                ldh     (y1 & 0FFh),a

                ld      a, c
                ldh     (radius & 0FFh),a

                call    circle

                pop     hl
                pop     de
                dec     d
                jr      nz, s1_l2

                call    swap_buf

                ret

; =============================================================================
; Draw a line from (X1),(Y1) to (X2),(Y2) with (color)
; =============================================================================
line:           xor     a
                ldh     (d & 0FFh),a
                ldh     ((d+1) & 0FFh),a

		ld      a,(x2)
		ld      b,a
		ld      a,(x1)
		ld      (xc),a
		sub     b
		ld      b,0
		or      a
		jr      z,l_px
		ld      b,0ffh
		bit     7,a
		jr      z,l_px
		neg
		ld      b,01h
l_px:           ld      d,a             ; dx sichern
		sla     a
		ld      (i2),a          ; i2=dx*2
		ld      a,b
		ld      (sx),a
		ld      a,(y2)
		ld      b,a
		ld      a,(y1)
		ld      (yc),a
		sub     b
		ld      b,0
		jr      z,l_py
		ld      b,0ffh
		bit     7,a
		jr      z,l_py
		neg
		ld      b,01h
l_py:           ld      e,a             ; dy sichern
		sla     a
		ld      (i1),a          ; i1=dy*2
		ld      a,b
		ld      (sy),a
		ld      a,d
		cp      e               ; dx<dy ?
		jr      c,dloop2
; dx>dy
dloop:          ld      a,(xc)          ; if ((x1==x2)&&(y1==y2)) return;
		ld      b,a
		ld      a,(yc)
		ld      c,a
		call    point
		ld      a,(xc)
		ld      b,a
		ld      a,(x2)
		cp      b
		jr      nz,dnext
		ld      a,(yc)
		ld      b,a
		ld      a,(y2)
		cp      b
		jr      nz,dnext
		ret
dnext:          ld      a,(d)
		ld      e,a
		ld      a,(d+1)
		ld      d,a
		bit     7,d
		jr      nz,xxp
		ld      a,(sy)          ; yc+=sy
		ld      b,a
		ld      a,(yc)
		add     a,b
		ld      (yc),a

		ld      a,(i2)          ; d-=i2
		ld      b,a
		ld      a,e
		sub     b
		ld      e,a
		jr      nc,jnc1
		dec     d
jnc1:
xxp:            ld      a,(sx)          ; xc+=sx
		ld      b,a
		ld      a,(xc)
		add     a,b
		ld      (xc),a

		ld      a,(i1)          ; d+=i1
		ld      b,a
		ld      a,e
		add     a,b
		ld      (d),a
		jr      nc,jnc2
		inc     d
jnc2:           ld      a,d
		ld      (d+1),a
		jr      dloop
; dy>dx
dloop2:         ld      a,(xc)          ; if ((x1==x2)&&(y1==y2)) return;
		ld      b,a
		ld      a,(yc)
		ld      c,a
		call    point
		ld      a,(xc)
		ld      b,a
		ld      a,(x2)
		cp      b
		jr      nz,dnext2
		ld      a,(yc)
		ld      b,a
		ld      a,(y2)
		cp      b
		jr      nz,dnext2
		ret
dnext2:         ld      a,(d)
		ld      e,a
		ld      a,(d+1)
		ld      d,a
		bit     7,d
		jr      nz,yyp
		ld      a,(sx)          ; xc+=sy
		ld      b,a
		ld      a,(xc)
		add     a,b
		ld      (xc),a

		ld      a,(i1)          ; d-=i2
		ld      b,a
		ld      a,e
		sub     b
		ld      e,a
		jr      nc,jnc12
		dec     d
jnc12:
yyp:            ld      a,(sy)          ; yc+=sy
		ld      b,a
		ld      a,(yc)
		add     a,b
		ld      (yc),a

		ld      a,(i2)          ; d+=i1
		ld      b,a
		ld      a,e
		add     a,b
		ld      (d),a
		jr      nc,jnc22
		inc     d
jnc22:          ld      a,d
		ld      (d+1),a
		jr      dloop2

; =============================================================================
; Draw a circle to (x1),(y1) with (radius)
; =============================================================================
circle:         ld      a,(radius)
		ld      e,a             ; e=sy
		ld      l,a
		xor     a
		ld      d,a             ; d=sx
		ld      h,a             
		sla     l               ; hl=3-(hl<<1)
		rl      h
		ld      a,3
		sub     l
		ld      l,a
		ld      a,h
		sbc     a,0
		ld      h,a

		ld      a,(x1)          ; b=x
		ld      b,a
		ld      a,(y1)          ; c=y
		ld      c,a

		push    hl

		push    de
		push    bc              ;  put(px+RADIUS,py)
		ld      a,b
		add     a,e
		ld      b,a
		call    point
		pop     bc
		pop     de

		push    de
		push    bc              ;  put(px-RADIUS,py)
		ld      a,b
		sub     e
		ld      b,a
		call    point
		pop     bc
		pop     de

		push    de
		push    bc              ;  put(px,py+RADIUS)
		ld      a,c
		add     a,e
		ld      c,a
		call    point
		pop     bc
		pop     de

		push    de
		push    bc              ;  put(px,py-RADIUS)
		ld      a,c
		sub     e
		ld      c,a
		call    point
		pop     bc
		pop     de

		pop     hl

cl:             inc     d

		bit     7,h
		jr      z,no_y

		ld      a,d             ; d += (x<<2)
		ld      c,a
		xor     a
		ld      b,a
		sla     c
		rl      b
		sla     c
		rl      b
		add     hl,bc
		jr      cp

no_y:           ld      a,d             ; d += ((x - y)<<2)
		sub     e
		ld      c,a
		ld      a,0
		sbc     a,0
		ld      b,a
		sla     c
		rl      b
		sla     c
		rl      b
		add     hl,bc

		dec     e               ; y--

cp:             push    hl

		push    de              ; put(px+sx,py+sy)
		ld      a,(x1)          ; b=x
		add     a,d
		ld      b,a
		ld      a,(y1)          ; c=y
		add     a,e
		ld      c,a
		call    point
		pop     de

		push    de              ; put(px+sx,py-sy)
		ld      a,(x1)          ; b=x
		add     a,d
		ld      b,a
		ld      a,(y1)          ; c=y
		sub     e
		ld      c,a
		call    point
		pop     de

		push    de              ; put(px-sx,py+sy)
		ld      a,(x1)          ; b=x
		sub     d
		ld      b,a
		ld      a,(y1)          ; c=y
		add     a,e
		ld      c,a
		call    point
		pop     de

		push    de              ; put(px-sx,py-sy)
		ld      a,(x1)          ; b=x
		sub     d
		ld      b,a
		ld      a,(y1)          ; c=y
		sub     e
		ld      c,a
		call    point
		pop     de

		push    de              ; put(px+sy,py+sx)
		ld      a,(x1)          ; b=x
		add     a,e
		ld      b,a
		ld      a,(y1)          ; c=y
		add     a,d
		ld      c,a
		call    point
		pop     de
		
		push    de              ; put(px+sy,py-sx)
		ld      a,(x1)          ; b=x
		add     a,e
		ld      b,a
		ld      a,(y1)          ; c=y
		sub     d
		ld      c,a
		call    point
		pop     de

		push    de              ; put(px-sy,py+sx)
		ld      a,(x1)          ; b=x
		sub     e
		ld      b,a
		ld      a,(y1)          ; c=y
		add     a,d
		ld      c,a
		call    point
		pop     de

		push    de              ; put(px-sy,py-sx)
		ld      a,(x1)          ; b=x
		sub     e
		ld      b,a
		ld      a,(y1)          ; c=y
		sub     d
		ld      c,a
		call    point
		pop     de

		pop     hl

		ld      a,d
		cp      e
		jr      z,ce
		jp      c,cl

ce:             ret

; =============================================================================
; Put a pixel to (b/c) with (color)
; =============================================================================
point:          ld      a,c             ; de = D000h + y*2 + (x/8)*256
		rlc     a
		ld      e,a
		ld      a,b
		srl     a
		srl     a
		srl     a
                add     a,0D0h
		ld      d,a
		ld      a,b             
		and     7
		or      070h
		ld      l,a
		ld      h,0          ; hl=070h+(B&7)
		ld      a,(HL)
		ld      b,a
point1:         ld      hl,color        ; check color
		bit     0,(hl)
		jr      z,point3
		ld      a,(de)
                or      b
		ld      (de),a
point3:         inc     de
		bit     1,(hl)          ; check color
		jr      z,point5
		ld      a,(de)
                or      b
		ld      (de),a
point5:         ret

; =============================================================================
; Clear APA screen / Swap buffers
; =============================================================================
swap_buf:            ld      hl,0D000h
                ld      de,08000h
lp1:
                ldh     a,(STAT & 0FFh)
		bit     1,a
                jr      nz,lp1
                ld      a,(hl)
                ld      (de),a
                ld      (hl),0
		inc     hl
                inc     de
		ld      a,h
                cp      0E0h
		jp      nz,lp1
		ret

points:
.byte   -20, -25 ; 0
.byte   -20, +25 ; 1
.byte   -10, +25 ; 2
.byte   -10,   5 ; 3
.byte     0,   5 ; 4
.byte    10, +25 ; 5
.byte    20, +25 ; 6
.byte    10,  +5 ; 7
.byte    20,  -5 ; 8
.byte    20, -15 ; 9
.byte    10, -25 ; 10
.byte   -10, -15 ; 11
.byte   -10,  -5 ; 12
.byte     5,  -5 ; 13
.byte     5, -15 ; 14
.byte   -30, -25 ; 15
.byte   -30,  25 ; 16
.byte   -20,  35 ; 17
.byte    20,  35 ; 18
.byte    30,  25 ; 19
.byte    30, -25 ; 20
.byte    20, -35 ; 21
.byte   -20, -35 ; 22
.byte     0,   0 ; 23
.byte    10,  -8 ; 24
.byte   -25,  10 ; 25
e_points:

strokes:
.byte   0,1
.byte   1,2
.byte   2,3
.byte   3,4
.byte   4,5
.byte   5,6
.byte   6,7
.byte   7,8
.byte   8,9
.byte   9,10
.byte   10,0
.byte   11,12
.byte   12,13
.byte   13,14
.byte   14,11
.byte   15,16
.byte   16,17
.byte   17,18
.byte   18,19
.byte   19,20
.byte   20,21
.byte   21,22
.byte   22,15
e_strokes:

circles:
.byte   23,5
.byte   24,8
.byte   25,10
e_circles:

apa_pal:
.word   07FFFh
.word   003E0h
.word   07C00h
.word   0001Fh
