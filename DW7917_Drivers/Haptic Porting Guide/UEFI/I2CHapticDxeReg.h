#ifndef _I2CHAPTICDXEREG_H_
#define _I2CHAPTICDXEREG_H_


#define DW_F0_CALI						(0)
#define DW7917_WHO_AM_I					(0x40)

/*********************************************************
 *
 * DW7917 Register List
 *
 *********************************************************/
#define DW7917_REG_PWM_FREQ (0x04)
#define DW7917_REG_BOOST_MODE (0x07)
#define DW7917_REG_VD_CLAMP (0x0A)
#define DW7917_REG_ACT_MODE (0x0B)
#define DW7917_REG_PLAYBACK (0x0C)

#define DW7917_REG_WAVE_SEQ0 (0x0F)
#define DW7917_REG_WAVE_SEQ1 (0x10)
#define DW7917_REG_WAVE_SEQ2 (0x11)
#define DW7917_REG_WAVE_SEQ3 (0x12)
#define DW7917_REG_WAVE_SEQ4 (0x13)
#define DW7917_REG_WAVE_SEQ5 (0x14)
#define DW7917_REG_WAVE_SEQ6 (0x15)
#define DW7917_REG_WAVE_SEQ7 (0x16)

#define DW7917_REG_WAVE_LOOP0 (0x17)
#define DW7917_REG_WAVE_LOOP1 (0x18)
#define DW7917_REG_WAVE_LOOP2 (0x19)
#define DW7917_REG_WAVE_LOOP3 (0x1A)
#define DW7917_REG_MAIN_LOOP0 (0x17)

#define DW7917_REG_LRA_F0_MH (0x42)
#define DW7917_REG_LRA_F0_ML (0x43)

#define DW7917_REG_DEFAULT (0x00)

/* mem reg */
#define DW7917_REG_BASE_ADDRH (0x46)
#define DW7917_REG_BASE_ADDRL (0x47)
#define DW7917_REG_BASE_DATA (0x48)

/* boot mode bit */
#define DW7917_BIT_BOOST_ADAPT (1 << 3)
#define DW7917_BIT_BOOST_LUMP (1 << 2)
#define DW7917_BIT_BOOST_BYPASS (1 << 1)
#define DW7917_BIT_BOOST_EN (1 << 0)

/* mem mode set */
#define DW7917_BIT_ACT_MODE_MEM_MASK (~(1 << 0))
#define DW7917_BIT_ACT_MODE_MEM (0 << 0)
#define DW7917_BIT_ACT_MODE_RTP (1 << 0)

/* vd clamp mode set */
#define DW7917_BIT_PWM_FREQ_CLAMP_MASK (~(1 << 2))
#define DW7917_BIT_PWM_FREQ_CLAMP_ON (1 << 2)
#define DW7917_BIT_PWM_FREQ_CLAMP_OFF (0 << 2)

/*********************************************************
 *
 * Register Access
 *
 *********************************************************/
#define REG_NONE_ACCESS (0)
#define REG_RD_ACCESS (1 << 0)
#define REG_WR_ACCESS (1 << 1)

#endif

/*
version v0.1.0
make 23.07.05
*/

