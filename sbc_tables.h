/*
 *
 *  Bluetooth low-complexity, subband codec (SBC) library
 *
 *  Copyright (C) 2004-2008  Marcel Holtmann <marcel@holtmann.org>
 *  Copyright (C) 2004-2005  Henryk Ploetz <henryk@ploetzli.ch>
 *  Copyright (C) 2005-2006  Brad Midgley <bmidgley@xmission.com>
 *
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/* A2DP specification: Appendix B, page 69 */
static const int sbc_offset4[4][4] = {
	{ -1, 0, 0, 0 },
	{ -2, 0, 0, 1 },
	{ -2, 0, 0, 1 },
	{ -2, 0, 0, 1 }
};

/* A2DP specification: Appendix B, page 69 */
static const int sbc_offset8[4][8] = {
	{ -2, 0, 0, 0, 0, 0, 0, 1 },
	{ -3, 0, 0, 0, 0, 0, 1, 2 },
	{ -4, 0, 0, 0, 0, 0, 1, 2 },
	{ -4, 0, 0, 0, 0, 0, 1, 2 }
};

#define SP4(val) ASR(val, SCALE_PROTO4_TBL)
#define SA4(val) ASR(val, SCALE_ANA4_TBL)
#define SP8(val) ASR(val, SCALE_PROTO8_TBL)
#define SA8(val) ASR(val, SCALE_ANA8_TBL)
#define SS4(val) ASR(val, SCALE_SPROTO4_TBL)
#define SS8(val) ASR(val, SCALE_SPROTO8_TBL)
#define SN4(val) ASR(val, SCALE_NPROTO4_TBL)
#define SN8(val) ASR(val, SCALE_NPROTO8_TBL)

static const int32_t _sbc_proto_4[20] = {
	SP4(0x02cb3e8c), SP4(0x22b63dc0), SP4(0x002329cc), SP4(0x053b7548),
	SP4(0x31eab940), SP4(0xec1f5e60), SP4(0xff3773a8), SP4(0x0061c5a7),
	SP4(0x07646680), SP4(0x3f239480), SP4(0xf89f23a8), SP4(0x007a4737),
	SP4(0x00b32807), SP4(0x083ddc80), SP4(0x4825e480), SP4(0x0191e578),
	SP4(0x00ff11ca), SP4(0x00fb7991), SP4(0x069fdc58), SP4(0x4b584000)
};
/*
	0x100000000
	0x02cb3e8c : 0.010913762263953685760498046875			--0
	0x22b63dc0 : 0.13559328019618988037109375				--1
	0x002329cc : 0.000536548905074596405029296875			--2
	0x053b7548 : 0.02043850906193256378173828125			--3
	0x31eab940 : 0.19498784840106964111328125				--4
	0xec1f5e60 : -0.077646352350711822509765625				--5
	0xff3773a8 : -0.00306012295186519622802734375			--6
	0x0061c5a7 : 0.00149188353680074214935302734375			--7
	0x07646680 : 0.0288757383823394775390625				--8
	0x3f239480 : 0.2466366589069366455078125				--9
	0xf89f23a8 : -0.02882172726094722747802734375			--10
	0x007a4737 : 0.00186581700108945369720458984375			--11
	0x00b32807 : 0.00273370905779302120208740234375			--12
	0x083ddc80 : 0.0321939289569854736328125				--13
	0x4825e480 : 0.2818281948566436767578125				--14
	0x0191e578 : 0.00613245181739330291748046875			--15
	0x00ff11ca :  0.0038920515216886997222900390625			--16
	0x00fb7991 : 0.00383720197714865207672119140625			--17
	0x069fdc58 : 0.02587678097188472747802734375			--18
	0x4b584000 : 0.294315338134765625						--19
*/


static const int32_t _anamatrix4[4] = {
	SA4(0x2d413cc0), SA4(0x3b20d780), SA4(0x40000000), SA4(0x187de2a0)
};

static const int32_t _sbc_proto_8[40] = {
	SP8(0x02e5cd20), SP8(0x22d0c200), SP8(0x006bfe27), SP8(0x07808930),
	SP8(0x3f1c8800), SP8(0xf8810d70), SP8(0x002cfdc6), SP8(0x055acf28),
	SP8(0x31f566c0), SP8(0xebfe57e0), SP8(0xff27c437), SP8(0x001485cc),
	SP8(0x041c6e58), SP8(0x2a7cfa80), SP8(0xe4c4a240), SP8(0xfe359e4c),
	SP8(0x0048b1f8), SP8(0x0686ce30), SP8(0x38eec5c0), SP8(0xf2a1b9f0),
	SP8(0xffe8904a), SP8(0x0095698a), SP8(0x0824a480), SP8(0x443b3c00),
	SP8(0xfd7badc8), SP8(0x00d3e2d9), SP8(0x00c183d2), SP8(0x084e1950),
	SP8(0x4810d800), SP8(0x017f43fe), SP8(0x01056dd8), SP8(0x00e9cb9f),
	SP8(0x07d7d090), SP8(0x4a708980), SP8(0x0488fae8), SP8(0x0113bd20),
	SP8(0x0107b1a8), SP8(0x069fb3c0), SP8(0x4b3db200), SP8(0x00763f48)
};

/*
	0x200000000
	0x02e5cd20 : 0.0056594945490360260009765625				--0
	0x22d0c200 : 0.067998945713043212890625					--1
	0x006bfe27 : 0.000823919544927775859832763671875		--2
	0x07808930: 0.01465252600610256195068359375				--3
	0x3f1c8800 : 0.1232645511627197265625					--4
	0xf8810d70 :	0.01464040763676166534423828125			--5
	0x002cfdc6 : 0.00034325639717280864715576171875			--6
	0x055acf28 : 0.010458444245159626007080078125			--7

	0x31f566c0 : 0.097575388848781585693359375				--8
	0xebfe57e0 : 0.0390751399099826812744140625				--9
	0xff27c437 : 0.001649730955250561237335205078125		--10
	0x001485cc : 0.0001565753482282161712646484375			--11
	0x041c6e58 : 0.008029411546885967254638671875			--12
	0x2a7cfa80 : 0.08298476040363311767578125				--13
	0xe4c4a240 : 0.053187303245067596435546875				--14
	0xfe359e4c : 0.0034971744753420352935791015625			--15

	0x0048b1f8 : 0.000554620288312435150146484375			--16
	0x0686ce30 : 0.01274723373353481292724609375			--17
	0x38eec5c0 : 0.111196689307689666748046875				--18
	0xf2a1b9f0 : 0.02610987611114978790283203125			--19
	0xffe8904a : 0.00017880531959235668182373046875			--20
	0x0095698a : 0.00113992509432137012481689453125			--21
	0x0824a480 : 0.01590456068515777587890625				--22
	0x443b3c00 : 0.13326442241668701171875					--23

	0xfd7badc8 : 0.004915780387818813323974609375			--24
	0x00d3e2d9 : 0.001616562833078205585479736328125		--25
	0x00c183d2 : 0.00147640169598162174224853515625			--26
	0x084e1950 : 0.01622084714472293853759765625			--27
	0x4810d800 : 0.1407535076141357421875					--28
	0x017f43fe : 0.00292408443056046962738037109375			--29
	0x01056dd8 : 0.001994545571506023406982421875			--30
	0x00e9cb9f : 0.001783717307262122631072998046875		--31
	
	0x07d7d090 : 0.01531841047108173370361328125			--32
	0x4a708980 : 0.14538984000682830810546875				--33
	0x0488fae8 : 0.008857575245201587677001953125			--34
	0x0113bd20 : 0.0021037198603153228759765625				--35
	0x0107b1a8 : 0.002011825330555438995361328125			--36
	0x069fb3c0 : 0.012937180697917938232421875				--37
	0x4b3db200 : 0.146955072879791259765625					--38
	0x00763f48 : 0.000902154482901096343994140625			--39
*/

static const int32_t sbc_proto_4_40m0[] = {
	SS4(0x00000000), SS4(0xffa6982f), SS4(0xfba93848), SS4(0x0456c7b8),
	SS4(0x005967d1), SS4(0xfffb9ac7), SS4(0xff589157), SS4(0xf9c2a8d8),
	SS4(0x027c1434), SS4(0x0019118b), SS4(0xfff3c74c), SS4(0xff137330),
	SS4(0xf81b8d70), SS4(0x00ec1b8b), SS4(0xfff0b71a), SS4(0xffe99b00),
	SS4(0xfef84470), SS4(0xf6fb4370), SS4(0xffcdc351), SS4(0xffe01dc7)
};

static const int32_t sbc_proto_4_40m1[] = {
	SS4(0xffe090ce), SS4(0xff2c0475), SS4(0xf694f800), SS4(0xff2c0475),
	SS4(0xffe090ce), SS4(0xffe01dc7), SS4(0xffcdc351), SS4(0xf6fb4370),
	SS4(0xfef84470), SS4(0xffe99b00), SS4(0xfff0b71a), SS4(0x00ec1b8b),
	SS4(0xf81b8d70), SS4(0xff137330), SS4(0xfff3c74c), SS4(0x0019118b),
	SS4(0x027c1434), SS4(0xf9c2a8d8), SS4(0xff589157), SS4(0xfffb9ac7)
};

static const int32_t sbc_proto_8_80m0[] = {
	SS8(0x00000000), SS8(0xfe8d1970), SS8(0xee979f00), SS8(0x11686100),
	SS8(0x0172e690), SS8(0xfff5bd1a), SS8(0xfdf1c8d4), SS8(0xeac182c0),
	SS8(0x0d9daee0), SS8(0x00e530da), SS8(0xffe9811d), SS8(0xfd52986c),
	SS8(0xe7054ca0), SS8(0x0a00d410), SS8(0x006c1de4), SS8(0xffdba705),
	SS8(0xfcbc98e8), SS8(0xe3889d20), SS8(0x06af2308), SS8(0x000bb7db),
	SS8(0xffca00ed), SS8(0xfc3fbb68), SS8(0xe071bc00), SS8(0x03bf7948),
	SS8(0xffc4e05c), SS8(0xffb54b3b), SS8(0xfbedadc0), SS8(0xdde26200),
	SS8(0x0142291c), SS8(0xff960e94), SS8(0xff9f3e17), SS8(0xfbd8f358),
	SS8(0xdbf79400), SS8(0xff405e01), SS8(0xff7d4914), SS8(0xff8b1a31),
	SS8(0xfc1417b8), SS8(0xdac7bb40), SS8(0xfdbb828c), SS8(0xff762170)
};

/*
	= _sbc_proto_8 / (-2)
	
	0x       0		0x 2e5cd20	0x22d0c200	0xdd2f3e00	
					0			1			-1	
	0xfd1a32e0	0x  1485cc	0x 41c6e58	0x2a7cfa80	
		-0			11			12			13
	0xe4c4a240	0xfe359e4c	0x  2cfdc6	0x 55acf28	
		14			15			6			7
	0x31f566c0	0xebfe57e0	0xff27c438	0x  48b1f6	
		8			9			10			16
	0x 686ce30	0x38eec5c0	0xf2a1b9f0	0xffe8904a	
		17			18			19			20
	0x  6bfe26	0x 7808930	0x3f1c8800	0xf8810d70	
		2			3			4			5
	0x  763f48	0x  95698a	0x 824a480	0x443b3c00	
		39			21			22			23
	0xfd7badc8	0x  d3e2d8	0x  c183d2	0x 84e1950	
		24			25			26			27
	0x4810d800	0x 17f43fe	0x 1056dd8	0x  e9cb9e	
		28			29			30			31
	0x 7d7d090	0x4a708980	0x 488fae8	0x 113bd20
		32			33			34			35


0	SP8(0x02e5cd20), SP8(0x22d0c200), SP8(0x006bfe27), SP8(0x07808930),
4	SP8(0x3f1c8800), SP8(0xf8810d70), SP8(0x002cfdc6), SP8(0x055acf28),	
8	SP8(0x31f566c0), SP8(0xebfe57e0), SP8(0xff27c437), SP8(0x001485cc),
12	SP8(0x041c6e58), SP8(0x2a7cfa80), SP8(0xe4c4a240), SP8(0xfe359e4c),	
16	SP8(0x0048b1f8), SP8(0x0686ce30), SP8(0x38eec5c0), SP8(0xf2a1b9f0),
20	SP8(0xffe8904a), SP8(0x0095698a), SP8(0x0824a480), SP8(0x443b3c00),	
24	SP8(0xfd7badc8), SP8(0x00d3e2d9), SP8(0x00c183d2), SP8(0x084e1950),
28	SP8(0x4810d800), SP8(0x017f43fe), SP8(0x01056dd8), SP8(0x00e9cb9f),	
32	SP8(0x07d7d090), SP8(0x4a708980), SP8(0x0488fae8), SP8(0x0113bd20),
36	SP8(0x0107b1a8), SP8(0x069fb3c0), SP8(0x4b3db200), SP8(0x00763f48)

*/

static const int32_t sbc_proto_8_80m1[] = {
	SS8(0xff7c272c), SS8(0xfcb02620), SS8(0xda612700), SS8(0xfcb02620),
	SS8(0xff7c272c), SS8(0xff762170), SS8(0xfdbb828c), SS8(0xdac7bb40),
	SS8(0xfc1417b8), SS8(0xff8b1a31), SS8(0xff7d4914), SS8(0xff405e01),
	SS8(0xdbf79400), SS8(0xfbd8f358), SS8(0xff9f3e17), SS8(0xff960e94),
	SS8(0x0142291c), SS8(0xdde26200), SS8(0xfbedadc0), SS8(0xffb54b3b),
	SS8(0xffc4e05c), SS8(0x03bf7948), SS8(0xe071bc00), SS8(0xfc3fbb68),
	SS8(0xffca00ed), SS8(0x000bb7db), SS8(0x06af2308), SS8(0xe3889d20),
	SS8(0xfcbc98e8), SS8(0xffdba705), SS8(0x006c1de4), SS8(0x0a00d410),
	SS8(0xe7054ca0), SS8(0xfd52986c), SS8(0xffe9811d), SS8(0x00e530da),
	SS8(0x0d9daee0), SS8(0xeac182c0), SS8(0xfdf1c8d4), SS8(0xfff5bd1a)
};
/*
	= _sbc_proto_8 / (-2)
	0x 107b1a8	0x 69fb3c0	0x4b3db200	0x 69fb3c0
		36			37			38			37
	0x 107b1a8	0x 113bd20	0x 488fae8	0x4a708980	
		36			35			34			33
	0x 7d7d090	0x  e9cb9e	0x 1056dd8	0x 17f43fe	
		32			31			30			29
	0x4810d800	0x 84e1950	0x  c183d2	0x  d3e2d8	
		28			27			26			25
	0xfd7badc8	0x443b3c00	0x 824a480	0x  95698a	
		24			23			22			21
	0x  763f48	0xf8810d70	0x3f1c8800	0x 7808930	
		39			5			4			3
	0x  6bfe26	0xffe8904a	0xf2a1b9f0	0x38eec5c0	
		2			20			19			18		
	0x 686ce30	0x  48b1f6	0xff27c438	0xebfe57e0	
		17			16			10			9
	0x31f566c0	0x 55acf28	0x  2cfdc6	0xfe359e4c	
		8			7			6			15
	0xe4c4a240	0x2a7cfa80	0x 41c6e58	0x  1485cc	
		14			13			12			11

0	SP8(0x02e5cd20), SP8(0x22d0c200), SP8(0x006bfe27), SP8(0x07808930),
4	SP8(0x3f1c8800), SP8(0xf8810d70), SP8(0x002cfdc6), SP8(0x055acf28),	
8	SP8(0x31f566c0), SP8(0xebfe57e0), SP8(0xff27c437), SP8(0x001485cc),
12	SP8(0x041c6e58), SP8(0x2a7cfa80), SP8(0xe4c4a240), SP8(0xfe359e4c),	
16	SP8(0x0048b1f8), SP8(0x0686ce30), SP8(0x38eec5c0), SP8(0xf2a1b9f0),
20	SP8(0xffe8904a), SP8(0x0095698a), SP8(0x0824a480), SP8(0x443b3c00),	
24	SP8(0xfd7badc8), SP8(0x00d3e2d9), SP8(0x00c183d2), SP8(0x084e1950),
28	SP8(0x4810d800), SP8(0x017f43fe), SP8(0x01056dd8), SP8(0x00e9cb9f),	
32	SP8(0x07d7d090), SP8(0x4a708980), SP8(0x0488fae8), SP8(0x0113bd20),
36	SP8(0x0107b1a8), SP8(0x069fb3c0), SP8(0x4b3db200), SP8(0x00763f48)
*/

static const int32_t _anamatrix8[8] = {
	SA8(0x3b20d780), SA8(0x187de2a0), SA8(0x3ec52f80), SA8(0x3536cc40),
	SA8(0x238e7680), SA8(0x0c7c5c20), SA8(0x2d413cc0), SA8(0x40000000)
};
/*
static const double x_anamatrix8[8] = {
	cos(3.1415926/8), cos(3.1415926*3/8), cos(3.1415926/16), cos(3.1415926*3/16),
	cos(3.1415926*5/16), cos(3.1415926*7/16), cos(3.1415926/4),1
};

static const double f_anamatrix8[8] = {
	0.92387950420379638671875, 0.3826834261417388916015625,
	0.98078525066375732421875,0.831469595432281494140625,
	0.55557024478912353515625, 0.1950903236865997314453125, 
	0.707106769084930419921875,1
};
*/


static const int32_t synmatrix4[8][4] = {
	{ SN4(0x05a82798), SN4(0xfa57d868), SN4(0xfa57d868), SN4(0x05a82798) },
	{ SN4(0x030fbc54), SN4(0xf89be510), SN4(0x07641af0), SN4(0xfcf043ac) },
	{ SN4(0x00000000), SN4(0x00000000), SN4(0x00000000), SN4(0x00000000) },
	{ SN4(0xfcf043ac), SN4(0x07641af0), SN4(0xf89be510), SN4(0x030fbc54) },
	{ SN4(0xfa57d868), SN4(0x05a82798), SN4(0x05a82798), SN4(0xfa57d868) },
	{ SN4(0xf89be510), SN4(0xfcf043ac), SN4(0x030fbc54), SN4(0x07641af0) },
	{ SN4(0xf8000000), SN4(0xf8000000), SN4(0xf8000000), SN4(0xf8000000) },
	{ SN4(0xf89be510), SN4(0xfcf043ac), SN4(0x030fbc54), SN4(0x07641af0) }
};

static const int32_t synmatrix8[16][8] = {
	{ SN8(0x05a82798), SN8(0xfa57d868), SN8(0xfa57d868), SN8(0x05a82798),
	  SN8(0x05a82798), SN8(0xfa57d868), SN8(0xfa57d868), SN8(0x05a82798) },
	{ SN8(0x0471ced0), SN8(0xf8275a10), SN8(0x018f8b84), SN8(0x06a6d988),
	  SN8(0xf9592678), SN8(0xfe70747c), SN8(0x07d8a5f0), SN8(0xfb8e3130) },
	{ SN8(0x030fbc54), SN8(0xf89be510), SN8(0x07641af0), SN8(0xfcf043ac),
	  SN8(0xfcf043ac), SN8(0x07641af0), SN8(0xf89be510), SN8(0x030fbc54) },
	{ SN8(0x018f8b84), SN8(0xfb8e3130), SN8(0x06a6d988), SN8(0xf8275a10),
	  SN8(0x07d8a5f0), SN8(0xf9592678), SN8(0x0471ced0), SN8(0xfe70747c) },
	{ SN8(0x00000000), SN8(0x00000000), SN8(0x00000000), SN8(0x00000000),
	  SN8(0x00000000), SN8(0x00000000), SN8(0x00000000), SN8(0x00000000) },
	{ SN8(0xfe70747c), SN8(0x0471ced0), SN8(0xf9592678), SN8(0x07d8a5f0),
	  SN8(0xf8275a10), SN8(0x06a6d988), SN8(0xfb8e3130), SN8(0x018f8b84) },
	{ SN8(0xfcf043ac), SN8(0x07641af0), SN8(0xf89be510), SN8(0x030fbc54),
	  SN8(0x030fbc54), SN8(0xf89be510), SN8(0x07641af0), SN8(0xfcf043ac) },
	{ SN8(0xfb8e3130), SN8(0x07d8a5f0), SN8(0xfe70747c), SN8(0xf9592678),
	  SN8(0x06a6d988), SN8(0x018f8b84), SN8(0xf8275a10), SN8(0x0471ced0) },
	{ SN8(0xfa57d868), SN8(0x05a82798), SN8(0x05a82798), SN8(0xfa57d868),
	  SN8(0xfa57d868), SN8(0x05a82798), SN8(0x05a82798), SN8(0xfa57d868) },
	{ SN8(0xf9592678), SN8(0x018f8b84), SN8(0x07d8a5f0), SN8(0x0471ced0),
	  SN8(0xfb8e3130), SN8(0xf8275a10), SN8(0xfe70747c), SN8(0x06a6d988) },
	{ SN8(0xf89be510), SN8(0xfcf043ac), SN8(0x030fbc54), SN8(0x07641af0),
	  SN8(0x07641af0), SN8(0x030fbc54), SN8(0xfcf043ac), SN8(0xf89be510) },
	{ SN8(0xf8275a10), SN8(0xf9592678), SN8(0xfb8e3130), SN8(0xfe70747c),
	  SN8(0x018f8b84), SN8(0x0471ced0), SN8(0x06a6d988), SN8(0x07d8a5f0) },
	{ SN8(0xf8000000), SN8(0xf8000000), SN8(0xf8000000), SN8(0xf8000000),
	  SN8(0xf8000000), SN8(0xf8000000), SN8(0xf8000000), SN8(0xf8000000) },
	{ SN8(0xf8275a10), SN8(0xf9592678), SN8(0xfb8e3130), SN8(0xfe70747c),
	  SN8(0x018f8b84), SN8(0x0471ced0), SN8(0x06a6d988), SN8(0x07d8a5f0) },
	{ SN8(0xf89be510), SN8(0xfcf043ac), SN8(0x030fbc54), SN8(0x07641af0),
	  SN8(0x07641af0), SN8(0x030fbc54), SN8(0xfcf043ac), SN8(0xf89be510) },
	{ SN8(0xf9592678), SN8(0x018f8b84), SN8(0x07d8a5f0), SN8(0x0471ced0),
	  SN8(0xfb8e3130), SN8(0xf8275a10), SN8(0xfe70747c), SN8(0x06a6d988) }
};

/*
	0x8000000

0x 5a82799	0xfa57d867	0xfa57d866	0x 5a82799	
0x 5a8279a	0xfa57d868	0xfa57d866	0x 5a82798	
0x 471cece	0xf8275a0d	0x 18f8b83	0x 6a6d98a	
0xf9592677	0xfe70747b	0x 7d8a5f4	0xfb8e3134	
0x 30fbc55	0xf89be50c	0x 7641af3	0xfcf043ad	
0xfcf043aa	0x 7641af4	0xf89be50e	0x 30fbc52	
0x 18f8b83	0xfb8e3132	0x 6a6d98a	0xf8275a0c	
0x 7d8a5f3	0xf9592678	0x 471cecc	0xfe707480	
0x       0	0x       0	0x       1	0xffffffff	
0x       2	0xfffffffe	0x       3	0xfffffffd	
0xfe70747d	0x 471cecd	0xf9592677	0x 7d8a5f3	
0xf8275a0c	0x 6a6d98b	0xfb8e312f	0x 18f8b87	
0xfcf043ac	0x 7641af3	0xf89be50c	0x 30fbc56	
0x 30fbc52	0xf89be50e	0x 7641af5	0xfcf043a7	
0xfb8e3132	0x 7d8a5f4	0xfe70747b	0xf9592678	
0x 6a6d98b	0x 18f8b80	0xf8275a0d	0x 471ced2	
0xfa57d867	0x 5a8279a	0x 5a82798	0xfa57d865	
0xfa57d869	0x 5a8279c	0x 5a82796	0xfa57d863	
0xf9592676	0x 18f8b84	0x 7d8a5f4	0x 471cecc	
0xfb8e312f	0xf8275a0d	0xfe707482	0x 6a6d98d	
0xf89be50d	0xfcf043ad	0x 30fbc56	0x 7641af4	
0x 7641af2	0x 30fbc50	0xfcf043a7	0xf89be50a	
0xf8275a0d	0xf9592677	0xfb8e3134	0xfe707480	
0x 18f8b87	0x 471ced2	0x 6a6d98d	0x 7d8a5f5	
0xf8000000	0xf8000001	0xf8000001	0xf8000001	
0xf8000001	0xf8000001	0xf8000001	0xf8000001	
0xf8275a0c	0xf9592675	0xfb8e3130	0xfe707479	
0x 18f8b7f	0x 471cec9	0x 6a6d986	0x 7d8a5f2	
0xf89be50c	0xfcf043aa	0x 30fbc52	0x 7641af2	
0x 7641af5	0x 30fbc5a	0xfcf043b2	0xf89be510	
0xf9592676	0x 18f8b82	0x 7d8a5f3	0x 471ced1	
0xfb8e3136	0xf8275a0b	0xfe707475	0x 6a6d985
*/
