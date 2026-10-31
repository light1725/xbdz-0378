#include "SC92F7251.h"
#include <intrins.h>

#define FAN_PIN P25
#define ADC_NTC 0x00
#define ADC_HUM 0x04
#define ADC_W2  0x08
#define ADC_W1  0x09

/* ============================================================
   H表压缩存储

   保留温度列：
   5℃、10℃、15℃、20℃、25℃、30℃、35℃、40℃

   删除：
   45℃

   原来：
   20行 × 9列

   现在：
   20行 × 8列

   H3压缩数据长度：
   391 bytes
   ============================================================ */

code unsigned char H3[391]=
{
    0x03,0x02,0x60,0xCC,0xE1,0xFA,0x89,0xD4,0xE0,0xA7,0x65,0xE8,0xE0,0x3A,0xF5,0x5C,
    0xD6,0x0C,0x24,0xC8,0xB8,0xD0,0xC3,0xA3,0xA4,0xC1,0x98,0x8E,0xC0,0xBB,0xE4,0xC0,
    0x5A,0x50,0xAD,0x50,0x97,0xA0,0x8C,0xCD,0x87,0x2F,0x84,0x2B,0x82,0x7F,0x81,0x8A,
    0x80,0xFA,0x80,0xA3,0x6D,0x01,0xE7,0xBE,0x80,0xE1,0x3C,0x92,0x30,0xE0,0x6B,0x27,
    0xA4,0xE0,0x26,0x98,0xDC,0xCE,0xBD,0x70,0xC5,0xF2,0xA8,0xC2,0x86,0xE0,0xC1,0x20,
    0xB6,0xC0,0x86,0xEC,0xC0,0x41,0xD2,0xA1,0x7A,0x91,0xB2,0x89,0xB4,0x85,0x81,0x83,
    0x3B,0x81,0xF4,0x81,0x37,0x80,0xC7,0x80,0x83,0x57,0x01,0x3C,0xEB,0x40,0xE0,0xCB,
    0x28,0x34,0xE0,0x46,0x4D,0x44,0xD9,0xDA,0xC0,0xCA,0x11,0x58,0xC4,0x23,0x60,0xC1,
    0xC9,0xD0,0xC0,0xCF,0xA8,0xC0,0x62,0x84,0xB0,0xCA,0x99,0x21,0x8D,0x75,0x87,0x76,
    0x84,0x49,0x82,0x89,0x81,0x8D,0x80,0xF9,0x80,0xA0,0x6A,0x48,0x00,0xD3,0x0D,0xB0,
    0xE0,0x85,0xB2,0xF4,0xE0,0x2F,0x37,0x1C,0xD1,0xB3,0x40,0xC7,0x04,0xE0,0xC2,0xEF,
    0x2C,0xC1,0x49,0xEC,0xC0,0x97,0xFE,0xC0,0x49,0x20,0xA4,0xB8,0x93,0x25,0x8A,0x61,
    0x85,0xD2,0x83,0x61,0x82,0x05,0x81,0x3F,0x80,0xCA,0x80,0x83,0x57,0x3C,0x00,0x8F,
    0xE0,0x08,0xE0,0x5A,0x1F,0x18,0xE0,0x20,0x6C,0xC8,0xCC,0x5F,0x94,0xC4,0xFC,0xF4,
    0xC2,0x1E,0x94,0xC0,0xF1,0xD6,0xC0,0x71,0x02,0xB7,0x28,0x9C,0x04,0x8E,0xCF,0x88,
    0x1D,0x84,0x9A,0x82,0xB3,0x81,0xA1,0x81,0x03,0x80,0xA6,0x6D,0x49,0x32,0x00,0x64,
    0x48,0xA8,0xE0,0x3E,0x26,0xF0,0xD6,0xBE,0xA8,0xC8,0xD2,0x34,0xC3,0x9D,0x00,0xC1,
    0x8E,0x66,0xC0,0xB4,0x50,0xC0,0x55,0x5A,0xAA,0x30,0x95,0xB6,0x8B,0x9A,0x86,0x6D,
    0x83,0xAF,0x82,0x2F,0x81,0x54,0x80,0xD6,0x80,0x8A,0x5B,0x3D,0x2B,0x00,0x47,0x64,
    0x90,0xE0,0x2B,0xCE,0x74,0xD0,0x47,0xF8,0xC6,0x68,0xA0,0xC2,0xA9,0xA4,0xC1,0x29,
    0xBC,0xC0,0x88,0x7C,0xC0,0x41,0x64,0xA0,0xBC,0x91,0x07,0x89,0x34,0x85,0x28,0x82,
    0xFC,0x81,0xC9,0x81,0x19,0x80,0xB2,0x74,0x4D,0x34,0x25,0x00,0x33,0xDB,0xC8,0xDF,
    0x86,0x50,0xCB,0xE1,0xCC,0xC4,0xBD,0xAC,0xC1,0xFE,0x8C,0xC0,0xE2,0x22,0xC0,0x68,
    0xD8,0xB2,0xDC,0x99,0xB5,0x8D,0x89,0x87,0x65,0x84,0x2E,0x82,0x72,0x81,0x7A,0x80,
    0xEB,0x80,0x96,0x62,0x42,0x2D,0x20
};

unsigned int adc_hum;
unsigned long r_hum;

signed char temperature=25;
unsigned char h_raw=0,h_filt=0,h_judge=0;
unsigned char delay_min=30,trigger_rh=65;
unsigned int w1,w2,last_w1=0,last_w2=0;
unsigned char show_time_cnt=0;

/* 动态湿度快速上升监测变量（规格书要求） */
unsigned char rise_baseline=0;
unsigned char first_rise=0;
bit rise_stage=0;
volatile unsigned int rise_sec=0;

bit baseline_ok=0;

volatile unsigned int fan_sec=0;
volatile bit sample5s=1,knob_tick=0,tx_flag=0,hs_sample_tick=0;

enum {SENSE,MOTOR} state;

void GPIO_Init(void);
void ADC_Init(void);
void Timer0_Init(void);
void UART_Init(void);
void HS313_Timer2_Init(void);

unsigned int HS313_Read_Peak(void);
unsigned int ADC_Read(unsigned char ch);
unsigned long Calc_HS_R(unsigned int adc);
signed char Read_NTC_Temp(void);
unsigned char H_Lookup(unsigned long r,signed char t);

unsigned char Map_Time(unsigned int a);
unsigned char Map_Hum(unsigned int a);

void Fan(bit on);
void Start_Fan(void);
void Reset_Rise_Monitor(unsigned char base);
void Check_Rise_Trigger(void);
void State_Machine(void);

void UART_Byte(unsigned char c);
void UART_Num(unsigned int n);
void UART_Item(unsigned char c, unsigned int n);
void Display_Send_W1W2(unsigned char w1,unsigned char w2);


/* ============================================================
   从压缩H表读取指定 row / col

   col：
   0 = 5℃
   1 = 10℃
   2 = 15℃
   3 = 20℃
   4 = 25℃
   5 = 30℃
   6 = 35℃
   7 = 40℃

   45℃已经删除
   ============================================================ */


void main(void)
{
    GPIO_Init();
    ADC_Init();
    Timer0_Init();
    UART_Init();
    HS313_Timer2_Init();

    Fan(0);
    state=SENSE;

    /* 开机立即读取电位器并计算初始设定值：W1控制延时时间，W2控制湿度阈值 */
    w1=ADC_Read(ADC_W1);
    w2=ADC_Read(ADC_W2);
    last_w1=w1;
    last_w2=w2;
    delay_min=Map_Time(w1);
    trigger_rh=Map_Hum(w2);

    EA=1;
    Display_Send_W1W2(0, trigger_rh);

    while(1)
    {
        WDTCON |= 0x10;

        /* 每100ms实时检测旋钮调节，扭动电位器时数码管立即同步刷新 */
        if(knob_tick)
        {
            unsigned int diff_w1, diff_w2;
            unsigned char new_delay, new_rh;
            bit w1_changed = 0;

            knob_tick=0;

            w1=ADC_Read(ADC_W1);
            w2=ADC_Read(ADC_W2);

            diff_w1 = (w1 > last_w1) ? (w1 - last_w1) : (last_w1 - w1);
            diff_w2 = (w2 > last_w2) ? (w2 - last_w2) : (last_w2 - w2);

            /* W2湿度旋钮：10 count迟滞防抖（约0.12%RH），消除临界点ADC底噪跳字 */
            if(diff_w2 > 10)
            {
                last_w2 = w2;
                new_rh = Map_Hum(w2);
            }
            else
            {
                new_rh = trigger_rh;
            }

            /* W1时间旋钮：平时需>36 count触发显示切换；已处于时间显示状态时>10 count即可精细微调 */
            if(diff_w1 > 36)
            {
                last_w1 = w1;
                delay_min = Map_Time(w1);
                w1_changed = 1;
            }
            else if(show_time_cnt && diff_w1 > 10)
            {
                new_delay = Map_Time(w1);
                if(new_delay != delay_min)
                {
                    last_w1 = w1;
                    delay_min = new_delay;
                    w1_changed = 1;
                }
            }

            if(w1_changed)
            {
                trigger_rh = new_rh;
                show_time_cnt = 3;
                Display_Send_W1W2(delay_min, trigger_rh);
            }
            else if(new_rh != trigger_rh)
            {
                trigger_rh = new_rh;
                if(baseline_ok)
                {
                    Display_Send_W1W2(show_time_cnt ? delay_min : h_judge, trigger_rh);
                }
            }
        }

        if(!sample5s)
            continue;

        sample5s=0;

        temperature=Read_NTC_Temp();
        adc_hum=HS313_Read_Peak();
        r_hum=Calc_HS_R(adc_hum);

        h_raw=H_Lookup(r_hum,temperature);

        if(!baseline_ok)
        {
            h_filt=h_raw;
            h_judge=h_raw;
            Reset_Rise_Monitor(h_judge);
            Display_Send_W1W2(h_judge, trigger_rh);
            continue;
        }
        else
        {
            if(h_raw>h_filt)
            {
                h_filt=(unsigned char)(((unsigned int)h_filt+h_raw+1U)>>1);
                h_judge=h_raw;
            }
            else
            {
                h_filt=(unsigned char)(((unsigned int)h_filt+h_raw)>>1);
                h_judge=h_filt;
            }
        }

        State_Machine();

        UART_Byte('H');
        UART_Byte(':');
        UART_Num(h_judge);

        UART_Item('T', (temperature<0)?(0-temperature):temperature);
        UART_Item('R', (unsigned int)(r_hum/100UL));
        UART_Item('A', adc_hum);
        UART_Item('D', delay_min);
        UART_Item('S', trigger_rh);

        UART_Byte('\r');
        UART_Byte('\n');

        /* 数码管/电脑板显示输出：
           平时：左边实时显示实测湿度 h_judge (00~99)，右边显示设定阈值 trigger_rh (30~80)；
           调节时间旋钮W1时：左边显示延时分钟 delay_min (5~60) 约3秒，方便校准。
        */
        if(show_time_cnt)
        {
            show_time_cnt--;
            Display_Send_W1W2(delay_min, trigger_rh);
        }
        else
        {
            Display_Send_W1W2(h_judge, trigger_rh);
        }
    }
}


void GPIO_Init(void)
{
    SC92F7251_NIO_Init();

    P2CON|=0x20;
    P2PH&=~0x20;
    P2&=~0x20;

    P0CON|=0x08;
    P0PH&=~0x08;
    P0&=~0x08;

    P2CON|=0x02;
    P2PH&=~0x02;
    P2&=~0x02;

    P2CON&=~0x10;
    P2PH&=~0x10;

    P2CON&=~0x01;
    P2PH&=~0x01;

    P1CON&=~0x40;
    P1PH&=~0x40;

    P1CON&=~0x80;
    P1PH&=~0x80;

    /* 未使用的外部引出脚(P0.0~P0.2, P1.0~P1.2)开启内部上拉，防止悬空引入交流电磁干扰 */
    P0CON&=~0x07;
    P0PH|=0x07;
    P1CON&=~0x07;
    P1PH|=0x07;

    ADCCFG0|=0x01;
    ADCCFG0|=0x10;
    ADCCFG1|=0x03;
}


void ADC_Init(void)
{
    OPINX=0xC2;
    OPREG&=~0x80;
    ADCCON&=~0x20;
    ADCCON|=0x80;
}


unsigned int ADC_Read(unsigned char ch)
{
    unsigned int v;
    unsigned int timeout=2000;

    ADCCON&=0xF0;
    ADCCON|=ch&0x0F;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    ADCCON|=0x40;

    while(!(ADCCON&0x10) && --timeout);

    v=((unsigned int)ADCVH<<4)|(ADCVL>>4);

    ADCCON&=~0x10;

    return v;
}


void Timer0_Init(void)
{
    TMOD&=~0x0F;
    TMOD|=0x01;
    TMCON&=~0x01;

    TH0=0x3C;
    TL0=0xB0;

    TF0=0;
    ET0=1;
    IP|=0x02;
    TR0=1;
}


void timer0_isr(void) interrupt 1
{
    static unsigned char c5=0,c1=0,ck=0;

    TH0=0x3C;
    TL0=0xB0;

    if(++ck>=2)
    {
        ck=0;
        knob_tick=1;
    }

    /* 每1秒采样刷新一次（原5秒采样太慢，会导致呼气瞬态湿度被漏掉） */
    if(++c5>=20)
    {
        c5=0;
        sample5s=1;
    }

    if(++c1>=20)
    {
        c1=0;

        if(state==MOTOR && fan_sec<65535)
            fan_sec++;

        if(rise_stage && rise_sec<65535)
            rise_sec++;
    }
}


void HS313_Timer2_Init(void)
{
    T2CON=0x00;
    TMCON&=~0x04;

    RCAP2H=0xFE;
    RCAP2L=0x0C;

    TH2=0xFE;
    TL2=0x0C;

    T2CON&=~0x80;
    ET2=1;
    T2CON|=0x04;
}


void timer2_isr(void) interrupt 5
{
    T2CON&=~0x80;

    if(P0&0x08)
    {
        P0&=~0x08;
        P2|=0x02;
    }
    else
    {
        P0|=0x08;
        P2&=~0x02;
        hs_sample_tick=1;
    }
}


unsigned int HS313_Read_Peak(void)
{
    unsigned char i, d;
    unsigned int v;
    unsigned long sum=0;
    unsigned int timeout;

    hs_sample_tick=0;

    for(i=0;i<16;i++)
    {
        /* 等待Timer2 1kHz正半周期峰值同步信号 */
        timeout=30000;
        while(!hs_sample_tick && --timeout);

        hs_sample_tick=0;

        /* 正半周期建立稳定延时（约60us~80us），避开上升沿RC过渡过程 */
        for(d=0;d<150;d++)
        {
            _nop_();
        }

        v=ADC_Read(ADC_HUM);
        sum+=v;
        WDTCON |= 0x10;
    }

    return (unsigned int)(sum/16);
}


/* ============================================================
   HS-313 电阻计算 (单位: 10欧姆, 即 0.01kΩ, 与H3压缩表完全匹配)

   0378原理图电路与低湿校准:
   P0.3(5V) -> R7(2.2k 1%) -> 传感器HS-313 -> CN3_Pin2 -> R5(33k 1%) -> P2.1(GND)
   AIN4(P2.4) 经 R6(1k) 采样 CN3_Pin2 处分压。

   低端误差修正说明:
   在45%~80%中高湿区，传感器阻抗(5.5k~160k)远小于交流容抗，纯阻性理论公式非常精准；
   但在20%~30%低湿区，传感器纯阻抗达数兆欧(1.2M~2.6M)，传感器内部极间电容(~250pF)
   与PCB寄生电容在1kHz交流正半周下会产生约50~55 count (约65mV) 的交流基底电平。
   若不扣除此硬件基底，实际25%低湿会测成30%从而提前误触发30%挡位。
   
   因此引入基底校准常数 HS_ADC_OFFSET(52)，扣除交流零点基底，全量程(20%~80%)线性完美吻合。
   ============================================================ */

#define HS_ADC_OFFSET 52U

unsigned long Calc_HS_R(unsigned int a)
{
    unsigned long div;

    if(a <= HS_ADC_OFFSET + 1U)
        return 9429000UL;

    a -= HS_ADC_OFFSET;

    if(a >= 3676U)
        return 157UL;

    div = 13516800UL / a;

    if(div <= 3677UL)
        return 157UL;

    return div - 3520UL;
}


signed char Read_NTC_Temp(void)
{
    unsigned int a;

    a=ADC_Read(ADC_NTC);

    /* NTC开路(>3900, 约<-15℃)或短路(<200, 约>100℃)异常保护：默认按标准室温25℃处理 */
    if(a>3900 || a<200)
        return 25;

    if(a>=3044) /* <= 2.5℃ */
        return 0;

    if(a>=2838) /* 2.5℃ ~ 7.5℃ -> 5℃档 */
        return 5;

    if(a>=2619) /* 7.5℃ ~ 12.5℃ -> 10℃档 */
        return 10;

    if(a>=2391) /* 12.5℃ ~ 17.5℃ -> 15℃档 */
        return 15;

    if(a>=2162) /* 17.5℃ ~ 22.5℃ -> 20℃档 */
        return 20;

    if(a>=1937) /* 22.5℃ ~ 27.5℃ -> 25℃档 */
        return 25;

    if(a>=1721) /* 27.5℃ ~ 32.5℃ -> 30℃档 */
        return 30;

    if(a>=1518) /* 32.5℃ ~ 37.5℃ -> 35℃档 */
        return 35;

    if(a>=1330) /* 37.5℃ ~ 42.5℃ -> 40℃档 */
        return 40;

    return 45;  /* > 42.5℃ */
}


unsigned char H_Lookup(unsigned long r,signed char t)
{
    code unsigned int col_offset[8]={0,53,106,156,206,254,301,347};
    unsigned char col,i,b,n;
    unsigned int p;
    unsigned long h1=0,h2,range;

    /*
       H表只有：
       5 / 10 / 15 / 20 / 25 / 30 / 35 / 40℃
       
       38℃以上统一使用40℃列。
    */

    if(t<=7)
        col=0;
    else if(t<=12)
        col=1;
    else if(t<=17)
        col=2;
    else if(t<=22)
        col=3;
    else if(t<=27)
        col=4;
    else if(t<=32)
        col=5;
    else if(t<=37)
        col=6;
    else
        col=7;

    p=col_offset[col];
    for(n=0;n<4;n++)
    {
        h1=(h1<<8)|H3[p++];
    }

    if(r>=h1)
        return 5;

    for(i=0;i<19;i++)
    {
        b=H3[p++];
        if(b<0x80)
        {
            n=0;
            range=b;
        }
        else if(b<0xC0)
        {
            n=1;
            range=b&0x3F;
        }
        else if(b<0xE0)
        {
            n=2;
            range=b&0x1F;
        }
        else
        {
            n=3;
            range=b&0x1F;
        }

        while(n--)
        {
            range=(range<<8)|H3[p++];
        }

        h2=h1-range;

        if(r>h2)
        {
            unsigned char rh;

            if(!range)
                rh = 5+i*5;
            else
                rh = (unsigned char)(5+i*5+((h1-r)*5UL)/range);

            /* 实测分段线性精度补偿（与标准温湿度仪器对标校准）：
               - <=25%RH：不抬升(+0%)，保证低湿绝不误触发30%最低挡位；
               - 26%~33%RH：平滑过渡(+1%~+4%)；
               - 中低湿段(仪器45%左右，原始值34%~47%)：补偿 +5%；
               - 中湿过渡段(原始值48%~52%)：补偿 +4%；
               - 高湿段(仪器60%以上，原始值53%~63%)：补偿 +3%；
               - 64%~66%过渡段：补偿 +2%；
               - 70%高湿段(原始值67%~75%)：补偿 +1%。
            */
            if(rh<=25)
                return rh;
            if(rh<=33)
                return (unsigned char)(rh + (rh-24)/2);
            if(rh<=47)
                return (unsigned char)(rh + 5);
            if(rh<=52)
                return (unsigned char)(rh + 4);
            if(rh<=63)
                return (unsigned char)(rh + 3);
            if(rh<=66)
                return (unsigned char)(rh + 2);
            if(rh<=75)
                return (unsigned char)(rh + 1);

            return rh;
        }

        h1=h2;
    }

    return 100;
}


unsigned char Map_Time(unsigned int a)
{
    unsigned long val;

    if(a<=50)
        return 5;
    if(a>=4040)
        return 60;

    val=((unsigned long)(a-50)*55UL+1995UL)/3990UL;
    return (unsigned char)(5+val);
}


unsigned char Map_Hum(unsigned int a)
{
    unsigned long val;

    if(a<=50)
        return 30;
    if(a>=4040)
        return 80;

    val=((unsigned long)(a-50)*50UL+1995UL)/3990UL;
    return (unsigned char)(30+val);
}


void Fan(bit on)
{
    FAN_PIN=on?1:0;
}


void Reset_Rise_Monitor(unsigned char base)
{
    ET0=0;
    rise_baseline=base;
    first_rise=0;
    rise_stage=0;
    rise_sec=0;
    baseline_ok=1;
    ET0=1;
}


void Start_Fan(void)
{
    Fan(1);
    ET0=0;
    state=MOTOR;
    fan_sec=0;
    ET0=1;
    Reset_Rise_Monitor(h_judge);
}


/* ============================================================
   检测“上涨5% + 3分钟内再次上涨5%”
   
   产品规格书要求：
   (2) 当湿度变化增加时，检测到第一个信号（湿度变化超过5%）开始计时，
       下一个三分钟内湿度变化超过5%，则电机启动运行。
   ============================================================ */

void Check_Rise_Trigger(void)
{
    unsigned int cur_rise_sec;

    /* 环境湿度若自然走低低于基准，无论处于哪个阶段均跟随下调并复位基准，
       避免先小幅上升再开窗变干后，后续洗澡上升10%无法触发的问题 */
    if(h_judge<rise_baseline)
    {
        Reset_Rise_Monitor(h_judge);
        return;
    }

    if(rise_stage)
    {
        /* 阶段1：已检测到第一次上涨5%，等待第二个5%（或相对基准累计达10%） */
        if(h_judge >= (unsigned char)(first_rise+5) ||
           h_judge >= (unsigned char)(rise_baseline+10))
        {
            if(state==MOTOR)
            {
                ET0=0;
                fan_sec=0; /* 风机运行中：重新完整计时 */
                ET0=1;
                Reset_Rise_Monitor(h_judge);
            }
            else
            {
                Start_Fan(); /* 风机未运行：启动风机 */
            }
            return;
        }

        ET0=0;
        cur_rise_sec=rise_sec;
        ET0=1;

        /* 3分钟（180秒）超时：第二次上涨未出现，超时复位基准 */
        if(cur_rise_sec>=180)
        {
            Reset_Rise_Monitor(h_judge);
            return;
        }

        return;
    }

    /* 阶段0：等待第一次上涨5% */
    /* 若单次剧烈上涨直接达到10%以上，直接触发启动 */
    if(h_judge >= (unsigned char)(rise_baseline+10))
    {
        if(state==MOTOR)
        {
            ET0=0;
            fan_sec=0;
            ET0=1;
            Reset_Rise_Monitor(h_judge);
        }
        else
        {
            Start_Fan();
        }
        return;
    }

    /* 检测到第一个信号：湿度相对基准上涨超过5% */
    if(h_judge >= (unsigned char)(rise_baseline+5))
    {
        ET0=0;
        rise_stage=1;
        first_rise=h_judge;
        rise_sec=0;
        ET0=1;
    }
}


/* ============================================================
   状态机：
   满足条件(1) 湿度达到或超过设定湿度值，电机高速动作；
   或满足条件(2) 湿度变化增加时，检测到第一个信号(上涨>5%)开始计时，
                 下一个3分钟内变化>5%，电机启动运行。
   ============================================================ */

void State_Machine(void)
{
    unsigned int target_sec, cur_fan_sec;

    if(state==MOTOR)
    {
        /* 运行期间继续检测上涨：满足上涨10%则清零时间重新完整计时 */
        Check_Rise_Trigger();

        target_sec=(unsigned int)delay_min*60U;

        ET0=0;
        cur_fan_sec=fan_sec;
        ET0=1;

        if(cur_fan_sec>=target_sec)
        {
            /* 延时时间到达后：若湿度已下降则关闭风机；若湿度依然超标则继续排风 */
            if(h_judge<trigger_rh)
            {
                ET0=0;
                fan_sec=0;
                state=SENSE;
                ET0=1;
                Fan(0);
                Reset_Rise_Monitor(h_judge);
            }
            else
            {
                ET0=0;
                fan_sec=0;
                ET0=1;
                Reset_Rise_Monitor(h_judge);
            }
        }

        return;
    }

    /* 启动条件1：实测湿度 >= 设定阈值（W2电位器设置30%~80%） */
    if(h_judge>=trigger_rh)
    {
        Start_Fan();
        return;
    }

    /* 启动条件2：检测到上涨5%开始计时，3分钟内再次上涨5%，启动电机 */
    Check_Rise_Trigger();
}


void UartInt(void) interrupt 4
{
    if(TI)
    {
        TI=0;
        tx_flag=1;
    }

    if(RI)
        RI=0;
}


void UART_Init(void)
{
    P1CON&=0xF3;
    P1PH|=0x0C;

    SCON=0x50;
    PCON|=0x80;

    T2CON&=~0x30;

    TMOD&=~0xF0;
    TMOD|=0x20;
    TMCON|=0x02;

    TL1=100;
    TH1=100;

    TR1=1;
    EUART=1;
}


void UART_Byte(unsigned char c)
{
    unsigned int to = 2000;
    tx_flag=0;
    SBUF=c;

    while(!tx_flag && --to);
    WDTCON |= 0x10;
}


void UART_Num(unsigned int n)
{
    unsigned char d[5],i=0;

    if(!n)
    {
        UART_Byte('0');
        return;
    }

    while(n)
    {
        d[i++]=n%10;
        n/=10;
    }

    while(i)
        UART_Byte('0'+d[--i]);
}


void UART_Item(unsigned char c, unsigned int n)
{
    UART_Byte(' ');
    UART_Byte(c);
    UART_Byte(':');
    UART_Num(n);
}

void Display_Send_W1W2(unsigned char w1,unsigned char w2)
{
    UART_Byte(0xAA);
    UART_Byte(0x55);
    UART_Byte(w1);
    UART_Byte(w2);
}
