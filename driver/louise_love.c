/*
    "I love Louise tan!" Device Driver
    Copyright (C) 2010 PSI

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#if MODULE
	#include <linux/kernel.h>
	#include <linux/module.h>
	#include <linux/errno.h>
	#include <linux/fs.h>
	#include <asm/uaccess.h>
	#include <linux/random.h>
#else
	#include <string.h>
	#include <stdio.h>
	#include <stdlib.h>
	//ユーザランドでも動くようにするための、関数・定数の再定義
	#define printk(...) printf(__VA_ARGS__)
	#define KERN_INFO ""
	#define KERN_WARNING ""
#endif

/* 文章をstruct Wordの配列として表現する */

struct Word {
	const short id;	//これを比較するだけで、surfaceとreadingとposのすべてが同じ事が保証される
	const short surface; //Surface[]へのインデックス
	const short reading; //Reading[]へのインデックス
	const short pos;	//Pos[]へのインデックス
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//形態素解析データ開始
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// make_c_source.rbで生成したソースは以下に張り付けてください。

#define CopipeSize (491)
static const struct Word Copipe[CopipeSize] = {
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=2,	.surface=2,	.reading=2,	.pos=1	},
	{.id=3,	.surface=3,	.reading=3,	.pos=2	},
	{.id=4,	.surface=4,	.reading=4,	.pos=3	},
	{.id=5,	.surface=5,	.reading=5,	.pos=4	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=8,	.surface=8,	.reading=8,	.pos=4	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=9,	.surface=9,	.reading=9,	.pos=4	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=10,	.surface=10,	.reading=10,	.pos=1	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=10,	.surface=10,	.reading=10,	.pos=1	},
	{.id=11,	.surface=11,	.reading=11,	.pos=6	},
	{.id=12,	.surface=11,	.reading=11,	.pos=3	},
	{.id=13,	.surface=12,	.reading=12,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=9,	.surface=9,	.reading=9,	.pos=4	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=14,	.surface=13,	.reading=13,	.pos=1	},
	{.id=3,	.surface=3,	.reading=3,	.pos=2	},
	{.id=15,	.surface=14,	.reading=14,	.pos=1	},
	{.id=5,	.surface=5,	.reading=5,	.pos=4	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=9,	.surface=9,	.reading=9,	.pos=4	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=16,	.surface=15,	.reading=15,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=17,	.surface=16,	.reading=16,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=17,	.surface=16,	.reading=16,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=18,	.surface=17,	.reading=17,	.pos=2	},
	{.id=19,	.surface=18,	.reading=18,	.pos=0	},
	{.id=20,	.surface=19,	.reading=19,	.pos=3	},
	{.id=21,	.surface=20,	.reading=20,	.pos=7	},
	{.id=10,	.surface=10,	.reading=10,	.pos=1	},
	{.id=22,	.surface=21,	.reading=21,	.pos=6	},
	{.id=23,	.surface=21,	.reading=21,	.pos=8	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=24,	.surface=22,	.reading=22,	.pos=4	},
	{.id=25,	.surface=23,	.reading=23,	.pos=4	},
	{.id=26,	.surface=24,	.reading=24,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=27,	.surface=25,	.reading=25,	.pos=1	},
	{.id=28,	.surface=26,	.reading=26,	.pos=0	},
	{.id=29,	.surface=27,	.reading=27,	.pos=0	},
	{.id=30,	.surface=28,	.reading=28,	.pos=0	},
	{.id=31,	.surface=29,	.reading=29,	.pos=7	},
	{.id=32,	.surface=30,	.reading=30,	.pos=0	},
	{.id=33,	.surface=31,	.reading=31,	.pos=0	},
	{.id=31,	.surface=29,	.reading=29,	.pos=7	},
	{.id=34,	.surface=32,	.reading=32,	.pos=0	},
	{.id=35,	.surface=33,	.reading=33,	.pos=7	},
	{.id=16,	.surface=15,	.reading=15,	.pos=0	},
	{.id=36,	.surface=34,	.reading=34,	.pos=6	},
	{.id=37,	.surface=35,	.reading=35,	.pos=3	},
	{.id=38,	.surface=36,	.reading=36,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=16,	.surface=15,	.reading=15,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=9,	.surface=9,	.reading=9,	.pos=4	},
	{.id=39,	.surface=37,	.reading=37,	.pos=4	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=40,	.surface=38,	.reading=38,	.pos=6	},
	{.id=37,	.surface=35,	.reading=35,	.pos=3	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=41,	.surface=39,	.reading=39,	.pos=0	},
	{.id=36,	.surface=34,	.reading=34,	.pos=6	},
	{.id=37,	.surface=35,	.reading=35,	.pos=3	},
	{.id=38,	.surface=36,	.reading=36,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=41,	.surface=39,	.reading=39,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=41,	.surface=39,	.reading=39,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=34,	.surface=32,	.reading=32,	.pos=0	},
	{.id=34,	.surface=32,	.reading=32,	.pos=0	},
	{.id=41,	.surface=39,	.reading=39,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=42,	.surface=40,	.reading=40,	.pos=5	},
	{.id=41,	.surface=39,	.reading=39,	.pos=0	},
	{.id=10,	.surface=10,	.reading=10,	.pos=1	},
	{.id=43,	.surface=41,	.reading=41,	.pos=6	},
	{.id=44,	.surface=42,	.reading=42,	.pos=1	},
	{.id=45,	.surface=22,	.reading=22,	.pos=7	},
	{.id=43,	.surface=41,	.reading=41,	.pos=6	},
	{.id=44,	.surface=42,	.reading=42,	.pos=1	},
	{.id=45,	.surface=22,	.reading=22,	.pos=7	},
	{.id=43,	.surface=41,	.reading=41,	.pos=6	},
	{.id=44,	.surface=42,	.reading=42,	.pos=1	},
	{.id=46,	.surface=43,	.reading=43,	.pos=6	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=47,	.surface=44,	.reading=44,	.pos=0	},
	{.id=48,	.surface=45,	.reading=45,	.pos=0	},
	{.id=49,	.surface=46,	.reading=46,	.pos=8	},
	{.id=31,	.surface=29,	.reading=29,	.pos=7	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=30,	.surface=28,	.reading=28,	.pos=0	},
	{.id=50,	.surface=47,	.reading=47,	.pos=2	},
	{.id=37,	.surface=35,	.reading=35,	.pos=3	},
	{.id=51,	.surface=48,	.reading=48,	.pos=2	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=9,	.surface=9,	.reading=9,	.pos=4	},
	{.id=15,	.surface=14,	.reading=14,	.pos=1	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=10,	.surface=10,	.reading=10,	.pos=1	},
	{.id=39,	.surface=37,	.reading=37,	.pos=4	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=10,	.surface=10,	.reading=10,	.pos=1	},
	{.id=11,	.surface=11,	.reading=11,	.pos=6	},
	{.id=9,	.surface=9,	.reading=9,	.pos=4	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=52,	.surface=49,	.reading=49,	.pos=6	},
	{.id=53,	.surface=50,	.reading=50,	.pos=1	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=54,	.surface=51,	.reading=51,	.pos=0	},
	{.id=45,	.surface=22,	.reading=22,	.pos=7	},
	{.id=26,	.surface=24,	.reading=24,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=55,	.surface=52,	.reading=52,	.pos=0	},
	{.id=56,	.surface=53,	.reading=53,	.pos=0	},
	{.id=57,	.surface=54,	.reading=41,	.pos=0	},
	{.id=58,	.surface=55,	.reading=54,	.pos=5	},
	{.id=59,	.surface=56,	.reading=55,	.pos=2	},
	{.id=37,	.surface=35,	.reading=35,	.pos=3	},
	{.id=60,	.surface=57,	.reading=56,	.pos=7	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=30,	.surface=28,	.reading=28,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=9,	.surface=9,	.reading=9,	.pos=4	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=39,	.surface=37,	.reading=37,	.pos=4	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=61,	.surface=58,	.reading=57,	.pos=2	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=30,	.surface=28,	.reading=28,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=61,	.surface=58,	.reading=57,	.pos=2	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=11,	.surface=11,	.reading=11,	.pos=6	},
	{.id=39,	.surface=37,	.reading=37,	.pos=4	},
	{.id=9,	.surface=9,	.reading=9,	.pos=4	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=62,	.surface=59,	.reading=58,	.pos=0	},
	{.id=63,	.surface=60,	.reading=59,	.pos=7	},
	{.id=64,	.surface=61,	.reading=60,	.pos=0	},
	{.id=65,	.surface=62,	.reading=61,	.pos=3	},
	{.id=66,	.surface=63,	.reading=62,	.pos=3	},
	{.id=67,	.surface=64,	.reading=63,	.pos=7	},
	{.id=68,	.surface=65,	.reading=64,	.pos=2	},
	{.id=10,	.surface=10,	.reading=10,	.pos=1	},
	{.id=46,	.surface=43,	.reading=43,	.pos=6	},
	{.id=69,	.surface=66,	.reading=65,	.pos=4	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=70,	.surface=67,	.reading=66,	.pos=7	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=8,	.surface=8,	.reading=8,	.pos=4	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=71,	.surface=68,	.reading=67,	.pos=4	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=72,	.surface=69,	.reading=68,	.pos=1	},
	{.id=39,	.surface=37,	.reading=37,	.pos=4	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=62,	.surface=59,	.reading=58,	.pos=0	},
	{.id=73,	.surface=70,	.reading=69,	.pos=7	},
	{.id=74,	.surface=71,	.reading=70,	.pos=0	},
	{.id=75,	.surface=72,	.reading=71,	.pos=7	},
	{.id=76,	.surface=73,	.reading=72,	.pos=2	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=39,	.surface=37,	.reading=37,	.pos=4	},
	{.id=10,	.surface=10,	.reading=10,	.pos=1	},
	{.id=47,	.surface=44,	.reading=44,	.pos=0	},
	{.id=63,	.surface=60,	.reading=59,	.pos=7	},
	{.id=55,	.surface=52,	.reading=52,	.pos=0	},
	{.id=63,	.surface=60,	.reading=59,	.pos=7	},
	{.id=77,	.surface=74,	.reading=73,	.pos=5	},
	{.id=78,	.surface=75,	.reading=74,	.pos=6	},
	{.id=79,	.surface=76,	.reading=75,	.pos=3	},
	{.id=10,	.surface=10,	.reading=10,	.pos=1	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=80,	.surface=77,	.reading=76,	.pos=8	},
	{.id=81,	.surface=78,	.reading=77,	.pos=7	},
	{.id=74,	.surface=71,	.reading=70,	.pos=0	},
	{.id=75,	.surface=72,	.reading=71,	.pos=7	},
	{.id=76,	.surface=73,	.reading=72,	.pos=2	},
	{.id=82,	.surface=79,	.reading=78,	.pos=1	},
	{.id=70,	.surface=67,	.reading=66,	.pos=7	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=54,	.surface=51,	.reading=51,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=83,	.surface=4,	.reading=4,	.pos=2	},
	{.id=15,	.surface=14,	.reading=14,	.pos=1	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=84,	.surface=80,	.reading=79,	.pos=6	},
	{.id=21,	.surface=20,	.reading=20,	.pos=7	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=46,	.surface=43,	.reading=43,	.pos=6	},
	{.id=69,	.surface=66,	.reading=65,	.pos=4	},
	{.id=53,	.surface=50,	.reading=50,	.pos=1	},
	{.id=39,	.surface=37,	.reading=37,	.pos=4	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=25,	.surface=23,	.reading=23,	.pos=4	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=8,	.surface=8,	.reading=8,	.pos=4	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=85,	.surface=81,	.reading=80,	.pos=0	},
	{.id=86,	.surface=82,	.reading=81,	.pos=0	},
	{.id=15,	.surface=14,	.reading=14,	.pos=1	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=87,	.surface=83,	.reading=82,	.pos=9	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=88,	.surface=84,	.reading=83,	.pos=0	},
	{.id=89,	.surface=85,	.reading=84,	.pos=6	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=90,	.surface=86,	.reading=85,	.pos=0	},
	{.id=67,	.surface=64,	.reading=63,	.pos=7	},
	{.id=91,	.surface=87,	.reading=86,	.pos=3	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=74,	.surface=71,	.reading=70,	.pos=0	},
	{.id=92,	.surface=88,	.reading=87,	.pos=7	},
	{.id=90,	.surface=86,	.reading=85,	.pos=0	},
	{.id=10,	.surface=10,	.reading=10,	.pos=1	},
	{.id=67,	.surface=64,	.reading=63,	.pos=7	},
	{.id=10,	.surface=10,	.reading=10,	.pos=1	},
	{.id=93,	.surface=89,	.reading=88,	.pos=4	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=82,	.surface=79,	.reading=78,	.pos=1	},
	{.id=94,	.surface=90,	.reading=89,	.pos=6	},
	{.id=10,	.surface=10,	.reading=10,	.pos=1	},
	{.id=95,	.surface=91,	.reading=90,	.pos=0	},
	{.id=82,	.surface=79,	.reading=78,	.pos=1	},
	{.id=96,	.surface=92,	.reading=91,	.pos=0	},
	{.id=31,	.surface=29,	.reading=29,	.pos=7	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=80,	.surface=77,	.reading=76,	.pos=8	},
	{.id=97,	.surface=93,	.reading=92,	.pos=7	},
	{.id=98,	.surface=94,	.reading=93,	.pos=0	},
	{.id=35,	.surface=33,	.reading=33,	.pos=7	},
	{.id=94,	.surface=90,	.reading=89,	.pos=6	},
	{.id=99,	.surface=91,	.reading=90,	.pos=3	},
	{.id=82,	.surface=79,	.reading=78,	.pos=1	},
	{.id=55,	.surface=52,	.reading=52,	.pos=0	},
	{.id=31,	.surface=29,	.reading=29,	.pos=7	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=80,	.surface=77,	.reading=76,	.pos=8	},
	{.id=97,	.surface=93,	.reading=92,	.pos=7	},
	{.id=98,	.surface=94,	.reading=93,	.pos=0	},
	{.id=100,	.surface=95,	.reading=94,	.pos=7	},
	{.id=101,	.surface=96,	.reading=95,	.pos=6	},
	{.id=99,	.surface=91,	.reading=90,	.pos=3	},
	{.id=102,	.surface=97,	.reading=96,	.pos=7	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=103,	.surface=98,	.reading=55,	.pos=2	},
	{.id=37,	.surface=35,	.reading=35,	.pos=3	},
	{.id=10,	.surface=10,	.reading=10,	.pos=1	},
	{.id=104,	.surface=99,	.reading=97,	.pos=0	},
	{.id=105,	.surface=100,	.reading=98,	.pos=5	},
	{.id=106,	.surface=101,	.reading=99,	.pos=6	},
	{.id=37,	.surface=35,	.reading=35,	.pos=3	},
	{.id=107,	.surface=102,	.reading=100,	.pos=0	},
	{.id=75,	.surface=72,	.reading=71,	.pos=7	},
	{.id=76,	.surface=73,	.reading=72,	.pos=2	},
	{.id=45,	.surface=22,	.reading=22,	.pos=7	},
	{.id=20,	.surface=19,	.reading=19,	.pos=3	},
	{.id=108,	.surface=103,	.reading=101,	.pos=6	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=46,	.surface=43,	.reading=43,	.pos=6	},
	{.id=109,	.surface=104,	.reading=102,	.pos=3	},
	{.id=110,	.surface=105,	.reading=103,	.pos=6	},
	{.id=111,	.surface=106,	.reading=104,	.pos=1	},
	{.id=112,	.surface=107,	.reading=105,	.pos=5	},
	{.id=113,	.surface=108,	.reading=106,	.pos=6	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=98,	.surface=94,	.reading=93,	.pos=0	},
	{.id=100,	.surface=95,	.reading=94,	.pos=7	},
	{.id=81,	.surface=78,	.reading=77,	.pos=7	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=80,	.surface=77,	.reading=76,	.pos=8	},
	{.id=97,	.surface=93,	.reading=92,	.pos=7	},
	{.id=114,	.surface=109,	.reading=107,	.pos=6	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=115,	.surface=104,	.reading=102,	.pos=6	},
	{.id=37,	.surface=35,	.reading=35,	.pos=3	},
	{.id=116,	.surface=110,	.reading=108,	.pos=7	},
	{.id=117,	.surface=111,	.reading=109,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=118,	.surface=112,	.reading=110,	.pos=0	},
	{.id=119,	.surface=113,	.reading=111,	.pos=7	},
	{.id=120,	.surface=114,	.reading=112,	.pos=6	},
	{.id=121,	.surface=115,	.reading=100,	.pos=7	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=39,	.surface=37,	.reading=37,	.pos=4	},
	{.id=122,	.surface=116,	.reading=113,	.pos=1	},
	{.id=62,	.surface=59,	.reading=58,	.pos=0	},
	{.id=31,	.surface=29,	.reading=29,	.pos=7	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=123,	.surface=117,	.reading=114,	.pos=7	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=54,	.surface=51,	.reading=51,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=46,	.surface=43,	.reading=43,	.pos=6	},
	{.id=69,	.surface=66,	.reading=65,	.pos=4	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=39,	.surface=37,	.reading=37,	.pos=4	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=11,	.surface=11,	.reading=11,	.pos=6	},
	{.id=54,	.surface=51,	.reading=51,	.pos=0	},
	{.id=39,	.surface=37,	.reading=37,	.pos=4	},
	{.id=12,	.surface=11,	.reading=11,	.pos=3	},
	{.id=8,	.surface=8,	.reading=8,	.pos=4	},
	{.id=39,	.surface=37,	.reading=37,	.pos=4	},
	{.id=124,	.surface=118,	.reading=51,	.pos=0	},
	{.id=125,	.surface=119,	.reading=115,	.pos=8	},
	{.id=15,	.surface=14,	.reading=14,	.pos=1	},
	{.id=39,	.surface=37,	.reading=37,	.pos=4	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=126,	.surface=120,	.reading=116,	.pos=0	},
	{.id=122,	.surface=116,	.reading=113,	.pos=1	},
	{.id=127,	.surface=121,	.reading=117,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=128,	.surface=122,	.reading=118,	.pos=0	},
	{.id=15,	.surface=14,	.reading=14,	.pos=1	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=129,	.surface=123,	.reading=119,	.pos=0	},
	{.id=15,	.surface=14,	.reading=14,	.pos=1	},
	{.id=39,	.surface=37,	.reading=37,	.pos=4	},
	{.id=6,	.surface=6,	.reading=6,	.pos=5	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=7,	.surface=7,	.reading=7,	.pos=1	},
	{.id=83,	.surface=4,	.reading=4,	.pos=2	},
	{.id=130,	.surface=124,	.reading=120,	.pos=6	},
	{.id=131,	.surface=125,	.reading=121,	.pos=2	},
	{.id=4,	.surface=4,	.reading=4,	.pos=3	},
	{.id=4,	.surface=4,	.reading=4,	.pos=3	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=132,	.surface=126,	.reading=122,	.pos=0	},
	{.id=31,	.surface=29,	.reading=29,	.pos=7	},
	{.id=133,	.surface=127,	.reading=123,	.pos=0	},
	{.id=134,	.surface=128,	.reading=124,	.pos=6	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=135,	.surface=129,	.reading=125,	.pos=7	},
	{.id=136,	.surface=130,	.reading=126,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
	{.id=85,	.surface=81,	.reading=80,	.pos=0	},
	{.id=86,	.surface=82,	.reading=81,	.pos=0	},
	{.id=31,	.surface=29,	.reading=29,	.pos=7	},
	{.id=0,	.surface=0,	.reading=0,	.pos=0	},
	{.id=135,	.surface=129,	.reading=125,	.pos=7	},
	{.id=136,	.surface=130,	.reading=126,	.pos=0	},
	{.id=1,	.surface=1,	.reading=1,	.pos=1	},
};
#define SurfaceSize (131)
const char* Surface[SurfaceSize] = {
	"ルイズ",
	"！",
	"ぅぅ",
	"うう",
	"う",
	"わぁ",
	"ああ",
	"\n",
	"ああん",
	"あぁ",
	"…",
	"あっ",
	"ー",
	"ぅ",
	"ぁ",
	"クンカクンカ",
	"スーハースーハー",
	"いい",
	"匂い",
	"だ",
	"なぁ",
	"くん",
	"ん",
	"はぁ",
	"っ",
	"・",
	"フラン",
	"ソワーズ",
	"たん",
	"の",
	"桃色",
	"ブロンド",
	"髪",
	"を",
	"し",
	"た",
	"いお",
	"あ",
	"間違え",
	"モフモフ",
	"カリカリ",
	"き",
	"ゅ",
	"い",
	"小説",
	"11",
	"巻",
	"かわいかっ",
	"よぅ",
	"ふ",
	"ぁぁ",
	"あん",
	"アニメ",
	"３",
	"期",
	"決まって",
	"良かっ",
	"ね",
	"かわいい",
	"コミック",
	"も",
	"発売",
	"さ",
	"れ",
	"て",
	"嬉し",
	"やぁ",
	"にゃ",
	"ぎゃ",
	"ぐ",
	"なんて",
	"現実",
	"じゃ",
	"ない",
	"よく",
	"考え",
	"たら",
	"ちゃん",
	"は",
	"？",
	"そん",
	"ハルケ",
	"ギニア",
	"この",
	"ちき",
	"しょ",
	"やめ",
	"やる",
	"なんか",
	"え",
	"見",
	"てる",
	"表紙絵",
	"が",
	"僕",
	"に",
	"話しかけ",
	"ぞ",
	"よかっ",
	"世の中",
	"まだまだ",
	"捨て",
	"モン",
	"ねっ",
	"やっ",
	"ほ",
	"ぉ",
	"おおおお",
	"おおお",
	"いる",
	"よ",
	"ケティ",
	"ひとり",
	"で",
	"できる",
	"もん",
	"、",
	"ちゃあ",
	"アン",
	"様",
	"セ",
	"セイバー",
	"シャナ",
	"ヴィルヘルミナ",
	"うっ",
	"うぅ",
	"俺",
	"想",
	"いよ",
	"へ",
	"届け",
};
#define ReadingSize (127)
const char* Reading[ReadingSize] = {
	"るいず",
	"！",
	"ぅぅ",
	"うう",
	"う",
	"わぁ",
	"ああ",
	"\n",
	"ああん",
	"あぁ",
	"…",
	"あっ",
	"ー",
	"ぅ",
	"ぁ",
	"クンカクンカ",
	"スーハースーハー",
	"いい",
	"におい",
	"だ",
	"なぁ",
	"くん",
	"ん",
	"はぁ",
	"っ",
	"・",
	"ふらん",
	"そわーず",
	"たん",
	"の",
	"ももいろ",
	"ぶろんど",
	"かみ",
	"を",
	"し",
	"た",
	"いお",
	"あ",
	"まちがえ",
	"もふもふ",
	"かりかり",
	"き",
	"ゅ",
	"い",
	"しょうせつ",
	"11",
	"かん",
	"かわいかっ",
	"よぅ",
	"ふ",
	"ぁぁ",
	"あん",
	"あにめ",
	"3",
	"きまって",
	"よかっ",
	"ね",
	"かわいい",
	"こみっく",
	"も",
	"はつばい",
	"さ",
	"れ",
	"て",
	"うれし",
	"やぁ",
	"にゃ",
	"ぎゃ",
	"ぐ",
	"なんて",
	"げんじつ",
	"じゃ",
	"ない",
	"よく",
	"かんがえ",
	"たら",
	"ちゃん",
	"は",
	"？",
	"そん",
	"はるけ",
	"ぎにあ",
	"この",
	"ちき",
	"しょ",
	"やめ",
	"やる",
	"なんか",
	"え",
	"み",
	"てる",
	"ひょうしえ",
	"が",
	"ぼく",
	"に",
	"はなしかけ",
	"ぞ",
	"よのなか",
	"まだまだ",
	"すて",
	"もん",
	"ねっ",
	"やっ",
	"ほ",
	"ぉ",
	"おおおお",
	"おおお",
	"いる",
	"よ",
	"ケティ",
	"ひとり",
	"で",
	"できる",
	"、",
	"ちゃあ",
	"さま",
	"せ",
	"せいばー",
	"しゃな",
	"ヴィルヘルミナ",
	"うっ",
	"うぅ",
	"おれ",
	"そう",
	"いよ",
	"へ",
	"とどけ",
};
#define PosSize (10)
const char* Pos[PosSize] = {
	"名詞",
	"特殊",
	"形容詞",
	"助動詞",
	"感動詞",
	"副詞",
	"動詞",
	"助詞",
	"接尾辞",
	"連体詞",
};

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//形態素解析データ終了
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//-----------------------------------------------------------------------------------

/***********************************************************************************
  * 辞書生成部
  ***********************************************************************************/


//ワードリストの動的な長さの配列を作るときに使用する一方向リスト。
struct WordList{
	struct WordList* next;
	const struct Word* word;
};

	int Marcov2Used = 0;
	struct WordList MarcovList2[CopipeSize];
	int Marcov3Used = 0;
	struct WordList MarcovList3[CopipeSize-1];

//三階のマルコフ連鎖を行うときに参照する辞書。
//二階のマルコフ連鎖辞書の中に配置される。
struct SecondStageDic {
	int list_size;
	struct WordList* list;	//from MarcovList3
};

//二階のマルコフ連鎖時に使う辞書。
//三階のマルコフ連鎖が可能だったらそっちを使う。
struct FirstStageDic{
	struct SecondStageDic next[ReadingSize];
	//二階のマルコフ連鎖
	int list_size;
	struct WordList* list;	//from MarcovList2
};

	struct FirstStageDic Dic[ReadingSize];

struct WordList* getWordList(struct WordList* start,int cnt){
	int i;
	for(i=0;i<cnt;i++){
		start = start->next;
	}
	return start;
}

void add_second_dic(struct SecondStageDic* dic,const struct Word* word){
	struct WordList* insert = &MarcovList3[Marcov3Used];
	insert->word = word;
	Marcov3Used++;
	if(Marcov3Used > (CopipeSize-1)){
		printk( KERN_WARNING "louise_love : MarkovList3 memory leaked.\n" );
	}
	if(dic->list_size == 0){
		dic->list = insert;
		dic->list_size=1;
	}else{
		struct WordList* last = getWordList(dic->list,dic->list_size-1);
		last->next = insert;
		dic->list_size++;
	}
}

void add_first_dic(struct FirstStageDic* dic,const struct Word* word){
	struct WordList* insert = &MarcovList2[Marcov2Used];
	insert->word = word;
	Marcov2Used++;
	if(Marcov2Used > CopipeSize){
		printk( KERN_WARNING "louise_love : MarkovList3 memory leaked.\n" );
	}
	if(dic->list_size == 0){
		dic->list = insert;
		dic->list_size=1;
	}else{
		struct WordList* last = getWordList(dic->list,dic->list_size-1);
		last->next = insert;
		dic->list_size++;
	}
}

//辞書生成時はこの関数を呼ぶ。
void buildDictionary(void){
	short i;
	short beforebefore = -1;
	short before = -1;
	memset(Dic,0,sizeof(Dic));
	memset(MarcovList2,0,sizeof(MarcovList2));
	memset(MarcovList3,0,sizeof(MarcovList3));
	for(i=0;i<CopipeSize;i++){
		const struct Word* copipe = &Copipe[i];
		if(before >= 0){
			//辞書に追加
			struct FirstStageDic* dic = &Dic[before];
			add_first_dic(dic,copipe);
			if(beforebefore >= 0){
				struct SecondStageDic* second_dic = &(Dic[beforebefore].next[before]);
				add_second_dic(second_dic,copipe);
			}
		}
		//更新
		beforebefore = before;
		before = copipe->reading;
	}
}

/***********************************************************************************
  * 文章生成部
  ***********************************************************************************/

struct CopipeStatus{
	//readingのindex
	short beforebefore;
	short before;
	short now;
	//surfaceのindex
	short now_str;
	short now_str_size;
	short now_str_ptr;
	//乱数
	unsigned int seed;
};

unsigned int nextRand(struct CopipeStatus* status){
	unsigned int result;
	status->seed *= 214013;
	status->seed += 2531011; // ->次に呼び出されたときのseedに使う
	result = status->seed;
	result = result >> 0x10;
	result &= 0x7fff;
	return result;
}

void initCopipeStatus(struct CopipeStatus* status){
	status->beforebefore=-1;
	status->before=-1;
	status->now=0;	//「るいず」から開始
	status->now_str = 0; //「ルイズ」
	status->now_str_size = strlen(Surface[status->now_str]);
	status->now_str_ptr = 0;
#if MODULE
	status->seed = random32();
#else
	status->seed = 0;
#endif
}

void nextCopipeStatus(struct CopipeStatus* status){
	const struct Word* word;
	status->beforebefore = status->before;
	status->before = status->now;
	if(status->beforebefore >= 0){
		struct SecondStageDic* dic2 = &Dic[status->beforebefore].next[status->before];
		if(dic2->list_size > 0){
			int idx = nextRand(status) % dic2->list_size;
			word = (getWordList(dic2->list,idx))->word;
		}else{
			struct FirstStageDic* dic = &Dic[status->before];
			int idx = nextRand(status) % dic->list_size;
			word = (getWordList(dic->list,idx))->word;
		}
	}else{
		struct FirstStageDic* dic = &Dic[status->before];
		int idx = nextRand(status) % dic->list_size;
		word = (getWordList(dic->list,idx))->word;
	}
	
	status->now = word->reading;
	status->now_str = word->surface;
	status->now_str_size = strlen(Surface[status->now_str]);
	status->now_str_ptr = 0;
}

#ifdef MODULE
/***********************************************************************************
  * デバイスドライバ部
  ***********************************************************************************/
MODULE_AUTHOR("PSI");
MODULE_DESCRIPTION("\"I love Louise tan!\" MODULE");
MODULE_LICENSE("GPL");

/*
  * ファイル操作系関数
  */

//fopen
static int louise_open( struct inode* inode, struct file* filp ){
	struct CopipeStatus* status = kmalloc(sizeof(struct CopipeStatus),GFP_ATOMIC);
	printk( KERN_INFO "louise_love : uwaaaaaaaaaaaaaaaaaaaaaaaa\n" );
	if(status == NULL){
		printk( KERN_INFO "louise_love : can't alloc mem.\n" );
		return -EBUSY;
	}
	initCopipeStatus(status);
	filp->private_data = status;
	return 0;
}

//fclose
static int louise_release( struct inode* inode, struct file* filp ){
	printk( KERN_INFO "louise_love : boku ni ha louise chan ga iru !!\n");
	kfree(filp->private_data);
	return 0;
}

//fread
static ssize_t louise_read( struct file* filp, char* buf, size_t count, loff_t* pos ){
	struct CopipeStatus* status = (struct CopipeStatus*)filp->private_data;
	size_t left = count;
	//バッファめいっぱいまで書き込む。文字の途中で切れるのはどうしようかと思ったけど、まあいいや。
	while(left > 0){
		size_t left_str = status->now_str_size - status->now_str_ptr;
		size_t send;
		if(left > left_str){
			send = left_str;
		}else{
			send = left;
		}
		if ( copy_to_user( &buf[count-left], &Surface[status->now_str][status->now_str_ptr], send ) ) {
			printk( KERN_INFO "louise_love : copy_to_user failed\n" );
			return -EFAULT;
		}

		status->now_str_ptr += send;
		left -= send;
		if(status->now_str_size == status->now_str_ptr){
			nextCopipeStatus(status);
		}
	}
	*pos += count;
	return count;
}

//fwrite
static ssize_t louise_write(struct file* filp, const char* buf, size_t count, loff_t* pos ){
	return count; //何もしない（ｗ
}

/* この構造体に、デバイスへの操作関数を登録し、初期化時にカーネルへ渡す */

static struct file_operations louise_fops = {
  owner   : THIS_MODULE,
  read    : louise_read,
  write   : louise_write,
  open    : louise_open,
  release : louise_release,
};

/*
  * 終了・開始
  */

int  init_module( void ){
	if ( register_chrdev( 0x0721, "louise_love", &louise_fops ) ) {
		printk( KERN_INFO "louise_love : louise chan ha genjitsu ja nai!?\n" );
		return -EBUSY;
	}
	buildDictionary();
	printk( KERN_INFO "louise_love : kunka kunka.\n" );
	return 0;
}

void cleanup_module( void ){
	unregister_chrdev( 0x0721, "louise_love" );
	printk( KERN_INFO "louise_love : Harukeginia no louise he todoke !!\n" );
}


#else
/*
  * ユーザランドアプリとして動かすためのmain関数
  */
int main(int argc,char* argv[]){
	buildDictionary();
	int i;
	struct CopipeStatus status;
	initCopipeStatus(&status);
	for(;;){
		printk("%s",Surface[status.now_str]);
		nextCopipeStatus(&status);
	}
}
#endif


