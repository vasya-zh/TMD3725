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

#include "TMD3725.h"

int TMD3725::I2CGetreg(unsigned char addr, int reg) {
    /*
     * FUNCTION: Read the register value in byte form and return the value 
     * ---------
     * INPUT: fd - the file descriptor of the i2c device
     *        reg - the register address that we are reading from
     * RETURN: value at the register is returned
     *         -1 - error
     */
    unsigned char data = -1;
    _i2cPort.beginTransmission(addr);
	_i2cPort.write(reg);
	_i2cPort.endTransmission(false);
	_i2cPort.requestFrom(addr, 1);
	//delay(1);
	while (_i2cPort.available()) data = _i2cPort.read();
	_i2cPort.endTransmission(true);
	if (data >= 0) {
		//printf("%02X: %02X \n", reg, data);
		return data;
	}
	else {
		printf("I2C: No data on 0x%02x address, 0x%02x register\n", addr, reg);
		return -1;
	}
}

int TMD3725::I2CSetreg (unsigned char addr, int reg, int value) {
	/*
     * FUNCTION: Write value to the specific register 
     * ---------
     * INPUT: fd - the file descriptor of the i2c device
     *        reg - the register address that we are writing to
     *        value - the value that is written to the register
     * RETURN: 0 - success
     *         -1 - error
     */
    _i2cPort.beginTransmission(addr);
	_i2cPort.write(reg);
	_i2cPort.write(value);
	int error_code = _i2cPort.endTransmission();
	if (error_code != 0)
	{
		printf("I2C: Communication error: %d \n", error_code);
		return -1; // Device did not ACK
	}
	return 0;
}

bool TMD3725::connected()
{
	/*
     * FUNCTION: Check TMD3725 availability on TMD3725 address 
     * ---------
     * INPUT: fd - the file descriptor of the i2c device
     * RETURN: value at the register is returned
     *         0 - error
     */
	_i2cPort.beginTransmission(_address);
	uint8_t retval = _i2cPort.endTransmission();
	return !retval;
}

int TMD3725::set_atime(int reginfo[], int cycle_No) {
    /*
     * FUNCTION: Change the integration time
     * ---------
     * INPUT: fd - the file descriptor of the i2c device
     *        reginfo[35] - current values of all registers
     *        cycle_No - integration cycle numbers, must be between 1-256, each cycle takes 2.8ms
     * RETURN: 0 - success
     *         -1 - error
     */
    if ( (cycle_No <= 256) && (cycle_No >= 1) ) {
        reginfo[1] = cycle_No - 1;
    }
    else 
    	return -1;
    return I2CSetreg(TMD3725ADDR, ATIME_ADDR, reginfo[1]);
}


int TMD3725::get_all_data(int reginfo[]) {
    /*
     * FUNCTION: Read from each register once and stored the values in array 
     * ---------
     * INPUT: fd - the file descriptor of the i2c device
     *        reginfo[35] - the array used to store all the values from registers, it should have size of 35
     * RETURN: 0 - success
     *         -1 - error
     */
    /*Enable to PILT*/
    for (int i=0; i<9; i++) {
    	reginfo[i] = I2CGetreg(TMD3725ADDR, ENABLE_ADDR+i);
    	//printf("%d \n", reginfo[i]);
        if (reginfo[i] == -1) {
            printf("Error happens when reading some of ENABLE_ADDR to PILT register value.\n");
            return -1;
        }
    }
    /*PIHT*/
    if ((reginfo[9] = I2CGetreg(TMD3725ADDR, PIHT_ADDR)) == -1) {printf("Error happens when reading register value."); return -1;}
    /*PERS to PCFG1*/
    for (int i=0; i<4; i++) {
        if ((reginfo[10+i] = I2CGetreg(TMD3725ADDR, PERS_ADDR+i)) == -1) {
            printf("Error happens when reading PIHT register value.\n");
            return -1;
        }
    }
    /*CFG1 to PDATA*/
    for (int i=0; i<13; i++) {
        if ((reginfo[14+i] = I2CGetreg(TMD3725ADDR, CFG1_ADDR+i)) == -1) {
            printf("Error happens when reading PDATA register value.\n");
            return -1;
        }
    }
    /*CFG2*/
    if ((reginfo[27] = I2CGetreg(TMD3725ADDR, CFG2_ADDR)) == -1) {printf("Error happens when reading CFG2_ADDR register value.\n"); return -1;}
    /*CFG3*/
    if ((reginfo[28] = I2CGetreg(TMD3725ADDR, CFG3_ADDR)) == -1) {printf("Error happens when reading CFG3_ADDR register value.\n"); return -1;}
    /*POFFSETL*/
    if ((reginfo[29] = I2CGetreg(TMD3725ADDR, POFFSETL_ADDR)) == -1) {printf("Error happens when reading POFFSETL_ADDR register value.\n"); return -1;}
    /*POFFSETH*/
    if ((reginfo[30] = I2CGetreg(TMD3725ADDR, POFFSETH_ADDR)) == -1) {printf("Error happens when reading POFFSETH_ADDR register value.\n"); return -1;}
    /*CALIB*/
    if ((reginfo[31] = I2CGetreg(TMD3725ADDR, CALIB_ADDR)) == -1) {printf("Error happens when reading CALIB_ADDR register value.\n"); return -1;}
    /*CALIBCFG*/
    if ((reginfo[32] = I2CGetreg(TMD3725ADDR, CALIBCFG_ADDR)) == -1) {printf("Error happens when reading CALIBCFG_ADDR register value.\n"); return -1;}
    /*CALIBSTAT*/
    if ((reginfo[33] = I2CGetreg(TMD3725ADDR, CALIBSTAT_ADDR)) == -1) {printf("Error happens when reading CALIBSTAT_ADDR register value.\n"); return -1;}
    /*INTENAB*/
    if ((reginfo[34] = I2CGetreg(TMD3725ADDR, INTENAB_ADDR)) == -1) {printf("Error happens when reading INTENAB_ADDR register value.\n"); return -1;}
    printf("REG[35]: ");
    for (unsigned char k = 0; k < 35; k++) {
        printf("%02X ", reginfo[k]);
    }
    printf("\n\r");
    return 0;
}

int TMD3725::set_cfg1(int reginfo[], int IRtoG_flag, int again_flag) {
    /*
     * FUNCTION: Change the gain and ir to green setting
     * ---------
     * INPUT: fd - the file descriptor of the i2c device
     *        reginfo[35] - current values of all registers
     *        IRtoG_flag - 1 to enable ir to green, 0 to disable ir to green
     *        again_flag - [x1 - gain of 1, x4 - gain of 4, x16 - gain of 16, x64 - gain of 64]
     * RETURN: 0 - success
     *         -1 - error
     */
    if (IRtoG_flag) { reginfo[14] = reginfo[14] | 0x08; }
    else { reginfo[14] = reginfo[14] & 0xF7; }
    switch (again_flag) {
        case 1:
            reginfo[14] = reginfo[14] & 0xFC;
            break;
        case 4:
            reginfo[14] = reginfo[14] & 0xFD;
            reginfo[14] = reginfo[14] | 0x01;
            break;
        case 16:
            reginfo[14] = reginfo[14] & 0xFE;
            reginfo[14] = reginfo[14] | 0x02;
            break;
        case 64:
            reginfo[14] = reginfo[14] | 0x03;
            break;
        default:
            reginfo[14] = reginfo[14] & 0xFD;
            reginfo[14] = reginfo[14] | 0x01;
    }
    return I2CSetreg(TMD3725ADDR, CFG1_ADDR, reginfo[14]);
}

int TMD3725::enable_sensor(int reginfo[], int wait_flag, int prox_flag, int als_flag) {
    /*
     * FUNCTION: Change the enable register settings
     * ---------
     * INPUT: fd - the file descriptor of the i2c device
     *        reginfo[35] - current values of all registers
     *        wait_flag - 1 to enable wait feature, 0 to disable wait feature
     *        prox_flag - 1 to enable proximity feature, 0 to disable wait feature
     *        als_flag - 1 to enable als feature, 0 to disable wait feature
     * RETURN: 0 - success
     *         -1 - error
     */
    if (wait_flag) { reginfo[0] = reginfo[0] | 0x08; }
    else { reginfo[0] = reginfo[0] & 0xF7; }
    if (prox_flag) { reginfo[0] = reginfo[0] | 0x04; }
    else { reginfo[0] = reginfo[0] & 0xFB; }
    if (als_flag) { reginfo[0] = reginfo[0] | 0x03; }
    else { reginfo[0] = reginfo[0] & 0xFC; }
    return I2CSetreg(TMD3725ADDR, ENABLE_ADDR, reginfo[0]);
}

int TMD3725::init(int reginfo[]) {
    /*
     * FUNCTION: TMD3725 initialization
     * ---------
     * INPUT: fd - the file descriptor of the i2c device
     *        reginfo[35] - current values of all registers
     * RETURN: 0 - success
     *         -1 - error
     */
    if (set_atime(reginfo, 1) != -1) {
        printf("[DEVICE-0x%X]Set integration time to 1 integrated cycles.\n", reginfo[16]);
    } 
    else {
        printf("Failed to set integration time.\n", reginfo[16]);
        return -1;
    }

    if ((set_cfg1(reginfo, 0, x4)) != -1) {
        printf("[DEVICE-0x%X]Set gain to x4.\n", reginfo[16]);
    }
    else {
        printf("Failed to set gain.\n");
    }
    
    if ((enable_sensor(reginfo, 0, 1, 1)) != -1) {
        printf("[DEVICE-0x%X]Enable color integration cycle.\n", reginfo[16]);
    }
    else {
        printf("Failed to enable.\n");
        return -1;
    }
}

int TMD3725::get_optics_data(int color_array[]) {
    /*
     * FUNCTION: Read from only the color data registers and store the values in array
     * ---------
     * INPUT: fd - the file descriptor of the i2c device
     *        color_array[9] - the array used to store data from only the color data registers
     * RETURN: 0 - success
     *         -1 - error
     */
        printf("CLR   RED   GRN   BLU   PRX \n");
  	//          00 00 00 00 00 00 00 00 00 
    for (int i=0; i<9; i++) {
        if ((color_array[i] = I2CGetreg(TMD3725ADDR, CDATAL_ADDR+i)) == -1) {
            printf("Error happens when reading color value.");
            return -1;
        }
        else printf("%02X ", color_array[i]);
    }
    printf("\n\r");
    return 0;
}

hsv TMD3725::rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;
    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;
    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;
    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = 0.0;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan
    out.h *= 60.0;                              // degrees
    if( out.h < 0.0 )
        out.h += 360.0;
    return out;
}


rgb TMD3725::hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;     
}

optics_val TMD3725::get_calib_color(const int reginfo[]) {
    /*
     * FUNCTION: Read color data from sensor and caliberate them to usable data
     * ---------
     * INPUT: fd - the file descriptor of the i2c device
     *        reginfo[35] - current values of all registers
     * RETURN: calibed - return a struct that contains all the caliberated data
     *         empty struct calibed - return empty calibed if errors occur reading from register
     */
    int colorarray[9];
    optics_val calibed;
    if ((get_optics_data(colorarray)) == -1) {
        return calibed;     // if errors occur when reading from register, return empty calibed
    }
    calibed = calib_color(colorarray, reginfo);     // caliberating ...    
    return calibed;
}

optics_val TMD3725::calib_color(const int colorarray[], const int reginfo[]) {
    /*
     * FUNCTION: Caliberate color data with IR channel
     * ---------
     * INPUT: fd - the file descriptor of the i2c device
     *        colorarray[9] - values read only from color data registers
     *        reginfo[35] - current values of all registers
     * RETURN: calibed - return a struct that contains all the caliberated data
     */
    optics_val calibed;
    int rawr, rawg, rawb, rawc, Again;
    float Atime;
    rawr = combine_color(colorarray, R);
    rawg = combine_color(colorarray, G);
    rawb = combine_color(colorarray, B);
    rawc = combine_color(colorarray, C);
    Atime = 2.81 * (reginfo[1] + 1);            // calculate the integration time in ms
    if (reginfo[11] & 0x02) {
        Atime = Atime * 12;                     // account for wlong case
    }
    Again = power(2.0, (reginfo[14] & 0x03) * 2);   // calculate the gain in 1x, 4x, 16x, 64x
    if (!(reginfo[27] & 0x04)) {
        Again = Again/2;                        // account for wider range of gain
    }
    calibed.IR = ((rawr + rawg + rawb) - rawc)/2;
    calibed.CPL = (Again * Atime)/DGF;
    calibed.Lux = ((C_coef * rawc) + (R_coef * rawr) + (G_coef * rawg) + (B_coef * rawb))/calibed.CPL;  // equation for lux
    calibed.red = rawr - calibed.IR;            // caliberated red value
    calibed.green = rawg - calibed.IR;          // caliberated green value
    calibed.blue = rawb - calibed.IR;           // caliberated blue value
    calibed.clear = rawc - calibed.IR;          // caliberated clear value
    calibed.CCT = (CT_coef * (calibed.blue/calibed.red)) + CT_offset;   // equation for color temperature
    return calibed;
}

int TMD3725::combine_color(const int color_array[], int flag) {
    /*
     * FUNCTION: combine the seperate high bit and low bit color data into 2 bytes color data
     * ---------
     * INPUT: color_array[9] - the array that stored color value of registers
     *        flag - choose which color to combined. [options: C - clear, R - red, G - green, B - blue]
     * RETURN: tempcolor - the combined color value
     */
    int tempcolor = 0;
    switch(flag) {
        case 1:
            tempcolor = (color_array[1] << 8) + color_array[0];
            break;
        case 2:
            tempcolor = (color_array[3] << 8) + color_array[2];
            break;
        case 3:
            tempcolor = (color_array[5] << 8) + color_array[4];
            break;
        case 4:
            tempcolor = (color_array[7] << 8) + color_array[6];
            break;
        default:
            tempcolor = -1;
    }
    return tempcolor;
}

void TMD3725::print_color(const optics_val color_data) {
    /*
     * FUNCTION: Print the color from the processed data struct
     * ---------
     * INPUT: color_data - the struct that is storing caliberated color data
     * RETURN: void
     */
    /* formatting the data to left aligned, zero decimal, width of eight floats */
    rgb rgb_color;
    hsv hsv_color;
    printf("|CLEAR   |RED     |GREEN   |BLUE    |LUX     |CCT     |\n");
    printf("|%-8.0f|%-8.0f|%-8.0f|%-8.0f|%-8.0f|%-8.0f|\n", color_data.clear, color_data.red, color_data.green, 
                                                            color_data.blue, color_data.Lux, color_data.CCT);
    rgb_color.r = color_data.red/50;
    rgb_color.g = color_data.green/50;
    rgb_color.b = color_data.blue/50;
    hsv_color = rgb2hsv(rgb_color);
    printf("|HUE     |SAT     |VUE      |\n");
    printf("|%-8.0f|%-8.0f|%-8.0f|\n", hsv_color.h, hsv_color.s, hsv_color.v);
    //printf("RED     |GREEN   |BLUE\n");
    //printf("%-8.0f|%-8.0f|%-8.0f\n", color_data.red, color_data.green, color_data.blue);
    //rgb.r = color_data.red/255;
    return;
}

float TMD3725::power(float base, int power) {
    /*
     * FUNCTION: A simple function to calculate positive power
     * ---------
     * INPUT: base - the base number 
     *        power - a positive number represents the power
     * RETURN: result - the result of the power
     */
    float result = 1;
    for (int i=0; i<power; i++) {
        result = result * base;
    }
    return result;
}

void TMD3725::print_color_json(optics_val color_data, uint32_t timestamp) {
    /*
     * FUNCTION: Print the color in json form from the processed data struct
     * ---------
     * INPUT: color_data - the struct that is storing caliberated color data
     		  timestamp - current time in ms 
     * RETURN: void
     */
    rgb rgb_color;
    hsv hsv_color;
    rgb_color.r = color_data.red/50;
    rgb_color.g = color_data.green/50;
    rgb_color.b = color_data.blue/50;
    extern int motor_speed;
    hsv_color = rgb2hsv(rgb_color);
    printf("{\"timestamp\":\"%d\",", timestamp); 
    printf("\"hue\":\"%.0f\",", hsv_color.h); 
    printf("\"saturation\":\"%.0f\",", hsv_color.s); 
    printf("\"value\":\"%.0f\",", hsv_color.v); 
    printf("}\n\r"); 
}