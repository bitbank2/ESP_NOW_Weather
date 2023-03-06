// Created by http://oleddisplay.squix.ch/ Consider a donation
// In case of problems make sure that you are using the font file with the correct version!
const uint8_t Roboto_25Bitmaps[] PROGMEM = {

	// Bitmap Data:
	0x00, // ' '
	0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x00,0x0C,0xEC, // '!'
	0x9A,0x69,0xA6,0x92,0x40, // '"'
	0x03,0x18,0x04,0x20,0x18,0xC0,0x31,0x80,0x63,0x0F,0xFF,0x9F,0xFF,0x06,0x30,0x0C,0x60,0x18,0xC0,0x31,0x87,0xFF,0xCF,0xFF,0x83,0x18,0x06,0x30,0x0C,0x60,0x10,0x80,0x63,0x00, // '#'
	0x06,0x00,0x30,0x01,0x80,0x1F,0x03,0xFC,0x38,0x71,0xC1,0x8C,0x0C,0x60,0x73,0x80,0x0E,0x00,0x3E,0x00,0x7C,0x00,0xF0,0x01,0x80,0x0E,0xE0,0x73,0x03,0x9C,0x38,0x7F,0xC1,0xF8,0x03,0x00,0x18,0x00, // '$'
	0x3C,0x00,0x3F,0x00,0x31,0x82,0x18,0x63,0x0C,0x33,0x06,0x31,0x01,0xF9,0x80,0x79,0x80,0x00,0x80,0x00,0xC0,0x00,0xC7,0x80,0x47,0xE0,0x66,0x30,0x63,0x08,0x21,0x8C,0x30,0xC6,0x00,0x7F,0x00,0x0F,0x00, // '%'
	0x0F,0x00,0x3F,0x80,0xE3,0x01,0x86,0x03,0x0C,0x06,0x18,0x0E,0xE0,0x0F,0x80,0x1E,0x00,0x7C,0x01,0xCC,0x63,0x1C,0xCE,0x1D,0x9C,0x1F,0x38,0x1C,0x38,0x7C,0x3F,0xD8,0x3F,0x38, // '&'
	0xDB,0x6D,0x80, // '''
	0x04,0x18,0x61,0x83,0x0C,0x18,0x70,0xC1,0x83,0x06,0x0C,0x18,0x30,0x60,0xC1,0x83,0x83,0x06,0x06,0x0C,0x0C,0x0C,0x08, // '('
	0x40,0xE0,0x30,0x30,0x18,0x18,0x0C,0x0C,0x0C,0x0E,0x06,0x06,0x06,0x06,0x06,0x0E,0x0E,0x0C,0x0C,0x0C,0x18,0x18,0x30,0x30,0x60,0xC0, // ')'
	0x0C,0x00,0x80,0x10,0x32,0x6F,0xFC,0x1C,0x07,0x80,0xD8,0x31,0x84,0x20, // '*'
	0x06,0x00,0x30,0x01,0x80,0x0C,0x00,0x60,0x7F,0xFB,0xFF,0xC0,0xC0,0x06,0x00,0x30,0x01,0x80,0x0C,0x00,0x60,0x00, // '+'
	0x31,0x9C,0xC6,0x60, // ','
	0xFD,0xF8, // '-'
	0xCE,0xC0, // '.'
	0x01,0xC0,0x30,0x06,0x01,0x80,0x30,0x06,0x01,0x80,0x30,0x0C,0x01,0x80,0x30,0x0C,0x01,0x80,0x30,0x0C,0x01,0x80,0x20,0x0C,0x01,0x80,0x60,0x00, // '/'
	0x1F,0x81,0xFE,0x1C,0x38,0xC0,0xC6,0x06,0x70,0x33,0x81,0xDC,0x0E,0xE0,0x77,0x03,0xB8,0x1D,0xC0,0xE6,0x06,0x30,0x31,0x81,0x8E,0x1C,0x3F,0xC0,0xFC,0x00, // '0'
	0x06,0x3E,0xFE,0xC6,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06, // '1'
	0x1F,0x81,0xFE,0x1C,0x39,0xC0,0xCE,0x06,0x00,0x30,0x01,0x80,0x1C,0x01,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0E,0x00,0xE0,0x0E,0x00,0xFF,0xF7,0xFF,0x80, // '2'
	0x1F,0x03,0xFC,0x70,0xEE,0x06,0xE0,0x60,0x06,0x00,0x60,0x0C,0x0F,0x80,0xFC,0x00,0xE0,0x06,0x00,0x6C,0x06,0xE0,0x67,0x0E,0x3F,0xC1,0xF0, // '3'
	0x01,0xC0,0x0E,0x00,0xF0,0x0F,0x80,0x6C,0x06,0x60,0x73,0x03,0x18,0x30,0xC1,0x86,0x18,0x31,0xC1,0x8F,0xFF,0x7F,0xF8,0x03,0x00,0x18,0x00,0xC0,0x06,0x00, // '4'
	0x7F,0xE7,0xFE,0x60,0x06,0x00,0x60,0x06,0x00,0x7F,0x07,0xF8,0x61,0xC0,0x0E,0x00,0x60,0x06,0x00,0x6C,0x06,0xC0,0xE6,0x1C,0x7F,0x81,0xF0, // '5'
	0x07,0x01,0xF0,0x38,0x07,0x00,0x60,0x0C,0x00,0xDF,0x0F,0xF8,0xF1,0xCC,0x0C,0xC0,0xEC,0x06,0xC0,0x6C,0x0E,0xE0,0xC7,0x1C,0x3F,0x81,0xF0, // '6'
	0xFF,0xF7,0xFF,0x80,0x18,0x00,0xC0,0x0E,0x00,0x60,0x07,0x00,0x30,0x01,0x80,0x18,0x00,0xC0,0x0C,0x00,0x60,0x07,0x00,0x30,0x03,0x80,0x18,0x01,0xC0,0x00, // '7'
	0x0F,0x01,0xFE,0x1C,0x38,0xC0,0xC6,0x06,0x30,0x31,0x81,0x86,0x18,0x1F,0x81,0xFE,0x1C,0x38,0xC0,0xCE,0x07,0x70,0x39,0x81,0xCE,0x1C,0x3F,0xC0,0xFC,0x00, // '8'
	0x1F,0x03,0xF8,0x71,0xC6,0x0E,0xE0,0x6C,0x06,0xC0,0x6E,0x06,0x60,0x67,0x1E,0x3F,0xE1,0xE6,0x00,0x60,0x0E,0x00,0xC0,0x38,0x1F,0x01,0xC0, // '9'
	0xCE,0xC0,0x00,0x00,0x00,0xCE,0xC0, // ':'
	0x30,0xE3,0x00,0x00,0x00,0x00,0x00,0x03,0x0C,0x71,0x86,0x30, // ';'
	0x00,0x40,0x38,0x3F,0x1F,0x0F,0x81,0x80,0x3E,0x01,0xF0,0x0F,0xC0,0x38,0x01,0x00, // '<'
	0xFF,0xDF,0xF8,0x00,0x00,0x00,0x01,0xFF,0xBF,0xF0, // '='
	0x80,0x1C,0x03,0xE0,0x0F,0x80,0x7C,0x01,0x81,0xF0,0xF8,0xF8,0x1C,0x02,0x00,0x00, // '>'
	0x3E,0x0F,0xF3,0x86,0x60,0xE0,0x0C,0x03,0x80,0x60,0x1C,0x07,0x01,0xC0,0x30,0x0E,0x01,0xC0,0x00,0x00,0x00,0x60,0x1C,0x01,0x80, // '?'
	0x00,0xFC,0x00,0x3F,0xF8,0x03,0xC0,0xF0,0x38,0x01,0x83,0x80,0x06,0x18,0x3C,0x11,0x83,0xF8,0xCC,0x38,0x86,0x61,0x84,0x12,0x18,0x60,0xB0,0xC3,0x05,0x86,0x18,0x2C,0x30,0xC3,0x21,0x86,0x19,0x0C,0x71,0x8C,0x7F,0xFC,0x61,0xE7,0x81,0x80,0x00,0x0E,0x00,0x00,0x3C,0x08,0x00,0xFF,0xC0,0x00,0xFC,0x00, // '@'
	0x01,0x80,0x01,0xE0,0x00,0xF0,0x00,0x78,0x00,0x66,0x00,0x33,0x00,0x19,0x80,0x18,0x60,0x0C,0x30,0x0E,0x1C,0x06,0x06,0x03,0xFF,0x03,0xFF,0xC1,0x80,0x61,0xC0,0x38,0xC0,0x0C,0x60,0x06,0x70,0x03,0x80, // 'A'
	0xFF,0x87,0xFE,0x30,0x39,0x80,0xEC,0x07,0x60,0x3B,0x01,0x98,0x1C,0xFF,0xC7,0xFE,0x30,0x19,0x80,0xEC,0x03,0x60,0x1B,0x01,0xD8,0x1E,0xFF,0xE7,0xFC,0x00, // 'B'
	0x07,0xC0,0x3F,0xE0,0xE0,0xE3,0x80,0xE7,0x01,0xCC,0x01,0x98,0x00,0x70,0x00,0xE0,0x01,0xC0,0x03,0x80,0x03,0x00,0x06,0x00,0xCC,0x03,0x9C,0x07,0x1C,0x1C,0x1F,0xF0,0x0F,0x80, // 'C'
	0xFF,0x03,0xFF,0x0C,0x0E,0x30,0x1C,0xC0,0x33,0x00,0xEC,0x03,0xB0,0x06,0xC0,0x1B,0x00,0x6C,0x01,0xB0,0x0E,0xC0,0x3B,0x00,0xCC,0x07,0x30,0x78,0xFF,0xC3,0xFC,0x00, // 'D'
	0xFF,0xEF,0xFE,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xFF,0xCF,0xFC,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xFF,0xEF,0xFE, // 'E'
	0xFF,0xEF,0xFE,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xFF,0xCF,0xFC,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0C,0x00, // 'F'
	0x07,0xC0,0x3F,0xE0,0xE0,0xE3,0x80,0xE7,0x00,0xCC,0x00,0x18,0x00,0x70,0x00,0xE0,0x01,0xC1,0xF9,0x83,0xF3,0x00,0x66,0x00,0xCE,0x01,0x8C,0x03,0x1E,0x0E,0x1F,0xF8,0x0F,0xC0, // 'G'
	0xC0,0x1D,0x80,0x3B,0x00,0x76,0x00,0xEC,0x01,0xD8,0x03,0xB0,0x07,0x60,0x0E,0xFF,0xFD,0xFF,0xFB,0x00,0x76,0x00,0xEC,0x01,0xD8,0x03,0xB0,0x07,0x60,0x0E,0xC0,0x1D,0x80,0x38, // 'H'
	0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE,0xEE, // 'I'
	0x00,0x60,0x06,0x00,0x60,0x06,0x00,0x60,0x06,0x00,0x60,0x06,0x00,0x60,0x06,0x00,0x60,0x06,0x00,0x6C,0x0E,0xC0,0xEE,0x1C,0x7F,0x81,0xF0, // 'J'
	0xC0,0x39,0x80,0xE3,0x03,0x86,0x0E,0x0C,0x38,0x18,0xE0,0x31,0x80,0x67,0x00,0xDE,0x01,0xFC,0x03,0xDC,0x07,0x1C,0x0C,0x1C,0x18,0x38,0x30,0x38,0x60,0x38,0xC0,0x39,0x80,0x78, // 'K'
	0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xFF,0xEF,0xFE, // 'L'
	0xE0,0x01,0xDE,0x00,0x7B,0xC0,0x0F,0x78,0x03,0xED,0x80,0x6D,0xB0,0x0D,0xB7,0x03,0x36,0x60,0x6E,0xCC,0x1D,0xD8,0xC3,0x3B,0x18,0x67,0x63,0x98,0xEC,0x33,0x1D,0x86,0xE3,0xB0,0x78,0x76,0x0F,0x0E,0xC1,0xC1,0xD8,0x18,0x38, // 'M'
	0xE0,0x0D,0xE0,0x1B,0xC0,0x37,0xC0,0x6F,0x80,0xDF,0x81,0xBB,0x83,0x73,0x06,0xE7,0x0D,0xC7,0x1B,0x86,0x37,0x0E,0x6E,0x0E,0xDC,0x0D,0xB8,0x1F,0x70,0x1E,0xE0,0x1D,0xC0,0x38, // 'N'
	0x07,0xC0,0x1F,0xF0,0x38,0x38,0x30,0x1C,0x70,0x0C,0x60,0x0C,0x60,0x0E,0xE0,0x0E,0xE0,0x0E,0xE0,0x0E,0xE0,0x0E,0x60,0x0E,0x60,0x0C,0x70,0x0C,0x30,0x1C,0x38,0x38,0x1F,0xF0,0x07,0xC0, // 'O'
	0xFF,0xC3,0xFF,0x8E,0x07,0x38,0x0E,0xE0,0x3B,0x80,0x6E,0x01,0xB8,0x0E,0xE0,0x73,0xFF,0x8F,0xFC,0x38,0x00,0xE0,0x03,0x80,0x0E,0x00,0x38,0x00,0xE0,0x03,0x80,0x00, // 'P'
	0x07,0xC0,0x1F,0xF0,0x38,0x38,0x70,0x18,0x60,0x1C,0x60,0x0C,0xE0,0x0C,0xE0,0x0C,0xE0,0x0E,0xE0,0x0E,0xE0,0x0E,0xE0,0x0C,0x60,0x0C,0x60,0x1C,0x70,0x18,0x38,0x38,0x1F,0xF0,0x07,0xF0,0x00,0x3C,0x00,0x1E,0x00,0x08, // 'Q'
	0xFF,0x83,0xFF,0x0C,0x0E,0x30,0x1C,0xC0,0x73,0x00,0xCC,0x07,0x30,0x1C,0xC0,0xE3,0xFF,0x0F,0xF8,0x30,0x60,0xC1,0xC3,0x03,0x0C,0x0E,0x30,0x18,0xC0,0x73,0x00,0xE0, // 'R'
	0x0F,0x80,0xFF,0x87,0x07,0x18,0x0E,0x60,0x39,0x80,0x07,0x00,0x0E,0x00,0x1F,0x00,0x1F,0x80,0x0F,0x00,0x0E,0x00,0x3B,0x80,0x6E,0x03,0x9C,0x1C,0x3F,0xF0,0x3F,0x00, // 'S'
	0xFF,0xFB,0xFF,0xE0,0x70,0x01,0xC0,0x07,0x00,0x1C,0x00,0x70,0x01,0xC0,0x07,0x00,0x1C,0x00,0x70,0x01,0xC0,0x07,0x00,0x1C,0x00,0x70,0x01,0xC0,0x07,0x00,0x1C,0x00, // 'T'
	0xC0,0x36,0x01,0xB0,0x0D,0x80,0x6C,0x03,0x60,0x1B,0x00,0xD8,0x06,0xC0,0x36,0x01,0xB0,0x0D,0x80,0x6C,0x03,0x60,0x1B,0x81,0xCE,0x1C,0x7F,0xE0,0xFC,0x00, // 'U'
	0xE0,0x07,0x30,0x03,0x1C,0x03,0x86,0x01,0x83,0x00,0xC1,0xC0,0xE0,0x60,0x60,0x30,0x30,0x1C,0x38,0x06,0x18,0x03,0x9C,0x00,0xCC,0x00,0x66,0x00,0x3F,0x00,0x0F,0x00,0x07,0x80,0x03,0xC0,0x00,0xC0,0x00, // 'V'
	0xC0,0x60,0x3B,0x01,0xC0,0xCE,0x07,0x03,0x18,0x3C,0x1C,0x60,0xF0,0x71,0x83,0x61,0x87,0x1D,0x86,0x0C,0x66,0x18,0x31,0x8C,0xE0,0xC6,0x33,0x03,0x30,0xCC,0x0E,0xC3,0x30,0x1B,0x07,0xC0,0x78,0x1E,0x01,0xE0,0x78,0x07,0x80,0xE0,0x0E,0x03,0x80,0x30,0x0C,0x00, // 'W'
	0xE0,0x1C,0xE0,0x71,0xC0,0xC1,0xC3,0x81,0x8E,0x03,0x98,0x03,0xF0,0x03,0xC0,0x07,0x80,0x0F,0x00,0x3E,0x00,0x7E,0x01,0xCE,0x03,0x1C,0x0E,0x1C,0x38,0x18,0x60,0x39,0xC0,0x38, // 'X'
	0xE0,0x0E,0x70,0x1C,0x30,0x18,0x38,0x38,0x18,0x30,0x1C,0x70,0x0C,0x60,0x0E,0xE0,0x06,0xC0,0x07,0xC0,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80,0x03,0x80, // 'Y'
	0xFF,0xFB,0xFF,0xE0,0x07,0x00,0x18,0x00,0xE0,0x07,0x00,0x18,0x00,0xE0,0x07,0x00,0x18,0x00,0xE0,0x07,0x00,0x18,0x00,0xE0,0x07,0x00,0x18,0x00,0xFF,0xFB,0xFF,0xE0, // 'Z'
	0xF7,0xB1,0x8C,0x63,0x18,0xC6,0x31,0x8C,0x63,0x18,0xC6,0x31,0x8C,0x63,0xDE, // '['
	0xE0,0x0C,0x01,0xC0,0x18,0x03,0x00,0x30,0x06,0x00,0xC0,0x0C,0x01,0x80,0x38,0x03,0x00,0x60,0x0E,0x00,0xC0,0x18,0x01,0x80,0x30,0x07,0x00,0x60, // '\'
	0xFB,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xEF,0xBE, // ']'
	0x18,0x0C,0x0F,0x07,0x86,0x63,0x31,0x99,0x86,0xC3,0x00, // '^'
	0xFF,0xEF,0xFE, // '_'
	0xE1,0x83,0x0E, // '`'
	0x3F,0x0F,0xF3,0x87,0x60,0x60,0x0C,0x7F,0x9F,0xF7,0x06,0xC0,0xD8,0x1B,0x8F,0x7F,0xE3,0xCC, // 'a'
	0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xDF,0x0F,0xF8,0xE1,0xCC,0x0C,0xC0,0xEC,0x06,0xC0,0x6C,0x06,0xC0,0xEC,0x0C,0xE1,0xCF,0xF8,0xDF,0x00, // 'b'
	0x1F,0x03,0xFC,0x70,0xE6,0x06,0xE0,0x6C,0x00,0xC0,0x0C,0x00,0xE0,0x06,0x06,0x70,0xE3,0xFC,0x1F,0x00, // 'c'
	0x00,0x60,0x06,0x00,0x60,0x06,0x00,0x60,0x06,0x1F,0x63,0xFE,0x70,0xE6,0x06,0xE0,0x6C,0x06,0xC0,0x6C,0x06,0xE0,0x66,0x06,0x70,0xE3,0xFE,0x1F,0x60, // 'd'
	0x1F,0x03,0xFC,0x70,0xC6,0x06,0xC0,0x6F,0xFE,0xFF,0xEC,0x00,0xE0,0x0E,0x00,0x70,0xE3,0xFC,0x1F,0x80, // 'e'
	0x0F,0x0F,0x8E,0x06,0x03,0x01,0x83,0xF9,0xFC,0x30,0x18,0x0C,0x06,0x03,0x01,0x80,0xC0,0x60,0x30,0x18,0x0C,0x00, // 'f'
	0x1F,0x63,0xFE,0x70,0xE6,0x06,0xE0,0x6C,0x06,0xC0,0x6C,0x06,0xE0,0x66,0x06,0x70,0xE3,0xFE,0x1F,0x60,0x06,0x40,0x66,0x0E,0x3F,0xC1,0xF0, // 'g'
	0xC0,0x18,0x03,0x00,0x60,0x0C,0x01,0x80,0x33,0xC7,0xFC,0xF1,0xD8,0x1B,0x03,0x60,0x6C,0x0D,0x81,0xB0,0x36,0x06,0xC0,0xD8,0x1B,0x03,0x00, // 'h'
	0xDB,0x01,0xB6,0xDB,0x6D,0xB6,0xD8, // 'i'
	0x18,0x61,0x80,0x00,0x61,0x86,0x18,0x61,0x86,0x18,0x61,0x86,0x18,0x61,0x86,0x3B,0xEF,0x00, // 'j'
	0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0C,0x00,0xC1,0xCC,0x38,0xC7,0x0C,0xE0,0xDC,0x0F,0x80,0xFC,0x0E,0xC0,0xC6,0x0C,0x70,0xC3,0x8C,0x1C,0xC0,0xE0, // 'k'
	0xDB,0x6D,0xB6,0xDB,0x6D,0xB6,0xDB,0x00, // 'l'
	0xDF,0x1F,0x1F,0xF7,0xF3,0x87,0x87,0x60,0x60,0x6C,0x0C,0x0D,0x81,0x81,0xB0,0x30,0x36,0x06,0x06,0xC0,0xC0,0xD8,0x18,0x1B,0x03,0x03,0x60,0x60,0x6C,0x0C,0x0C, // 'm'
	0xCF,0x1F,0xF3,0xC7,0x60,0x6C,0x0D,0x81,0xB0,0x36,0x06,0xC0,0xD8,0x1B,0x03,0x60,0x6C,0x0C, // 'n'
	0x1F,0x81,0xFE,0x1C,0x38,0xC0,0xCE,0x07,0x60,0x1B,0x00,0xD8,0x06,0xE0,0x73,0x03,0x1C,0x38,0x7F,0x81,0xF8,0x00, // 'o'
	0xDF,0x0F,0xF8,0xE1,0xCC,0x0C,0xC0,0xEC,0x06,0xC0,0x6C,0x06,0xC0,0xEC,0x0C,0xE1,0xCF,0xF8,0xDF,0x0C,0x00,0xC0,0x0C,0x00,0xC0,0x0C,0x00, // 'p'
	0x1F,0x63,0xFE,0x70,0xEE,0x06,0xE0,0x6C,0x06,0xC0,0x6C,0x06,0xE0,0x66,0x06,0x70,0xE3,0xFE,0x1F,0x60,0x06,0x00,0x60,0x06,0x00,0x60,0x06, // 'q'
	0xEE,0xFE,0xF0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0, // 'r'
	0x1F,0x03,0xFC,0x71,0xC6,0x0E,0x60,0x07,0xE0,0x1F,0x80,0x3C,0x00,0xEE,0x0E,0x70,0xC7,0xFC,0x1F,0x00, // 's'
	0x38,0x38,0x38,0xFE,0xFE,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x1E,0x0E, // 't'
	0xC0,0xD8,0x1B,0x03,0x60,0x6C,0x0D,0x81,0xB0,0x36,0x06,0xC0,0xD8,0x1B,0x87,0x3F,0xE3,0xEC, // 'u'
	0xE0,0x73,0x03,0x1C,0x38,0x61,0x83,0x0C,0x1C,0xE0,0x66,0x03,0x30,0x1F,0x80,0x78,0x03,0xC0,0x0C,0x00,0x60,0x00, // 'v'
	0xC1,0xC1,0xB0,0x70,0x6E,0x1C,0x39,0x8D,0x0C,0x63,0x63,0x18,0xD8,0xC3,0x62,0x60,0xD8,0xD8,0x36,0x36,0x0D,0x05,0x81,0xC1,0xC0,0x70,0x70,0x18,0x1C,0x00, // 'w'
	0xE0,0xE6,0x1C,0x71,0x83,0x30,0x1F,0x01,0xE0,0x0C,0x01,0xE0,0x1F,0x03,0x30,0x71,0x86,0x1C,0xC0,0xE0, // 'x'
	0xE0,0x73,0x03,0x1C,0x38,0x61,0x83,0x0C,0x1C,0xE0,0x66,0x03,0x30,0x0F,0x80,0x78,0x03,0xC0,0x0E,0x00,0x60,0x03,0x00,0x30,0x01,0x80,0x78,0x03,0x80,0x00, // 'y'
	0xFF,0xCF,0xFC,0x01,0xC0,0x38,0x07,0x00,0x60,0x0C,0x01,0xC0,0x38,0x07,0x00,0x60,0x0F,0xFE,0xFF,0xE0, // 'z'
	0x06,0x0E,0x18,0x38,0x38,0x30,0x30,0x30,0x30,0x30,0x70,0xE0,0xE0,0x70,0x30,0x30,0x30,0x30,0x30,0x30,0x38,0x18,0x0E,0x06, // '{'
	0xDB,0x6D,0xB6,0xDB,0x6D,0xB6,0xDB,0x6C, // '|'
	0xC0,0xE0,0x30,0x30,0x38,0x18,0x18,0x18,0x18,0x18,0x1C,0x0E,0x0E,0x1C,0x18,0x18,0x18,0x18,0x18,0x38,0x30,0x30,0xE0,0xC0 // '}'
};
const GFXglyph Roboto_25Glyphs[] PROGMEM = {
// bitmapOffset, width, height, xAdvance, xOffset, yOffset
	  {     0,   1,   1,   7,    0,    0 }, // ' '
	  {     1,   4,  18,   7,    2,  -18 }, // '!'
	  {    10,   6,   6,   9,    2,  -19 }, // '"'
	  {    15,  15,  18,  16,    1,  -18 }, // '#'
	  {    49,  13,  23,  15,    1,  -21 }, // '$'
	  {    87,  17,  18,  19,    1,  -18 }, // '%'
	  {   126,  15,  18,  17,    1,  -18 }, // '&'
	  {   160,   3,   6,   5,    1,  -19 }, // '''
	  {   163,   7,  26,  10,    2,  -20 }, // '('
	  {   186,   8,  26,  10,    0,  -20 }, // ')'
	  {   212,  11,  10,  12,    0,  -18 }, // '*'
	  {   226,  13,  13,  15,    1,  -15 }, // '+'
	  {   248,   5,   6,   6,    0,   -3 }, // ','
	  {   252,   7,   2,   8,    0,   -8 }, // '-'
	  {   254,   4,   3,   8,    2,   -3 }, // '.'
	  {   256,  11,  20,  11,    0,  -18 }, // '/'
	  {   284,  13,  18,  15,    1,  -18 }, // '0'
	  {   314,   8,  18,  15,    2,  -18 }, // '1'
	  {   332,  13,  18,  15,    1,  -18 }, // '2'
	  {   362,  12,  18,  15,    1,  -18 }, // '3'
	  {   389,  13,  18,  15,    1,  -18 }, // '4'
	  {   419,  12,  18,  15,    2,  -18 }, // '5'
	  {   446,  12,  18,  15,    2,  -18 }, // '6'
	  {   473,  13,  18,  15,    1,  -18 }, // '7'
	  {   503,  13,  18,  15,    1,  -18 }, // '8'
	  {   533,  12,  18,  15,    1,  -18 }, // '9'
	  {   560,   4,  13,   7,    2,  -13 }, // ':'
	  {   567,   6,  16,   6,    0,  -13 }, // ';'
	  {   579,  11,  11,  14,    1,  -13 }, // '<'
	  {   595,  11,   7,  15,    2,  -12 }, // '='
	  {   605,  11,  11,  14,    2,  -13 }, // '>'
	  {   621,  11,  18,  13,    1,  -18 }, // '?'
	  {   646,  21,  22,  23,    1,  -17 }, // '@'
	  {   704,  17,  18,  17,    0,  -18 }, // 'A'
	  {   743,  13,  18,  17,    2,  -18 }, // 'B'
	  {   773,  15,  18,  17,    1,  -18 }, // 'C'
	  {   807,  14,  18,  17,    2,  -18 }, // 'D'
	  {   839,  12,  18,  15,    2,  -18 }, // 'E'
	  {   866,  12,  18,  15,    2,  -18 }, // 'F'
	  {   893,  15,  18,  18,    1,  -18 }, // 'G'
	  {   927,  15,  18,  19,    2,  -18 }, // 'H'
	  {   961,   4,  18,   8,    2,  -18 }, // 'I'
	  {   970,  12,  18,  15,    1,  -18 }, // 'J'
	  {   997,  15,  18,  17,    2,  -18 }, // 'K'
	  {  1031,  12,  18,  14,    2,  -18 }, // 'L'
	  {  1058,  19,  18,  23,    2,  -18 }, // 'M'
	  {  1101,  15,  18,  19,    2,  -18 }, // 'N'
	  {  1135,  16,  18,  18,    1,  -18 }, // 'O'
	  {  1171,  14,  18,  17,    2,  -18 }, // 'P'
	  {  1203,  16,  21,  18,    1,  -18 }, // 'Q'
	  {  1245,  14,  18,  16,    2,  -18 }, // 'R'
	  {  1277,  14,  18,  16,    1,  -18 }, // 'S'
	  {  1309,  14,  18,  16,    1,  -18 }, // 'T'
	  {  1341,  13,  18,  17,    2,  -18 }, // 'U'
	  {  1371,  17,  18,  17,    0,  -18 }, // 'V'
	  {  1410,  22,  18,  23,    1,  -18 }, // 'W'
	  {  1460,  15,  18,  17,    1,  -18 }, // 'X'
	  {  1494,  16,  18,  16,    0,  -18 }, // 'Y'
	  {  1530,  14,  18,  16,    1,  -18 }, // 'Z'
	  {  1562,   5,  24,   8,    2,  -20 }, // '['
	  {  1577,  11,  20,  11,    0,  -18 }, // '\'
	  {  1605,   6,  24,   8,    0,  -20 }, // ']'
	  {  1623,   9,   9,  11,    1,  -18 }, // '^'
	  {  1634,  12,   2,  12,    0,    0 }, // '_'
	  {  1637,   6,   4,   9,    1,  -19 }, // '`'
	  {  1640,  11,  13,  15,    2,  -13 }, // 'a'
	  {  1658,  12,  19,  15,    2,  -19 }, // 'b'
	  {  1687,  12,  13,  14,    1,  -13 }, // 'c'
	  {  1707,  12,  19,  15,    1,  -19 }, // 'd'
	  {  1736,  12,  13,  14,    1,  -13 }, // 'e'
	  {  1756,   9,  19,  10,    1,  -19 }, // 'f'
	  {  1778,  12,  18,  15,    1,  -13 }, // 'g'
	  {  1805,  11,  19,  15,    2,  -19 }, // 'h'
	  {  1832,   3,  18,   7,    2,  -18 }, // 'i'
	  {  1839,   6,  23,   7,   -1,  -18 }, // 'j'
	  {  1857,  12,  19,  14,    2,  -19 }, // 'k'
	  {  1886,   3,  19,   7,    2,  -19 }, // 'l'
	  {  1894,  19,  13,  23,    2,  -13 }, // 'm'
	  {  1925,  11,  13,  15,    2,  -13 }, // 'n'
	  {  1943,  13,  13,  15,    1,  -13 }, // 'o'
	  {  1965,  12,  18,  15,    2,  -13 }, // 'p'
	  {  1992,  12,  18,  15,    1,  -13 }, // 'q'
	  {  2019,   8,  13,   9,    1,  -13 }, // 'r'
	  {  2032,  12,  13,  14,    1,  -13 }, // 's'
	  {  2052,   8,  16,   9,    0,  -16 }, // 't'
	  {  2068,  11,  13,  15,    2,  -13 }, // 'u'
	  {  2086,  13,  13,  13,    0,  -13 }, // 'v'
	  {  2108,  18,  13,  20,    1,  -13 }, // 'w'
	  {  2138,  12,  13,  13,    1,  -13 }, // 'x'
	  {  2158,  13,  18,  13,    0,  -13 }, // 'y'
	  {  2188,  12,  13,  13,    1,  -13 }, // 'z'
	  {  2208,   8,  24,   9,    1,  -20 }, // '{'
	  {  2232,   3,  21,   7,    2,  -18 }, // '|'
	  {  2240,   8,  24,   9,    0,  -20 } // '}'
};
const GFXfont Roboto_25 PROGMEM = {
(uint8_t  *)Roboto_25Bitmaps,(GFXglyph *)Roboto_25Glyphs,0x20, 0x7E, 31};
