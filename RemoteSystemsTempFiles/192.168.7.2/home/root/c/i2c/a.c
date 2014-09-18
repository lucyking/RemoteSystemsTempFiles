//#include <linux/module.h>
#include <stdio.h>
#include <stdlib.h>   //exit()
#include <fcntl.h>    //define O_RDWR
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#define SMPLRT_DIV  0x19    //采样率分频，典型值：0x07(125Hz) */
#define CONFIG   0x1A       // 低通滤波频率，典型值：0x06(5Hz) */
#define GYRO_CONFIG  0x1B   // 陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s) */
#define ACCEL_CONFIG 0x1C  // 加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz) */



#define ACCEL_XOUT_H 0x3B  // 存储最近的X轴、Y轴、Z轴加速度感应器的测量值 */
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

#define TEMP_OUT_H   0x41   // 存储的最近温度传感器的测量值 */
#define TEMP_OUT_L   0x42

#define GYRO_XOUT_H  0x43 // 存储最近的X轴、Y轴、Z轴陀螺仪感应器的测量值 */
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48

#define PWR_MGMT_1   0x6B // 电源管理，典型值：0x00(正常启用) */
#define WHO_AM_I     0x75 //IIC地址寄存器(默认数值0x68，只读) */
#define LSB_Sensitivity_gyro 16
#define LSB_Sensitivity_accle 4096
      
    void main()  
    {  
            int file,i,k;


            int addr = 0x68; //i2c device address
            char *filename = "/dev/i2c-1";  
            char wbuf[] = {0x6b, 0b00000000}; //first byte is address to write. others are bytes to be written
            char read_start_buf[1];   
            char rbuf[1000] = {0};
            unsigned short untem[1]={0},ungy[1]={0};
            signed short tem,gy;
            float temperature;


      
            if((file = open(filename, O_RDWR)) < 0)  
            {  
                    perror("Failed to open i2c device.\n");  
                    exit(1);  
            }  
      
            if(ioctl(file, I2C_SLAVE, addr) < 0)  
            {  
                    printf("Failed to access bus.\n");  
                    exit(1);  
            }  

         /*******************init_set*********************************************************/

            wbuf[0] = 0x6b;      // reset
            wbuf[1] = 0b00000000;
            write(file, wbuf, 2);


            wbuf[0] = 0x1a;      // 低通滤波频率，典型值：0x06(5Hz) */
            wbuf[1] = 0x06;
            write(file, wbuf, 2);

            wbuf[0] = 0x19;      //采样率分频，典型值：0x07(125Hz) */
            wbuf[1] = 0x07;
            write(file, wbuf, 2);
/*
            wbuf[0] = 0x1b;      // 陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
            wbuf[1] = 0x18;
            write(file, wbuf, 2);
*/


            wbuf[0] = 0x1b;              // 自检和陀螺仪范围write to register 0x1b set the gyro's Full Scale Range.
            wbuf[1] = 0b00011000;
            write(file, wbuf, 2);

            wbuf[0] = 0x1c;       // 加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz) */
            wbuf[1] = 0b00010000;
            write(file, wbuf, 2);


            wbuf[0] = 0x6b;              //write 0x00 to start.
            wbuf[1] = 0x00;
            write(file, wbuf, 2);


            printf("Temperature:\tGyroscope:\t\t\t\tAccelermetter:\n");



            /****************************gyro_begain************************************************/

            for(i=0;i<50;i++)
            {

        	    //Temperature
                read_start_buf[0] = 0x41;
                write(file, read_start_buf, 1); //reposition file pointer to register 0x29
                read(file, untem, 2); //read register 0x29 and save to rbuf


                untem[0]=untem[0]<<8 | untem[0]>>8;
                tem=(signed short)untem[0];



                temperature=tem/340+36.53;
                printf("T:%.0f℃\t\t",temperature);




/***************************Gyro:*************************************************************/

            	    //gyro_x
                    read_start_buf[0] = 0x43;   
                    write(file, read_start_buf, 1); //reposition file pointer to register 0x29  
                    read(file, ungy, 2); //read register 0x29 and save to rbuf
                    ungy[0]=ungy[0]<<8|ungy[0]>>8;
                    gy=(signed short) ungy[0];
                    printf("gx:%5d  ", gy/LSB_Sensitivity_gyro+2);


            	    //gyro_y
                    read_start_buf[0] = 0x45;
                    write(file, read_start_buf, 1); //reposition file pointer to register 0x29
                    read(file, ungy, 2); //read register 0x29 and save to rbuf
                    ungy[0]=ungy[0]<<8|ungy[0]>>8;
                    gy=(signed short) ungy[0];
                    printf("gy:%5d  ", gy/LSB_Sensitivity_gyro);

            	    //gyro_z
                    read_start_buf[0] = 0x47;
                    write(file, read_start_buf, 1); //reposition file pointer to register 0x29
                    read(file, ungy, 2); //read register 0x29 and save to rbuf
                    ungy[0]=ungy[0]<<8|ungy[0]>>8;
                    gy=(signed short) ungy[0];
                    printf("gz:%5d", gy/LSB_Sensitivity_gyro);


                    printf("\t\t");


 /***************************Accelermetter:*************************************************************/
            	    //gyro_x
                    read_start_buf[0] = 0x3b;
                    write(file, read_start_buf, 1); //reposition file pointer to register 0x29
                    read(file, ungy, 2); //read register 0x29 and save to rbuf
                    ungy[0]=ungy[0]<<8|ungy[0]>>8;
                    gy=(signed short) ungy[0];
                    printf("ax:%5d  ", gy/LSB_Sensitivity_accle);


            	    //gyro_y
                    read_start_buf[0] = 0x3d;
                    write(file, read_start_buf, 1); //reposition file pointer to register 0x29
                    read(file, ungy, 2); //read register 0x29 and save to rbuf
                    ungy[0]=ungy[0]<<8|ungy[0]>>8;
                    gy=(signed short) ungy[0];
                    printf("ay:%5d  ", gy/LSB_Sensitivity_accle);

            	    //gyro_z
                    read_start_buf[0] = 0x3f;
                    write(file, read_start_buf, 1); //reposition file pointer to register 0x29
                    read(file, ungy, 2); //read register 0x29 and save to rbuf
                    ungy[0]=ungy[0]<<8|ungy[0]>>8;
                    gy=(signed short) ungy[0];
                    printf("az:%5d\n", gy/LSB_Sensitivity_accle);





      


                    /*
                    read_start_buf[0] = 0x43;               //int
                    write(file, read_start_buf, 1);
                    read(file, ibuf, 2);
                    printf("%d\t",ibuf[0]);
                    */






                    /*
                    sp=p;
                    printf("x:%d\t", (p[0]-1)&32767 );
                    printf("x:%d\t", *sp );
                    */
/**********************************************************************************/





/**********************************************************************************/





/**********************************************************************************/


                    sleep(1);
            }  

            wbuf[0] = 0x6b;              //write 0x40 to register 0x6b set the gyro to sleep.
            wbuf[1] = 0x40;
            write(file, wbuf, 2);
      
            close(file);  
    }  
