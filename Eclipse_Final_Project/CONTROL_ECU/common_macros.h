/**************************************
 * Module: Common-Macros
 * File name: common_macros.h
 * Description: commonly used macros
 * Author: amr mohsen
 *************************************/

#ifndef COMMON_MACROS
#define COMMON_MACROS

/* SET a certan bit in any register */
#define SET_BIT(REG,BIT) (REG|=(1<<BIT))

/* CLEAR a certan bit in any register */
#define CLEAR_BIT(REG,BIT) (REG&=(~(1<<BIT)))

/* TOGGLE a certan bit in any register */
#define TOGGLE_BIT(REG,BIT) (REG^=(1<<BIT))

/* GET a certan bit in any register */
#define GET_BIT(REG,BIT) ((REG & (1<<BIT)) >> BIT )

/* ROTATE right the register value with specific number of rotates */
#define ROR(REG,num) (REG=(REG>>num)|(REG<<(8-num)))

/* ROTATE left the register value with specific number of rotates */
#define ROL(REG,num) (REG=(REG<<num)|(REG>>(8-num)))

/* Check if a specific bit is set in any register and return true if yes */
#define BIT_IS_SET(REG,BIT) (REG &(1<<BIT))

/* Check if a specific bit is cleared in any register and return true if yes */
#define BIT_IS_CLEAR(REG,BIT) (!(REG &(1<<BIT)))

#endif









