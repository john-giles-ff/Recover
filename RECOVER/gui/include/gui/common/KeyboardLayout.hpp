#ifndef KEYBOARD_LAYOUT
#define KEYBOARD_LAYOUT

#include <touchgfx/widgets/Keyboard.hpp>
#include <touchgfx/hal/Types.hpp>
#include <fonts/ApplicationFontProvider.hpp>
#include "BitmapDatabase.hpp"

using namespace touchgfx;

/**
 * Array specifying the keys used in the CustomKeyboard.
 */
static const Keyboard::Key keyArray[30] =
{
    { 1, Rect( 10, 75, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //Q
    { 2, Rect( 60, 75, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //W
    { 3, Rect(110, 75, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //E
    { 4, Rect(160, 75, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //R 
    { 5, Rect(210, 75, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //T 
    { 6, Rect(260, 75, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //Y
    { 7, Rect(310, 75, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //U
    { 8, Rect(360, 75, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //I 
    { 9, Rect(410, 75, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //O
    {10, Rect(460, 75, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //P

    {12, Rect( 35, 125, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //A
    {13, Rect( 85, 125, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //S
    {14, Rect(135, 125, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //D
    {15, Rect(185, 125, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //F
    {16, Rect(235, 125, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //G
    {17, Rect(285, 125, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //H
    {18, Rect(335, 125, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //J
    {19, Rect(385, 125, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //K
    {20, Rect(435, 125, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //L

	{23, Rect( 60, 175, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //Z
    {24, Rect(110, 175, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //X
    {25, Rect(160, 175, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //C
    {26, Rect(210, 175, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //V
    {27, Rect(260, 175, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //B
    {28, Rect(310, 175, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //N
    {29, Rect(360, 175, 45, 45), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID}, //M

    {30, Rect(90, 225, 380, 45), BITMAP_KEYBOARD_SPACE_HIGHLIGHTED_ID} //space
};

/**
 * Callback areas for the special buttons on the CustomKeyboard.
 */
static Keyboard::CallbackArea callbackAreas[5] =
{
    {Rect(10, 175, 45, 45), 0, BITMAP_KEYBOARD_KEY_SHIFT_HIGHLIGHTED_ID},	// caps-lock
    {Rect(510, 75, 40, 45), 0, BITMAP_KEYBOARD_KEY_DELETE_HIGHLIGHTED_ID},	// backspace
    {Rect(475, 225, 75, 45), 0, BITMAP_KEYBOARD_KEY_NUM_HIGHLIGHTED_ID},	// mode
	{Rect(10, 225, 75, 45), 0, BITMAP_KEYBOARD_KEY_NUM_HIGHLIGHTED_ID},		// mode
	{Rect(485, 125, 65, 95), 0, BITMAP_KEYBOARD_KEY_ENTER_HIGHLIGHTED_ID}	// enter
};

/**
 * The layout for the CustomKeyboard.
 */
static const Keyboard::Layout layout =
{
    BITMAP_KEYBOARD_BACKGROUND_ID,
    keyArray,
    30,
    callbackAreas,
    5,
    Rect(10, 10, 540, 50),
    TypedText(T_ENTEREDTEXT),
#if !defined(USE_BPP) || USE_BPP==16
    0xFFFF,
#elif USE_BPP==24
    0xFFFFFF,
#elif USE_BPP==4
    0xF,
#elif USE_BPP==2
    0x3,
#else
#error Unknown USE_BPP
#endif
    Typography::KEYBOARD,
    0
};

#endif
