/* commands.h
 *
 *  Created on: 19 Feb 2019
 *      Author: Alex
 *
 *      Holds all of the commands that are allowed for the application
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

enum COMMANDS
{
	TEST = 0x00,
    ENTER_PC_MODE = 0x01,
    EXIT_PC_MODE = 0x02,
    TRANSPARENT_MODE = 0x03,
	GOTO_BOOTLOADER = 0x04,	
	PRINT_PC_MODE = 0x05,
};

#endif /* COMMANDS_H_ */