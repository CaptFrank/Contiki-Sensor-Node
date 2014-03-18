/*
 * shell_get_data.c
 *
 *  Created on: 2014-03-17
 *      Author: francispapineau
 */

#include "contiki.h"
#include "shell.h"

#include "shell-i2c-driver.h"

#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(shell_get_process, "get");
SHELL_COMMAND(get_data_command,
	      "get",
	      "get [num]: get ([num] times)",
	      &shell_get_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_get_process, ev, data){

	PROCESS_BEGIN();



	PROCESS_END();

}

/*---------------------------------------------------------------------------*/
void shell_get_data_init(void){
  shell_register_command(&get_data_command);
}
/*---------------------------------------------------------------------------*/
