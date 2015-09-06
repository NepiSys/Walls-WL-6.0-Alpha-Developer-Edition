#define Version  "6.0"
#define OSName  "Walls"
#define OSID  "WL Test Beta"

#include "user32.as"
#func PrintWindow "PrintWindow" int, int, int
#func SetLayered "SetLayeredWindowAttributes" int , int , int , int 

#include "gdi32.as"
#define ctype RGB(%1,%2,%3) (%1 | %2 << 8 | %3 << 16)
#define WM_CTLCOLOREDIT $133
#define ES_PASSWORD     $00000020
#define WS_EX_CLIENTEDGE	$00000200
#define WS_CHILD		$40000000
#define WS_VISIBLE		$10000000

#define global ldima(%1) dimtype %1,1

