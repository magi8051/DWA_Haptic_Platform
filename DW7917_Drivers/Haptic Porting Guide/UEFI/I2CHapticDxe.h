#ifndef _I2CHAPTICDXE_H_
#define _I2CHAPTICDXE_H_
#include <Uefi.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Protocol/EFII2CHaptic.h>
#include <Protocol/EFITlmm.h>
#include <i2c_api.h>

/* Using chip control */
#define DW791x_UEFI_DRIVER /* dw7917 */
#define DW_GPIO_RSTN (6)
#define DW_I2C_INSTANCE I2C_INSTANCE_002
#define DW_I2C_ADDR (0x59) /* 8bit 0xB2 */
#define DW_I2C_DATA_SIZE (1)
#define DW_READ_CHIPID_RETRIES (1)
#define DW_I2C_BUS_FREQ (400)
#define DW_REG_CHIPID (0x00)
#define DW7917_CHIPID (0x40)

enum dw_bool
{
	DW_FALSE = 0,
	DW_TRUE = 1,
};

enum dw_gain_mode
{
	VD_CLAMP_OFF_MODE = 0,
	VD_CLAMP_ON_MODE = 1,
};

enum dw_haptic_work_mode
{
	DW_ACT_MODE = 0,
	DW_MEM_MODE = 1,
	DW_RTP_MODE = 2,
	DW_TRIG_MODE = 3,
};

enum dw_haptic_bst_mode
{
	DW_BST_BYPASS_MODE = 0,
	DW_BST_BOOST_MODE = 1,
};


struct dw_haptic
{
	uint32_t f0;
	uint32_t chipid;
};

struct dw_haptic_func
{
	void (*play_go)(uint8_t);
	void (*play_stop)(void);
	void (*set_ram_data)(uint8_t *);
	void (*misc_para_init)(void);
	void (*set_gain)(uint8_t);
	void (*set_gain_mode)(uint8_t);
	void (*play_mode)(uint8_t);
	void (*boost_mode)(uint8_t);
	void (*set_wav_seq)(uint8_t, uint8_t);
	void (*set_wav_loop)(uint8_t, uint8_t);
};

extern i2c_status dw_i2c_read(uint8_t, uint8_t *);
extern i2c_status dw_i2c_write(uint8_t, uint8_t);
extern void dw_i2c_write_bits(uint8_t, uint32_t, uint8_t);
extern struct dw_haptic_func dw7917_func_list;
extern uint8_t dw_wav1_data[];
extern uint32_t dw_ram_wav1_len;
extern struct dw_haptic dw_haptic;

#endif

/*
version v0.1.0
make 23.07.05
*/

