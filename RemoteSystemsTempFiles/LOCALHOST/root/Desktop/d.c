//#include <linux/module.h>
#include <stdio.h>
#include <stdlib.h>   //exit()
#include <fcntl.h>    //define O_RDWR
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

      
    void main()  
    {  
            int file,i,k,r;
            int s,h,l;


            int addr = 0x68; //i2c device address
            char *filename = "/dev/i2c-1";  
            char wbuf[] = {0x6b, 0x00}; //first byte is address to write. others are bytes to be written
            char read_start_buf[1];
            char rbuf[1000] = {'0'};
            char a[80];
            short int8[10];


            unsigned short b[1],ungy[3];     // temperature variable
            signed short gy,p[3];
            float temperature;
            short t2;



      
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




            /****************************gyro_begain************************************************/

           for(i=0;i<5;i++)
          {
               read_start_buf[0] = 0x41;    //Temperature
               write(file, read_start_buf, 1); //reposition file pointer to register 0x29
               r = read(file, b , 2 ); //read register 0x29 and save to rbuf
               printf("get:%d bytes\n",r);


               b[0]=b[0]<<8 | b[0]>>8;
               t2=(signed short)b[0];
               printf("after:%d\n",t2);


               temperature=t2/340+36.53;
               printf("T:%.2f℃\n",temperature);

/*******************************************************************************************************/

            	printf("\n");

        	    //gyro_x
                read_start_buf[0] = 0x43;
                write(file, read_start_buf, 1); //reposition file pointer to register 0x29
                read(file, p , 2);   //read register 0x29 and save to rbuf

                read_start_buf[0] = 0x43;
                write(file, read_start_buf, 1); //reposition file pointer to register 0x29
                read(file, ungy , 2);

                ungy[0]=ungy[0]<<8 | ungy[0]>>8;
                gy=(signed short)ungy[0];
                printf("x_iv:%d\t",gy/131);

                printf("x_sr%d\t",p[0]);





                //gyro_y
                read_start_buf[0] = 0x45;
                write(file, read_start_buf, 1); //reposition file pointer to register 0x29
                read(file, p , 2);   //read register 0x29 and save to rbuf

                read_start_buf[0] = 0x45;
                write(file, read_start_buf, 1); //reposition file pointer to register 0x29
                read(file, ungy , 2);

                ungy[0]=ungy[0]<<8 | ungy[0]>>8;
                gy=(signed short)ungy[0];
                printf("y_iv:%d\t",gy/131);

                printf("y_sr%d\t",p[0]);


                //gyro_z
                read_start_buf[0] = 0x47;
                write(file, read_start_buf, 1); //reposition file pointer to register 0x29
                read(file, p , 2);   //read register 0x29 and save to rbuf

                read_start_buf[0] = 0x47;
                write(file, read_start_buf, 1); //reposition file pointer to register 0x29
                read(file, ungy , 2);

                ungy[0]=ungy[0]<<8 | ungy[0]>>8;
                gy=(signed short)ungy[0];
                printf("x_iv:%d\t",gy/131);

                printf("x_sr%d\n",p[0]);








/***********************************************************/


                   // sleep(1);
            }

      
            close(file);  
    }  
