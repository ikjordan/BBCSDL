/*
 *  Copyright (C) 2002-2010  The DOSBox Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/* $Id: int10_memory.cpp,v 1.30 2009-09-06 19:25:34 c2woody Exp $ */

/*  Memotech-Bill: For BBC Basic on Pico
    Codepage 437. Font expanded from 8x8 to 8x10.
    Top and bottom rows of box drawing characters (179 - 218) duplicated.
    Each scan row bit reversed. */

static const unsigned char font_10[256][10] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},    //   0
    { 0x00, 0x7E, 0x81, 0xA5, 0x81, 0xBD, 0x99, 0x81, 0x7E, 0x00},    //   1
    { 0x00, 0x7E, 0xFF, 0xDB, 0xFF, 0xC3, 0xE7, 0xFF, 0x7E, 0x00},    //   2
    { 0x00, 0x36, 0x7F, 0x7F, 0x7F, 0x3E, 0x1C, 0x08, 0x00, 0x00},    //   3
    { 0x00, 0x08, 0x1C, 0x3E, 0x7F, 0x3E, 0x1C, 0x08, 0x00, 0x00},    //   4
    { 0x00, 0x1C, 0x3E, 0x1C, 0x7F, 0x7F, 0x3E, 0x1C, 0x3E, 0x00},    //   5
    { 0x00, 0x08, 0x08, 0x1C, 0x3E, 0x7F, 0x3E, 0x1C, 0x3E, 0x00},    //   6
    { 0x00, 0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00, 0x00},    //   7
    { 0x00, 0xFF, 0xFF, 0xE7, 0xC3, 0xC3, 0xE7, 0xFF, 0xFF, 0x00},    //   8
    { 0x00, 0x00, 0x3C, 0x66, 0x42, 0x42, 0x66, 0x3C, 0x00, 0x00},    //   9
    { 0x00, 0xFF, 0xC3, 0x99, 0xBD, 0xBD, 0x99, 0xC3, 0xFF, 0x00},    //  10
    { 0x00, 0xF0, 0xE0, 0xF0, 0xBE, 0x33, 0x33, 0x33, 0x1E, 0x00},    //  11
    { 0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x7E, 0x18, 0x00},    //  12
    { 0x00, 0xFC, 0xCC, 0xFC, 0x0C, 0x0C, 0x0E, 0x0F, 0x07, 0x00},    //  13
    { 0x00, 0xFE, 0xC6, 0xFE, 0xC6, 0xC6, 0xE6, 0x67, 0x03, 0x00},    //  14
    { 0x00, 0x99, 0x5A, 0x3C, 0xE7, 0xE7, 0x3C, 0x5A, 0x99, 0x00},    //  15
    { 0x00, 0x01, 0x07, 0x1F, 0x7F, 0x1F, 0x07, 0x01, 0x00, 0x00},    //  16
    { 0x00, 0x40, 0x70, 0x7C, 0x7F, 0x7C, 0x70, 0x40, 0x00, 0x00},    //  17
    { 0x00, 0x18, 0x3C, 0x7E, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00},    //  18
    { 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x00, 0x00},    //  19
    { 0x00, 0xFE, 0xDB, 0xDB, 0xDE, 0xD8, 0xD8, 0xD8, 0x00, 0x00},    //  20
    { 0x00, 0x7C, 0xC6, 0x1C, 0x36, 0x36, 0x1C, 0x33, 0x1E, 0x00},    //  21
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x7E, 0x00, 0x00},    //  22
    { 0x00, 0x18, 0x3C, 0x7E, 0x18, 0x7E, 0x3C, 0x18, 0xFF, 0x00},    //  23
    { 0x00, 0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00},    //  24
    { 0x00, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00, 0x00},    //  25
    { 0x00, 0x00, 0x18, 0x30, 0x7F, 0x30, 0x18, 0x00, 0x00, 0x00},    //  26
    { 0x00, 0x00, 0x0C, 0x06, 0x7F, 0x06, 0x0C, 0x00, 0x00, 0x00},    //  27
    { 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x7F, 0x00, 0x00, 0x00},    //  28
    { 0x00, 0x00, 0x24, 0x66, 0xFF, 0x66, 0x24, 0x00, 0x00, 0x00},    //  29
    { 0x00, 0x00, 0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x00, 0x00, 0x00},    //  30
    { 0x00, 0x00, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00, 0x00, 0x00},    //  31
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},    //  32
    { 0x00, 0x0C, 0x1E, 0x1E, 0x0C, 0x0C, 0x00, 0x0C, 0x00, 0x00},    //  33
    { 0x00, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},    //  34
    { 0x00, 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00, 0x00},    //  35
    { 0x00, 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00, 0x00},    //  36
    { 0x00, 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00, 0x00},    //  37
    { 0x00, 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00, 0x00},    //  38
    { 0x00, 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},    //  39
    { 0x00, 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00, 0x00},    //  40
    { 0x00, 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00, 0x00},    //  41
    { 0x00, 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, 0x00},    //  42
    { 0x00, 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00, 0x00},    //  43
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06, 0x00},    //  44
    { 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00},    //  45
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00},    //  46
    { 0x00, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00, 0x00},    //  47
    { 0x00, 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00, 0x00},    //  48
    { 0x00, 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00, 0x00},    //  49
    { 0x00, 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00, 0x00},    //  50
    { 0x00, 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00, 0x00},    //  51
    { 0x00, 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00, 0x00},    //  52
    { 0x00, 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00, 0x00},    //  53
    { 0x00, 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00, 0x00},    //  54
    { 0x00, 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00, 0x00},    //  55
    { 0x00, 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00, 0x00},    //  56
    { 0x00, 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00, 0x00},    //  57
    { 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00},    //  58
    { 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06, 0x00},    //  59
    { 0x00, 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00, 0x00},    //  60
    { 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00},    //  61
    { 0x00, 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00, 0x00},    //  62
    { 0x00, 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00, 0x00},    //  63
    { 0x00, 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00, 0x00},    //  64
    { 0x00, 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00, 0x00},    //  65
    { 0x00, 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00, 0x00},    //  66
    { 0x00, 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00, 0x00},    //  67
    { 0x00, 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00, 0x00},    //  68
    { 0x00, 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00, 0x00},    //  69
    { 0x00, 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00, 0x00},    //  70
    { 0x00, 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00, 0x00},    //  71
    { 0x00, 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00, 0x00},    //  72
    { 0x00, 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, 0x00},    //  73
    { 0x00, 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00, 0x00},    //  74
    { 0x00, 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00, 0x00},    //  75
    { 0x00, 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00, 0x00},    //  76
    { 0x00, 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00, 0x00},    //  77
    { 0x00, 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00, 0x00},    //  78
    { 0x00, 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00, 0x00},    //  79
    { 0x00, 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00, 0x00},    //  80
    { 0x00, 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00, 0x00},    //  81
    { 0x00, 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00, 0x00},    //  82
    { 0x00, 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00, 0x00},    //  83
    { 0x00, 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, 0x00},    //  84
    { 0x00, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00, 0x00},    //  85
    { 0x00, 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00, 0x00},    //  86
    { 0x00, 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00, 0x00},    //  87
    { 0x00, 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00, 0x00},    //  88
    { 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00, 0x00},    //  89
    { 0x00, 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00, 0x00},    //  90
    { 0x00, 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00, 0x00},    //  91
    { 0x00, 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00, 0x00},    //  92
    { 0x00, 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00, 0x00},    //  93
    { 0x00, 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00},    //  94
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00},    //  95
    { 0x00, 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},    //  96
    { 0x00, 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00, 0x00},    //  97
    { 0x00, 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00, 0x00},    //  98
    { 0x00, 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00, 0x00},    //  99
    { 0x00, 0x38, 0x30, 0x30, 0x3E, 0x33, 0x33, 0x6E, 0x00, 0x00},    // 100
    { 0x00, 0x00, 0x00, 0x1E, 0x33, 0x3F, 0x03, 0x1E, 0x00, 0x00},    // 101
    { 0x00, 0x1C, 0x36, 0x06, 0x0F, 0x06, 0x06, 0x0F, 0x00, 0x00},    // 102
    { 0x00, 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F, 0x00},    // 103
    { 0x00, 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00, 0x00},    // 104
    { 0x00, 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, 0x00},    // 105
    { 0x00, 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},    // 106
    { 0x00, 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00, 0x00},    // 107
    { 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, 0x00},    // 108
    { 0x00, 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00, 0x00},    // 109
    { 0x00, 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00},    // 110
    { 0x00, 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00, 0x00},    // 111
    { 0x00, 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F, 0x00},    // 112
    { 0x00, 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78, 0x00},    // 113
    { 0x00, 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00, 0x00},    // 114
    { 0x00, 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00, 0x00},    // 115
    { 0x00, 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00, 0x00},    // 116
    { 0x00, 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00, 0x00},    // 117
    { 0x00, 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00, 0x00},    // 118
    { 0x00, 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00, 0x00},    // 119
    { 0x00, 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00, 0x00},    // 120
    { 0x00, 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F, 0x00},    // 121
    { 0x00, 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00, 0x00},    // 122
    { 0x00, 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00, 0x00},    // 123
    { 0x00, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00, 0x00},    // 124
    { 0x00, 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00, 0x00},    // 125
    { 0x00, 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},    // 126
    { 0x00, 0x00, 0x08, 0x1C, 0x36, 0x63, 0x63, 0x7F, 0x00, 0x00},    // 127
    { 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x18, 0x30, 0x1E, 0x00},    // 128
    { 0x00, 0x00, 0x33, 0x00, 0x33, 0x33, 0x33, 0x7E, 0x00, 0x00},    // 129
    { 0x00, 0x38, 0x00, 0x1E, 0x33, 0x3F, 0x03, 0x1E, 0x00, 0x00},    // 130
    { 0x00, 0x7E, 0xC3, 0x3C, 0x60, 0x7C, 0x66, 0xFC, 0x00, 0x00},    // 131
    { 0x00, 0x33, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x7E, 0x00, 0x00},    // 132
    { 0x00, 0x07, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x7E, 0x00, 0x00},    // 133
    { 0x00, 0x0C, 0x0C, 0x1E, 0x30, 0x3E, 0x33, 0x7E, 0x00, 0x00},    // 134
    { 0x00, 0x00, 0x00, 0x1E, 0x03, 0x03, 0x1E, 0x30, 0x1C, 0x00},    // 135
    { 0x00, 0x7E, 0xC3, 0x3C, 0x66, 0x7E, 0x06, 0x3C, 0x00, 0x00},    // 136
    { 0x00, 0x33, 0x00, 0x1E, 0x33, 0x3F, 0x03, 0x1E, 0x00, 0x00},    // 137
    { 0x00, 0x07, 0x00, 0x1E, 0x33, 0x3F, 0x03, 0x1E, 0x00, 0x00},    // 138
    { 0x00, 0x33, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, 0x00},    // 139
    { 0x00, 0x3E, 0x63, 0x1C, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00},    // 140
    { 0x00, 0x07, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, 0x00},    // 141
    { 0x00, 0x63, 0x1C, 0x36, 0x63, 0x7F, 0x63, 0x63, 0x00, 0x00},    // 142
    { 0x00, 0x0C, 0x0C, 0x00, 0x1E, 0x33, 0x3F, 0x33, 0x00, 0x00},    // 143
    { 0x00, 0x38, 0x00, 0x3F, 0x06, 0x1E, 0x06, 0x3F, 0x00, 0x00},    // 144
    { 0x00, 0x00, 0x00, 0xFE, 0x30, 0xFE, 0x33, 0xFE, 0x00, 0x00},    // 145
    { 0x00, 0x7C, 0x36, 0x33, 0x7F, 0x33, 0x33, 0x73, 0x00, 0x00},    // 146
    { 0x00, 0x1E, 0x33, 0x00, 0x1E, 0x33, 0x33, 0x1E, 0x00, 0x00},    // 147
    { 0x00, 0x00, 0x33, 0x00, 0x1E, 0x33, 0x33, 0x1E, 0x00, 0x00},    // 148
    { 0x00, 0x00, 0x07, 0x00, 0x1E, 0x33, 0x33, 0x1E, 0x00, 0x00},    // 149
    { 0x00, 0x1E, 0x33, 0x00, 0x33, 0x33, 0x33, 0x7E, 0x00, 0x00},    // 150
    { 0x00, 0x00, 0x07, 0x00, 0x33, 0x33, 0x33, 0x7E, 0x00, 0x00},    // 151
    { 0x00, 0x00, 0x33, 0x00, 0x33, 0x33, 0x3E, 0x30, 0x1F, 0x00},    // 152
    { 0x00, 0xC3, 0x18, 0x3C, 0x66, 0x66, 0x3C, 0x18, 0x00, 0x00},    // 153
    { 0x00, 0x33, 0x00, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x00, 0x00},    // 154
    { 0x00, 0x18, 0x18, 0x7E, 0x03, 0x03, 0x7E, 0x18, 0x18, 0x00},    // 155
    { 0x00, 0x1C, 0x36, 0x26, 0x0F, 0x06, 0x67, 0x3F, 0x00, 0x00},    // 156
    { 0x00, 0x33, 0x33, 0x1E, 0x3F, 0x0C, 0x3F, 0x0C, 0x0C, 0x00},    // 157
    { 0x00, 0x1F, 0x33, 0x33, 0x5F, 0x63, 0xF3, 0x63, 0xE3, 0x00},    // 158
    { 0x00, 0x70, 0xD8, 0x18, 0x3C, 0x18, 0x18, 0x1B, 0x0E, 0x00},    // 159
    { 0x00, 0x38, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x7E, 0x00, 0x00},    // 160
    { 0x00, 0x1C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, 0x00},    // 161
    { 0x00, 0x00, 0x38, 0x00, 0x1E, 0x33, 0x33, 0x1E, 0x00, 0x00},    // 162
    { 0x00, 0x00, 0x38, 0x00, 0x33, 0x33, 0x33, 0x7E, 0x00, 0x00},    // 163
    { 0x00, 0x00, 0x1F, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x00, 0x00},    // 164
    { 0x00, 0x3F, 0x00, 0x33, 0x37, 0x3F, 0x3B, 0x33, 0x00, 0x00},    // 165
    { 0x00, 0x3C, 0x36, 0x36, 0x7C, 0x00, 0x7E, 0x00, 0x00, 0x00},    // 166
    { 0x00, 0x1C, 0x36, 0x36, 0x1C, 0x00, 0x3E, 0x00, 0x00, 0x00},    // 167
    { 0x00, 0x0C, 0x00, 0x0C, 0x06, 0x03, 0x33, 0x1E, 0x00, 0x00},    // 168
    { 0x00, 0x00, 0x00, 0x00, 0x3F, 0x03, 0x03, 0x00, 0x00, 0x00},    // 169
    { 0x00, 0x00, 0x00, 0x00, 0x3F, 0x30, 0x30, 0x00, 0x00, 0x00},    // 170
    { 0x00, 0xC3, 0x63, 0x33, 0x7B, 0xCC, 0x66, 0x33, 0xF0, 0x00},    // 171
    { 0x00, 0xC3, 0x63, 0x33, 0xDB, 0xEC, 0xF6, 0xF3, 0xC0, 0x00},    // 172
    { 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00},    // 173
    { 0x00, 0x00, 0xCC, 0x66, 0x33, 0x66, 0xCC, 0x00, 0x00, 0x00},    // 174
    { 0x00, 0x00, 0x33, 0x66, 0xCC, 0x66, 0x33, 0x00, 0x00, 0x00},    // 175
    { 0x00, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x44, 0x11, 0x00},    // 176
    { 0x00, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0x00},    // 177
    { 0x00, 0xDB, 0xEE, 0xDB, 0x77, 0xDB, 0xEE, 0xDB, 0x77, 0x00},    // 178
    { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},    // 179
    { 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x18, 0x18, 0x18, 0x18},    // 180
    { 0x18, 0x18, 0x18, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18, 0x18},    // 181
    { 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x6F, 0x6C, 0x6C, 0x6C, 0x6C},    // 182
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x6C, 0x6C, 0x6C, 0x6C},    // 183
    { 0x00, 0x00, 0x00, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18, 0x18},    // 184
    { 0x6C, 0x6C, 0x6C, 0x6F, 0x60, 0x6F, 0x6C, 0x6C, 0x6C, 0x6C},    // 185
    { 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x6C},    // 186
    { 0x00, 0x00, 0x00, 0x7F, 0x60, 0x6F, 0x6C, 0x6C, 0x6C, 0x6C},    // 187
    { 0x6C, 0x6C, 0x6C, 0x6F, 0x60, 0x7F, 0x00, 0x00, 0x00, 0x00},    // 188
    { 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0x7F, 0x00, 0x00, 0x00, 0x00},    // 189
    { 0x18, 0x18, 0x18, 0x1F, 0x18, 0x1F, 0x00, 0x00, 0x00, 0x00},    // 190
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x18, 0x18, 0x18, 0x18},    // 191
    { 0x18, 0x18, 0x18, 0x18, 0x18, 0xF8, 0x00, 0x00, 0x00, 0x00},    // 192
    { 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0x00, 0x00, 0x00, 0x00},    // 193
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x18, 0x18, 0x18, 0x18},    // 194
    { 0x18, 0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0x18, 0x18, 0x18},    // 195
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00},    // 196
    { 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18, 0x18},    // 197
    { 0x18, 0x18, 0x18, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18, 0x18},    // 198
    { 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0xEC, 0x6C, 0x6C, 0x6C, 0x6C},    // 199
    { 0x6C, 0x6C, 0x6C, 0xEC, 0x0C, 0xFC, 0x00, 0x00, 0x00, 0x00},    // 200
    { 0x00, 0x00, 0x00, 0xFC, 0x0C, 0xEC, 0x6C, 0x6C, 0x6C, 0x6C},    // 201
    { 0x6C, 0x6C, 0x6C, 0xEF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00},    // 202
    { 0x00, 0x00, 0x00, 0xFF, 0x00, 0xEF, 0x6C, 0x6C, 0x6C, 0x6C},    // 203
    { 0x6C, 0x6C, 0x6C, 0xEC, 0x0C, 0xEC, 0x6C, 0x6C, 0x6C, 0x6C},    // 204
    { 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00},    // 205
    { 0x6C, 0x6C, 0x6C, 0xEF, 0x00, 0xEF, 0x6C, 0x6C, 0x6C, 0x6C},    // 206
    { 0x18, 0x18, 0x18, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00},    // 207
    { 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0xFF, 0x00, 0x00, 0x00, 0x00},    // 208
    { 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x18, 0x18, 0x18, 0x18},    // 209
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x6C, 0x6C, 0x6C, 0x6C},    // 210
    { 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0xFC, 0x00, 0x00, 0x00, 0x00},    // 211
    { 0x18, 0x18, 0x18, 0xF8, 0x18, 0xF8, 0x00, 0x00, 0x00, 0x00},    // 212
    { 0x00, 0x00, 0x00, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18, 0x18},    // 213
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x6C, 0x6C, 0x6C, 0x6C},    // 214
    { 0x6C, 0x6C, 0x6C, 0x6C, 0x6C, 0xFF, 0x6C, 0x6C, 0x6C, 0x6C},    // 215
    { 0x18, 0x18, 0x18, 0xFF, 0x18, 0xFF, 0x18, 0x18, 0x18, 0x18},    // 216
    { 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, 0x00, 0x00, 0x00, 0x00},    // 217
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x18, 0x18, 0x18, 0x18},    // 218
    { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00},    // 219
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00},    // 220
    { 0x00, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x00},    // 221
    { 0x00, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0x00},    // 222
    { 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00},    // 223
    { 0x00, 0x00, 0x00, 0x6E, 0x3B, 0x13, 0x3B, 0x6E, 0x00, 0x00},    // 224
    { 0x00, 0x00, 0x1E, 0x33, 0x1F, 0x33, 0x1F, 0x03, 0x03, 0x00},    // 225
    { 0x00, 0x00, 0x3F, 0x33, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00},    // 226
    { 0x00, 0x00, 0x7F, 0x36, 0x36, 0x36, 0x36, 0x36, 0x00, 0x00},    // 227
    { 0x00, 0x3F, 0x33, 0x06, 0x0C, 0x06, 0x33, 0x3F, 0x00, 0x00},    // 228
    { 0x00, 0x00, 0x00, 0x7E, 0x1B, 0x1B, 0x1B, 0x0E, 0x00, 0x00},    // 229
    { 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x03, 0x00},    // 230
    { 0x00, 0x00, 0x6E, 0x3B, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00},    // 231
    { 0x00, 0x3F, 0x0C, 0x1E, 0x33, 0x33, 0x1E, 0x0C, 0x3F, 0x00},    // 232
    { 0x00, 0x1C, 0x36, 0x63, 0x7F, 0x63, 0x36, 0x1C, 0x00, 0x00},    // 233
    { 0x00, 0x1C, 0x36, 0x63, 0x63, 0x36, 0x36, 0x77, 0x00, 0x00},    // 234
    { 0x00, 0x38, 0x0C, 0x18, 0x3E, 0x33, 0x33, 0x1E, 0x00, 0x00},    // 235
    { 0x00, 0x00, 0x00, 0x7E, 0xDB, 0xDB, 0x7E, 0x00, 0x00, 0x00},    // 236
    { 0x00, 0x60, 0x30, 0x7E, 0xDB, 0xDB, 0x7E, 0x06, 0x03, 0x00},    // 237
    { 0x00, 0x1C, 0x06, 0x03, 0x1F, 0x03, 0x06, 0x1C, 0x00, 0x00},    // 238
    { 0x00, 0x1E, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x00, 0x00},    // 239
    { 0x00, 0x00, 0x3F, 0x00, 0x3F, 0x00, 0x3F, 0x00, 0x00, 0x00},    // 240
    { 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x3F, 0x00, 0x00},    // 241
    { 0x00, 0x06, 0x0C, 0x18, 0x0C, 0x06, 0x00, 0x3F, 0x00, 0x00},    // 242
    { 0x00, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x00, 0x3F, 0x00, 0x00},    // 243
    { 0x00, 0x70, 0xD8, 0xD8, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00},    // 244
    { 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1B, 0x1B, 0x0E, 0x00},    // 245
    { 0x00, 0x0C, 0x0C, 0x00, 0x3F, 0x00, 0x0C, 0x0C, 0x00, 0x00},    // 246
    { 0x00, 0x00, 0x6E, 0x3B, 0x00, 0x6E, 0x3B, 0x00, 0x00, 0x00},    // 247
    { 0x00, 0x1C, 0x36, 0x36, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00},    // 248
    { 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00},    // 249
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00},    // 250
    { 0x00, 0xF0, 0x30, 0x30, 0x30, 0x37, 0x36, 0x3C, 0x38, 0x00},    // 251
    { 0x00, 0x1E, 0x36, 0x36, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00},    // 252
    { 0x00, 0x0E, 0x18, 0x0C, 0x06, 0x1E, 0x00, 0x00, 0x00, 0x00},    // 253
    { 0x00, 0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x00, 0x00, 0x00},    // 254
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}     // 255
    };