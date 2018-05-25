; A very simple GB soundroutine

init_sound:
                ; Volume/Channel setup
                ld      a, 08Fh
                ldh     (NR_52 & 0FFh),a
                ld      a, 077h
                ldh     (NR_50 & 0FFh),a
                ld      a, 0FFh
                ldh     (NR_51 & 0FFh),a

                ; don't use noise for now...
                ld      a, 0
                ldh     (NR_41 & 0FFh),a
                ldh     (NR_42 & 0FFh),a
                ldh     (NR_43 & 0FFh),a
                ldh     (NR_44 & 0FFh),a

                ; put a simple square-wave into Wave-RAM
                ld      hl, 0FF30h
                ld      c, 08h
                ld      a, 0FFh
is1:            ldi     (hl),a
                dec     c
                jr      nz, is1
                ld      c, 08h
                xor     a
is2:            ldi     (hl),a
                dec     c
                jr      nz, is2

                ld      hl, VOICE_1
                ld      de, snd_0
                ld      (hl),e
                inc     hl
                ld      (hl),d
                ld      hl, VOICE_2
                ld      de, snd_1
                ld      (hl),e
                inc     hl
                ld      (hl),d
                ld      hl, VOICE_3
                ld      de, snd_2
                ld      (hl),e
                inc     hl
                ld      (hl),d

                ld      a,1
                ldh     (DUR_1 & 0FFh),a
                ldh     (DUR_2 & 0FFh),a
                ldh     (DUR_3 & 0FFh),a

                ld      a,7
                ldh     (PLAY_VOICES & 0FFh),a

                ret

sound_off:
                ld      a,0             ; sound off to save battery
                ldh     (NR_52 & 0FFh),a
                ldh     (PLAY_VOICES & 0FFh), a

                ldh     a, (EN_SOUND & 0FFh)   ; sound enabled ?
                or      a
                call    nz, init_sound
                jr      exit_irq

; This function is linked into the vblank-IRQ and controls music-playback
timer_irq:
                push    af
                push    bc
                push    de
                push    hl

                ldh     a, (PLAY_VOICES & 0FFh)         ; all voices done playing ?
                or      a
                jr      z, sound_off

                ldh     a, (EN_SOUND & 0FFh)           ; sound enabled ?
                or      a
                jr      z, sound_off

                ldh     a, (DUR_1 & 0FFh)
                dec     a
                ldh     (DUR_1 & 0FFh),a
                call    z, upd_v1

                ldh     a, (DUR_2 & 0FFh)
                dec     a
                ldh     (DUR_2 & 0FFh),a
                call    z, upd_v2

                ldh     a, (DUR_3 & 0FFh)
                dec     a
                ldh     (DUR_3 & 0FFh),a
                call    z, upd_v3

exit_irq:       pop     hl
                pop     de
                pop     bc
                pop     af
                reti

upd_v1:
                ld      hl, PLAY_VOICES
                bit     0,(hl)
                ret     z

                ld      hl, VOICE_1
                ld      e,(hl)
                inc     hl
                ld      d,(hl)
                dec     hl

                ld      a,(de)          ; = duration
                inc     de
                ldh     (DUR_1 & 0FFh),a
                cp      0ffh
                jr      z, off_v1
                ld      a,(de)          ; = frequency low
                inc     de
                ld      b,a
                ld      a,(de)          ; = frequency high
                inc     de
                ld      c,a
                or      b
                jr      z, mute_v1

                ld      a, 000h
                ldh     (NR_10 & 0FFh),a
                ld      a, 080
                ldh     (NR_11 & 0FFh),a
                ld      a, 0F0h
                ldh     (NR_12 & 0FFh), a
                ld      a, b
                ldh     (NR_13 & 0FFh), a
                ld      a, c
                and     7
                or      080h
                ldh     (NR_14 & 0FFh), a

                ld      (hl),e
                inc     hl
                ld      (hl),d
                ret
mute_v1:                
                ld      a, 000h
                ldh     (NR_12 & 0FFh), a

                ld      (hl),e
                inc     hl
                ld      (hl),d
                ret
off_v1:
                ld      hl, PLAY_VOICES
                res     0,(hl)
                ret

upd_v2:
                ld      hl, PLAY_VOICES
                bit     1,(hl)
                ret     z
                ld      hl, VOICE_2
                ld      e,(hl)
                inc     hl
                ld      d,(hl)
                dec     hl

                ld      a,(de)          ; = duration
                inc     de
                ldh     (DUR_2 & 0FFh),a
                cp      0ffh
                jr      z, off_v2
                ld      a,(de)          ; = frequency low
                inc     de
                ld      b,a
                ld      a,(de)          ; = frequency high
                inc     de
                ld      c,a
                or      b
                jr      z, mute_v2

                ld      a, 080
                ldh     (NR_21 & 0FFh),a
                ld      a, 0F0h
                ldh     (NR_22 & 0FFh), a
                ld      a, b
                ldh     (NR_23 & 0FFh), a
                ld      a, c
                and     7
                or      080h
                ldh     (NR_24 & 0FFh), a

                ld      (hl),e
                inc     hl
                ld      (hl),d
                ret
mute_v2:                
                ld      a, 000h
                ldh     (NR_22 & 0FFh), a

                ld      (hl),e
                inc     hl
                ld      (hl),d
                ret
off_v2:
                ld      hl, PLAY_VOICES
                res     1,(hl)
                ret

upd_v3:
                ld      hl, PLAY_VOICES
                bit     2,(hl)
                ret     z

                ld      hl, VOICE_3
                ld      e,(hl)
                inc     hl
                ld      d,(hl)
                dec     hl

                ld      a,(de)          ; = duration
                inc     de
                ldh     (DUR_3 & 0FFh),a
                cp      0ffh
                jr      z, off_v3
                ld      a,(de)          ; = frequency low
                inc     de
                ld      b,a
                ld      a,(de)          ; = frequency high
                inc     de
                ld      c,a
                or      b
                jr      z, mute_v3      ; if freq=0 -> mute

                ld      a, 080h
                ldh     (NR_30 & 0FFh),a
                ld      a, 000h
                ldh     (NR_31 & 0FFh),a
                ld      a, 000100000b
                ldh     (NR_32 & 0FFh),a
                ld      a, b
                ldh     (NR_33 & 0FFh), a
                ld      a, c
                and     7
                or      080h
                ldh     (NR_34 & 0FFh), a

                ld      (hl),e
                inc     hl
                ld      (hl),d
                ret
mute_v3:

                ld      a, 000h
                ldh     (NR_32 & 0FFh), a

                ld      (hl),e
                inc     hl
                ld      (hl),d
                ret
off_v3:
                ld      hl, PLAY_VOICES
                res     2,(hl)
                ret

; Music, stolen from "Leisure Suit Larry" byte Sierra On-Line
#include "LARRY.INC"
