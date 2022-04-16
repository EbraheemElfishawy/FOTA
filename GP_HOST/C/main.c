#include "main.h"
#include <stdio.h>
#include <time.h>
#include<stdlib.h>
void loading(void);
int main()
{
     /*----------------------------- Ask Menu implementation----------------------------------------*/
        printf("..");
        delay(10000);
        printf("\n +===============================================================================+");
        printf("\n |                        Loading   EKM \"FOTA\" System                            |");
        printf("\n +===============================================================================+");
        loading();
        system("cls");
        printf("\n +===============================================================================+");
        printf("\n |                         Welcome to EKM \"FOTA\" System                          |");
        printf("\n +===============================================================================+");
        printf("\n |  1 | To START                                                                 |");
        printf("\n |----|--------------------------------------------------------------------------|");
        printf("\n |  0 | To EXIT                                                                  |");
        printf("\n +===============================================================================+");
        printf("\n Kindly, enter your choice ==> ");
        uint8_t temp;
        scanf(" %d",&temp);
        if (temp ==1)
        {
        Serial_Port_Configuration();

        loading();
        #if 1
        START:
        system("cls");
        printf("\n\n +===============================================================================+");
        printf("\n |              You can now control your machine                                 |");
        printf("\n +===============================================================================+");
        printf("\n | RESET + 0 | To Blink user LED                                                 |");
        printf("\n |-----------|-------------------------------------------------------------------|");
        printf("\n | RESET + 1 | To Jump to user app (No.1)                                        |");
        printf("\n |-----------|-------------------------------------------------------------------|");
        printf("\n | RESET + 2 | To Jump to user app (No.2)                                        |");
        printf("\n |-----------|-------------------------------------------------------------------|");
        printf("\n | RESET + 3 | To switch to bootloader command mode                              |");
        printf("\n |-----------|-------------------------------------------------------------------|");
        printf("\n | RESET + 4 | To RESET                                                          |");
        printf("\n +===============================================================================+");
        printf("\n | Important: You have only 5 seconds to choose after reset                      |");
        printf("\n +===============================================================================+");
        #endif

        printf("\n Enter your mode: ");
        uint8_t mode;
        scanf(" %d",&mode);
        if(mode == 0)
        {
            Write_to_serial_port_v2(&mode,1);
            Write_to_serial_port_v2(&mode,1);
            printf("Thank you, Blinking...");
        }
        else if(mode == 1)
        {
            Write_to_serial_port_v2(&mode,1);
            Write_to_serial_port_v2(&mode,1);
            printf("Thank you, first user application is now running");
        }
        else if(mode == 2)
        {
            Write_to_serial_port_v2(&mode,1);
            Write_to_serial_port_v2(&mode,1);
            printf("Thank you, second user application is now running");
        }
        else if(mode == 4)
        {
            mode = 4;
            Write_to_serial_port_v2(&mode,1);
            Write_to_serial_port_v2(&mode,1);
            printf("Thank you, RESETTING\n");
            goto START;
        }
        else if (mode == 3)
        {
            printf("\n Thank you, waiting for command list\n");
            Write_to_serial_port_v2(&mode,1);
            Write_to_serial_port_v2(&mode,1);
            loading();

           while(1)
           {
            system("cls");
            printf("\n +===============================================================================+");
            printf("\n |                           Bootloader Command list                             |");
            printf("\n +===============================================================================+");
            printf("\n | BL Command             | No. |                Description                     |");
            printf("\n |------------------------|-----|------------------------------------------------|");
            printf("\n | BL_GET_VER             |  1  |To read the bootloader version from the MCU     |");

            #if 0
            printf("\n |------------------------|-----|------------------------------------------------|");
            printf("\n | BL_GET_HLP             |  2  |To know what are the commands supported         |");
            printf("\n |                        |     |by the bootloader.                              |");
            printf("\n |------------------------|-----|------------------------------------------------|");
            printf("\n | BL_GET_CID             |  3  |To read the MCU chip identification number.     |");
            printf("\n |------------------------|-----|------------------------------------------------|");
            printf("\n | BL_GET_RDP_STATUS      |  4  |To read the FLASH Read Protection level.        |");
            printf("\n |------------------------|-----|------------------------------------------------|");
            printf("\n | BL_GO_TO_ADDR          |  5  |To jump bootloader to specified address.        |");
            printf("\n |------------------------|-----|------------------------------------------------|");
            printf("\n | BL_FLASH_MASS_ERASE    |  6  |To mass erase or sector erase of the user flash.|");
            #endif
            printf("\n |------------------------|-----|------------------------------------------------|");
            printf("\n | BL_FLASH_ERASE         |  7  |To erase sectors from the user flash.           |");
            printf("\n |------------------------|-----|------------------------------------------------|");
            printf("\n | BL_MEM_WRITE           |  8  |To write data into different memories of the MCU|");
            printf("\n |------------------------|-----|------------------------------------------------|");
            #if 0
            printf("\n | BL_EN_R_W_PROTECT      |  9  |To enable read/write protect on different       |");
            printf("\n |                        |     |sectors of the user flash.                      |");
            printf("\n |------------------------|-----|------------------------------------------------|");
            printf("\n | BL_MEM_READ(pending)   |  10 |To read data from different memories of the MCU.|");
            printf("\n |------------------------|-----|------------------------------------------------|");
            printf("\n | BL_READ_SECTOR_P_STATUS|  11 |To read all the sector protection status.       |");
            printf("\n |------------------------|-----|------------------------------------------------|");
            printf("\n | BL_OTP_READ(pending)   |  12 |To read the OTP contents.                       |");
            printf("\n |------------------------|-----|------------------------------------------------|");
            printf("\n |                        |  13 |To disable read/write protection on different   |");
            printf("\n |                        |     |sectors of the user flash .                     |");
            printf("\n | BL_DIS_R_W_PROTECT     |     |Note:This command takes the protection status to|");
            printf("\n |                        |     |the default state.                              |");
            printf("\n |------------------------|-----|------------------------------------------------|");
            #endif
            printf("\n | MENU_EXIT              |  0  |To exit this menu                               |");
            printf("\n +===============================================================================+");

            printf("\n   Type the command code here :");

            uint32_t command_code;
            scanf(" %d",&command_code);

            decode_menu_command_code(command_code);

            #if 0
                    printf("\n\n   Do you want to continue(y/n) ?:");
                    uint8_t proceed = 0;
                    scanf(" %c",&proceed);
                    proceed -= 'y';
                    if ( proceed)
                    {
                        printf("\n  ****** Thank you ! Exiting ******\n");
                        break;
                    }
            #endif
            printf("\n\n   Press any key to continue  :");
            uint8_t ch = getch();
            purge_serial_port();
           }

            }
            else
            {
            }

        }
        else
        {
            printf("Exiting....");
        }


}
void delay(int number_of_useconds)
{
    // Converting time into milli_seconds
    int milli_seconds =  number_of_useconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}
void loading(void)
{
    uint8_t i;
    printf("\n      loading: 0%%                                                    100%%");
    printf("\n                  ");
    for (uint8_t i = 0; i < 20; i++) {delay(50);printf("-");}
    for (uint8_t i = 0; i < 20; i++) {delay(100);printf("-");}

    delay(10);
    for (uint8_t i = 0; i < 10; i++) {delay(120);printf("-");}
    printf(">");
    delay(1000);
}
