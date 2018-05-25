; ============================================================================
;  GAMEBOY.INC
; ============================================================================
;  This file includes symbols for the internal registers of a Game Boy.
; ============================================================================

P1              = 0FF00h ; Register for reading joy pad info.    (R/W)
JOYPAD          = P1
SB              = 0FF01h ; Serial transfer data (R/W)
SC              = 0FF02h ; SIO control (R/W)
DIV             = 0FF04h ; Divider Register (R/W)
TIMA            = 0FF05h ; Timer counter (R/W)
TMA             = 0FF06h ; Timer Modulo (R/W)
TAC             = 0FF07h ; Timer Control
IF              = 0FF0Fh ; Interrupt Flag (R/W)
IE              = 0FFFFh ; Interrupt Enable (R/W)
LCDC            = 0FF40h ; LCD Control (R/W)
STAT            = 0FF41h ; LCDC Status   (R/W)  
SCY             = 0FF42h ; Scroll Y   (R/W)
SCX             = 0FF43h ; Scroll X   (R/W)
LY              = 0FF44h ; LCDC Y-Coordinate (R)
LYC             = 0FF45h ; LY Compare  (R/W)
SPRDMA          = 0FF46h ; Sprite DMA
BGP             = 0FF47h ; BG Palette Data  (W)
OBP0            = 0FF48h ; Object Palette 0 Data (W)
OBP1            = 0FF49h ; Object Palette 1 Data (W)
WY              = 0FF4Ah ; Window Y Position  (R/W)
WX              = 0FF4Bh ; Window X Position  (R/W)
DMA             = 0FF46h ; DMA Transfer and Start Address (W)
BCPS            = 0FF68h ; BG Write Specification
BCPD            = 0FF69h ; BG Write Data
OCPS            = 0FF6Ah ; OBJ Write Specification
OCPD            = 0FF6Bh ; OBJ Write Data
KEY1            = 0FF4dh ; 
VBK             = 0FF4Fh ; LCD-Display RAM Bank Register

NR_10           = 0FF10h ; Sound Mode 1 register, Sweep register (R/W)
NR_11           = 0FF11h ; Sound Mode 1 register, Sound length/Wave pattern duty (R/W)
NR_12           = 0FF12h ; Sound Mode 1 register, Envelope (R/W)
NR_13           = 0FF13h ; Sound Mode 1 register, Frequency lo (W)
NR_14           = 0FF14h ; Sound Mode 1 register, Frequency hi (R/W)
NR_20           = 0FF15h ; Sound Mode 1 register, Sweep register (R/W)
NR_21           = 0FF16h ; Sound Mode 2 register, Sound Length/Wave Pattern duty (R/W)
NR_22           = 0FF17h ; Sound Mode 2 register, envelope (R/W)
NR_23           = 0FF18h ; Sound Mode 2 register, frequency lo data (W)
NR_24           = 0FF19h ; Sound Mode 2 register, frequency hi data (R/W)
NR_30           = 0FF1Ah ; Sound Mode 3 register, Sound on/off (R/W)
NR_31           = 0FF1Bh ; Sound Mode 3 register, sound length (R/W)
NR_32           = 0FF1Ch ; Sound Mode 3 register, Select output level
NR_33           = 0FF1Dh ; Sound Mode 3 register, frequency's lower data (W)
NR_34           = 0FF1Eh ; Sound Mode 3 register, frequency's higher data (R/W)
NR_41           = 0FF20h ; Sound Mode 4 register, sound length (R/W)
NR_42           = 0FF21h ; Sound Mode 4 register, envelope (R/W)
NR_43           = 0FF22h ; Sound Mode 4 register, polynomial counter (R/W)
NR_44           = 0FF23h ; Sound Mode 4 register, counter/consecutive; inital (R/W)
NR_50           = 0FF24h ; Channel control / ON-OFF / Volume (R/W)
NR_51           = 0FF25h ; Selection of Sound output terminal (R/W)
NR_52           = 0FF26h ; Sound on/off (R/W)

WAVE            = 0FF30h ; Wave Pattern RAM is located from $FF30-$FF3f
MMU_BASE        = 02100h
