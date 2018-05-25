; All text used in this demo:

man_start       .byte "Start",0
man_options     .byte "Options",0
man_contact     .byte "Contact me !",0
man_copy1       .byte 17," Jens Ch.",0
man_copy2       .byte "Restemeier",0    

opt_title       .byte "Options:",0
opt_sound       .byte "Sound:",0
opt_color       .byte "Color:",0
opt_test        .byte "Test joypad",0
opt_type        .byte "System type:",0
opt_gb          .byte "Classic Game Boy",0
opt_pgb         .byte "Game Boy pocket ",0
opt_sgb         .byte "Super Game Boy",0
opt_cgb         .byte "Game Boy Color",0
opt_unknown     .byte "Unknown ?",0

adr_title       .byte "Address:",0
adr_nam1        .byte "Jens Christian",0
adr_nam2        .byte "Restemeier",0
adr_street      .byte "Bahnhofstra",19,"e 75",0
adr_town        .byte "32257 B",18,"nde",0
adr_country     .byte "Germany",0
adr_email       .byte "XXXXXXXXXXXX",0
adr_print       .byte "Print",0

tst_title       .byte "Joypad test",0
tst_value       .byte "Value:",0
tst_up          .byte "Up:",0
tst_down        .byte "Down:",0
tst_left        .byte "Left:",0
tst_right       .byte "Right:",0
tst_start       .byte "Start:",0
tst_select      .byte "Select:",0
tst_a           .byte "A:",0
tst_b           .byte "B:",0
tst_exit        .byte "Press A+B to exit",0

gen_on          .byte "on ",0
gen_off         .byte "off",0
gen_na          .byte "n/a",0
gen_exit        .byte "Exit",0

prn_con         .byte "Printing...",0
prn_bat         .byte "low batteries !",0
prn_jam         .byte "Paper jam !",0
prn_temp        .byte "Temperature !",0
prn_link        .byte "No printer found !",0
prn_none        .byte "                  ",0

apa_start       .byte "- press start -",0

game_over       .byte "Game Over",0

s_scroll_1:
;     |                    |
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "Hi !                "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "I want to apply for "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "a job as a          "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte " Game Boy Color     "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "and                 "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte " System Tools       "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "Engineer.           "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "You can control     "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "this scroller by    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "using the up and    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "down buttons, the   "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "START or the 'A'    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "button will bring   "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "you to the next     "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "screen.             "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "This is a small     "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "demonstration of my "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "programming skills  "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "on the Game Boy.    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "Let me tell you a   "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "little about myself:"
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "I started programm- "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "ing with some       "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "computers in public "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "display at a local  "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "shop. I learned     "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "BASIC on a SVI 328  "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "and SVI 318.        "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "These SVI computers "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "were preccessors of "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "the MSX series of   "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "computers.          "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "My first own        "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "computer was a      "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "Sinclair ZX81       "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "(Timex), which I    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "programmed mostly   "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "in BASIC.           "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "The next Computer   "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "was a Commodore     "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "C128, which I       "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "programmed in BASIC "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "and Assembler, and  "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "later mostly in     "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "Turbo Pascal on     "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "CP/M.               "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "Then I started with "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "PC programming on   "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "MS/DOS, first a     "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "286, then a 386 and "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "a 486.              "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "I programmed first  "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "in Turbo Pascal     "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "with Assembler      "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "subroutines, now    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "mostly in 'C' (gcc)."
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "Now I have an Apple "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "PowerMac and a AMD  "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "K6, both running    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "RedHat Linux.       "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "My first Videogame  "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "was a Nintendo      "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "Game Boy, later     "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "followed a Sony     "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "PlayStation, a      "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "PC-Engine and a     "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "SEGA Saturn.        "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "But the only system "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "I programmed is     "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "the Game Boy.       "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "One last thing: I   "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "showed this demo to "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "a lot of my friends,"
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "but nobody knew     "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "where the music was "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "from. I was quite   "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "surprised by this,  "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "if you haven't      "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "guessed it: it is   "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "from Sierra's       "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "Leisure Suit Larry. "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "  - press start -   "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
e_scroll_1:

adr_text:
.byte "Address:            "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "Jens Ch. Restemeier "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "XXXXXXXXXXXXXXXXXXXX"
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "XXXXXXXXXXXXXXXXXXXX" 
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "Germany             "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "E-Mail:             "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "XXXXXXXXXXXX      "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
.byte "                    "
.byte 0,0,0,0,0,0,0,0,0,0,0,0
