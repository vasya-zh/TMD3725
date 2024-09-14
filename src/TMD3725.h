/* MIT License
 * Copyright (c) 2024 Vasilii Zhuravskii zhuravsky.cc
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * 
 */

#ifndef __TMD3725_H
#define __TMD3725_H

#include <Wire.h>

#define TMD3725ADDR 	0x39	// TMD3725 default I2C address

// TMD3725 register addresses
#define ENABLE_ADDR     0x80	// R/W
#define ATIME_ADDR      0x81	// R/W
#define PTIME_ADDR      0x82	// R/W
#define WTIME_ADDR      0x83	// R/W
#define AILTL_ADDR      0x84	// R/W
#define AILTH_ADDR      0x85	// R/W
#define AIHTL_ADDR      0x86	// R/W
#define AIHTH_ADDR      0x87	// R/W
#define PILT_ADDR       0x88	// R/W
#define PIHT_ADDR       0x8A	// R/W
#define PERS_ADDR       0x8C	// R/W
#define CFG0_ADDR       0x8D	// R/W
#define PCFG0_ADDR      0x8E	// R/W
#define PCFG1_ADDR      0x8F	// R/W
#define CFG1_ADDR       0x90	// R/W
#define REVID_ADDR      0x91	// R
#define ID_ADDR         0x92	// R
#define STATUS_ADDR     0x93	// R/SC
#define CDATAL_ADDR     0x94	// R
#define CDATAH_ADDR     0x95	// R
#define RDATAL_ADDR     0x96	// R
#define RDATAH_ADDR     0x97	// R
#define GDATAL_ADDR     0x98	// R
#define GDATAH_ADDR     0x99	// R
#define BDATAL_ADDR     0x9A	// R
#define BDATAH_ADDR     0x9B	// R
#define PDATA_ADDR      0x9C	// R
#define CFG2_ADDR       0x9F	// R/W
#define CFG3_ADDR       0xAB	// R/W
#define POFFSETL_ADDR   0xC0	// R/W
#define POFFSETH_ADDR   0xC1	// R/W
#define CALIB_ADDR      0xD7	// R/W
#define CALIBCFG_ADDR   0xD9	// R/W
#define CALIBSTAT_ADDR  0xDC	// R/W
#define INTENAB_ADDR    0xDD	// R/W

// Gain setting
#define x1 1
#define x4 4
#define x16 16
#define x64 64

#define C_coef      0.16    // clear channel coefficient
#define R_coef      -0.04   
#define G_coef      0.16
#define B_coef      -0.29
#define DGF         682.85
#define CT_coef     4520    // color temperature coefficient
#define CT_offset   1804    // color temperature offset

/*combine color flag*/
#define C           1
#define R           2
#define G           3
#define B           4

typedef struct optics_val {
    // struct that stored the processed data that can be used in later analysis
    float red;
    float green;
    float blue;
    float clear;
    float IR;
    float CPL;
    float Lux;
    float CCT;
} optics_val;

typedef struct {
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
} hsv;

//extern hsv hsv_color;
//extern rgb rgb_color;

class TMD3725
{
private:
	uint8_t _address;
	TwoWire& _i2cPort;
	uint8_t _controlRegister;
	int I2CGetreg(uint8_t addr, int reg);
	int I2CSetreg (uint8_t addr, int reg, int value);

public:
	TMD3725(TwoWire& i2cPort = Wire) : _i2cPort(i2cPort)
	{
	}

	bool begin(uint8_t address = TMD3725ADDR)
	{
		_address = address;
		return connected();
	}

	bool connected(); // check if TMD3725 present on 0x39 I2C address
	int set_atime(int reginfo[], int cycle_No); // Set integration time
	int set_cfg1(int reginfo[], int IRtoG_flag, int again_flag); // set the gain and IR to GREEN settings
	int enable_sensor(int reginfo[], int wait_flag, int prox_flag, int als_flag); // Enable wait, prox, als features

	int init(int reginfo[]); // Initialize the sensor

	int get_all_data(int reginfo[]); // get all 35 sensor registers data
	int get_optics_data(int color_array[]); // get only 9 color registers data
	int combine_color(const int color_array[], int flag); // convert color array pairs to 2 byte color data
	optics_val calib_color(const int colorarray[], const int reginfo[]); // caliberate color data with IR channel
	optics_val get_calib_color(const int reginfo[]); // get data from the sensor and calibrate it

	float power(float base, int power); // power math function
	hsv rgb2hsv(rgb in); // convert rgb color structure to hsv color format
	rgb hsv2rgb(hsv in); // convert hsv color structure to rgb color format

	void print_color(optics_val color_data);    // get color data from the sensor and print it to stdout in rgb and hsv formats
	void print_color_json(optics_val color_data, uint32_t timestamp); // get color data from the sensor and print it to stdout in json format
};

#endif // __TMD3725_H
