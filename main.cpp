//reference
//https://docs.google.com/document/d/197dgqlNFhn72JrBeSYGtR4Y7DJQRUKC61oHVD9dvLNY/edit
#include "mbed.h"
SPI ADC(p11,p12,p13); //mosi, miso, sclk
DigitalOut cs[2] = {p14, p26};
RawSerial pc(USBTX, USBRX, 9600);
#define START_BIT   0x04
#define MODE_SINGLE 0x02    // Single-ended mode
#define MODE_DIFF   0x00    // Differential mode
uint16_t MCP3208_get(int channel, int adc_num);
uint16_t HK_data[40];
int rcmd = 0,cmdflag = 0, hk_num = 0; 
uint16_t d;
void commandget();
void initialize();
// for temp
float R3 = 110;
float R4 = 1000;
float R5 = 68000;
float Pt = 100;
float R_1 = 3;
float R_2 = 2;
float I = 0.001;
float Vreft = 2.5;
float ce = 0.003851;
float Rth, temp, volt;
float Rtha, tempa, volta;
float Rthb, tempb, voltb;
float Rthc, tempc, voltc;
float Rthd, tempd, voltd;
float Rthe, tempe, volte;
float Rthf, tempf, voltf;
float Rthg, tempg, voltg;
float Rse=0.05;
float Gain=50;

float gal;
Timer t;

int main(void)
{
    int bit = 8;
    float Vref = 5;
    ADC.format(bit,0);
    ADC.frequency(2000000); //VDD=5Vの場合は2MHz．
    pc.printf("\r\n");
    pc.printf("Hello, eps. This is MCP3208 test code.\r\n");
    pc.printf("if you wanna start, press *a*. \r\n");
    // for temp calucuration
    float gain = -R5*I/R4;
    float off = Vreft+I*R3;
    

    commandget();
    if(cmdflag == 1){
        if(rcmd == 'a'){
            t.start();
            while(1){
                for(int adc_num = 0; adc_num < 1; adc_num++){
                    for(int i = 0; i < 8; i++){
                        HK_data[hk_num] = MCP3208_get(i, adc_num);
                        //pc.printf("i = %d, ADC_num: %d, return = %d\n\r",i, adc_num, HK_data[hk_num]);
                        hk_num++;
                    }
                }

                volt = Vref*(float)HK_data[0] / 4096*(R_1 + R_2)/R_1;
                Rth = (volt-off)/gain+R3;
                temp = (Rth-Pt)/(ce*Pt);
                
                volta = Vref*(float)HK_data[1] / 4096*(R_1 + R_2)/R_1;
                Rtha = (volta-off)/gain+R3;
                tempa = (Rtha-Pt)/(ce*Pt);
                
                voltb = Vref*(float)HK_data[2] / 4096*(R_1 + R_2)/R_1;
                Rthb = (voltb-off)/gain+R3;
                tempb = (Rthb-Pt)/(ce*Pt);
                
                voltc = Vref*(float)HK_data[3] / 4096*(R_1 + R_2)/R_1;
                Rthc = (voltc-off)/gain+R3;
                tempc = (Rthc-Pt)/(ce*Pt);
                
                voltd = Vref*(float)HK_data[4] / 4096*(R_1 + R_2)/R_1;
                Rthd = (voltd-off)/gain+R3;
                tempd = (Rthd-Pt)/(ce*Pt);
                
                // volte = Vref*(float)HK_data[5] / 4096*(R_1 + R_2)/R_1;
                // Rthe = (volte-off)/gain+R3;
                // tempe = (Rthe-Pt)/(ce*Pt);
                
                // voltf = Vref*(float)HK_data[6] / 4096*(R_1 + R_2)/R_1;
                // Rthf = (voltf-off)/gain+R3;
                // tempf = (Rthf-Pt)/(ce*Pt);
                
                // voltg = Vref*(float)HK_data[7] / 4096*(R_1 + R_2)/R_1;
                // Rthg = (voltg-off)/gain+R3;
                // tempg = (Rthg-Pt)/(ce*Pt);
                // pc.printf("time[s], %.5f\r\n", t.read());
                // pc.printf("time[s], %.5f, Temp[deg], %.5f, %.5f, %.5f, %.5f, %.5f, %.5f, %.5f, %.5f\r\n",
                // t.read(),temp,tempa,tempb,tempc,tempd,tempe,tempf,tempg);

                // pc.printf("time[s], %.5f, Temp[deg], %.5f, %.5f, %.5f, %.5f, %.5f\r\n", t.read(),temp,tempa,tempb,tempc,tempd);
                pc.printf("%.5f, %.5f, %.5f, %.5f, %.5f, %.5f\r\n", t.read(),temp,tempa,tempb,tempc,tempd);
                // pc.printf("time, %.5f, CH[0]: Temp = %.5f[degree]\n\r", t.read(), temp);
                // pc.printf("time, %.5f, CH[1]: Temp = %.5f[degree]\n\r", t.read(), tempa);
                // pc.printf("time, %.5f, CH[2]: Temp = %.5f[degree]\n\r", t.read(), tempb);
                // pc.printf("time, %.5f, CH[3]: Temp = %.5f[degree]\n\r", t.read(), tempc);
                // pc.printf("time, %.5f, CH[4]: Temp = %.5f[degree]\n\r", t.read(), tempd);
                // pc.printf("time, %.5f, CH[5]: Temp = %.5f[degree]\n\r", t.read(), tempe);
                // pc.printf("time, %.5f, CH[6]: Temp = %.5f[degree]\n\r", t.read(), tempf);
                // pc.printf("time, %.5f, CH[7]: Temp = %.5f[degree]\n\r", t.read(), tempg);
                
                //Galvanometer
                //gal = Vref * (float)HK_data[2] / 4096/Gain/Rse;
                //pc.printf("ADC_num: 0, CH[2]: Galvanometer = %.5f[A]\n\r", gal);
                //pc.printf("ADC_num: 1, CH[0]: Volt = %.5f[V]\n\r", Vref * (float)HK_data[3] / 4096);
                //pc.printf("ADC_num: 1, CH[1]: Volt = %.5f[V]\n\r", Vref * (float)HK_data[4] / 4096);
                //pc.printf("ADC_num: 1, CH[2]: Volt = %.5f[V]\n\r", Vref * (float)HK_data[5] /｝｝ 4096);
                wait(1);
                hk_num = 0;
            }   
        }
    }
    // for(int j=0; j<hk_num; j++){
    //     //pc.printf("HK[%d]: %d\n\r", j, HK_data[j]);
    // } 
    initialize();
    
    
}


uint16_t MCP3208_get(int channel, int adc_num){
    cs[adc_num] = 0;
    //pc.printf("Channel: %d, ", channel);
    int command_high = START_BIT | MODE_SINGLE | ((channel & 0x04) >> 2);
    int command_low = (channel & 0x03) << 6;
    ADC.write(command_high);
    int high_byte = ADC.write(command_low) & 0x0F;
    int low_byte = ADC.write(0x00);
    int conv_result = (high_byte << 8) | low_byte;
    cs[adc_num] = 1;
    return conv_result;
}


void commandget()
{
    rcmd = pc.getc();
    cmdflag = 1;
    //pc.printf("get command: %d,%d\r\n",rcmd,cmdflag);
}
void initialize()
{
    rcmd = 0;
    cmdflag = 0;
    hk_num = 0;
}