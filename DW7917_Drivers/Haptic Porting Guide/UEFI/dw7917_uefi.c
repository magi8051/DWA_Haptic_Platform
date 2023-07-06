#include "I2CHapticDxe.h"
#include "I2CHapticDxeReg.h"

static void dw7917_bst_mode_config(uint8_t mode)
{
	switch (mode)
	{
	case DW7917_BIT_BOOST_ADAPT:
		DEBUG((EFI_D_ERROR, "I2CHapticDxe: boost adapt mode \n"));
		dw_i2c_write(DW7917_REG_BOOST_MODE, mode);
		break;
	case DW7917_BIT_BOOST_LUMP:
		DEBUG((EFI_D_ERROR, "I2CHapticDxe: boost lump mode \n"));
		dw_i2c_write(DW7917_REG_BOOST_MODE, mode);
		break;
	case DW7917_BIT_BOOST_BYPASS:
		DEBUG((EFI_D_ERROR, "I2CHapticDxe: boost bypass mode \n"));
		dw_i2c_write(DW7917_REG_BOOST_MODE, mode);
		break;
	case DW7917_BIT_BOOST_EN:
		DEBUG((EFI_D_ERROR, "I2CHapticDxe: boost en mode \n"));
		dw_i2c_write(DW7917_REG_BOOST_MODE, mode);
		break;
	default:
		DEBUG((EFI_D_ERROR, "I2CHapticDxe: error boost mode \n"));
		break;
	}
}

static void dw7917_play_mode(uint8_t play_mode)
{
	switch (play_mode)
	{
	case DW_ACT_MODE:
		DEBUG((EFI_D_ERROR, "I2CHapticDxe: standby mode \n"));
		break;
	case DW_MEM_MODE:
		DEBUG((EFI_D_ERROR, "I2CHapticDxe: memory mode \n"));
		dw_i2c_write_bits(DW7917_REG_ACT_MODE, DW7917_BIT_ACT_MODE_MEM_MASK, DW7917_BIT_ACT_MODE_MEM);
		break;
	case DW_RTP_MODE:
		DEBUG((EFI_D_ERROR, "I2CHapticDxe: rtp mode \n"));
		dw_i2c_write_bits(DW7917_REG_ACT_MODE, DW7917_BIT_ACT_MODE_MEM_MASK, DW7917_BIT_ACT_MODE_RTP);
		dw7917_bst_mode_config(DW7917_BIT_BOOST_LUMP);
		break;
	default:
		break;
	}
}

static void dw7917_set_wav_seq(uint8_t wav, uint8_t index)
{
	dw_i2c_write(DW7917_REG_WAVE_SEQ0 + wav, index);
}

static void dw7917_set_wav_loop(uint8_t wav, uint8_t loop)
{
	switch (wav)
	{
	case 0:
		dw_i2c_write(DW7917_REG_WAVE_LOOP0, loop << 4);
		break;
	case 1:
		dw_i2c_write(DW7917_REG_WAVE_LOOP0, loop << 0);
		break;
	case 2:
		dw_i2c_write(DW7917_REG_WAVE_LOOP1, loop << 4);
		break;
	case 3:
		dw_i2c_write(DW7917_REG_WAVE_LOOP1, loop << 0);
		break;
	case 4:
		dw_i2c_write(DW7917_REG_WAVE_LOOP2, loop << 4);
		break;
	case 5:
		dw_i2c_write(DW7917_REG_WAVE_LOOP2, loop << 0);
		break;
	case 6:
		break;
		dw_i2c_write(DW7917_REG_WAVE_LOOP3, loop << 4);
	case 7:
		dw_i2c_write(DW7917_REG_WAVE_LOOP3, loop << 0);
	}
}

static void dw7917_gain_mode(uint8_t mode)
{
	switch (mode)
	{
	case VD_CLAMP_ON_MODE:
		dw_i2c_write_bits(DW7917_REG_PWM_FREQ, DW7917_BIT_PWM_FREQ_CLAMP_MASK, DW7917_BIT_PWM_FREQ_CLAMP_ON);
		break;
	case VD_CLAMP_OFF_MODE:
		dw_i2c_write_bits(DW7917_REG_PWM_FREQ, DW7917_BIT_PWM_FREQ_CLAMP_MASK, DW7917_BIT_PWM_FREQ_CLAMP_OFF);
		break;
	}
}

static void dw7917_set_gain(uint8_t gain)
{
	/* gain x 0.04v */
	dw_i2c_write(DW7917_REG_VD_CLAMP, gain);
}

static void dw7917_set_ram_data(uint8_t *wav)
{
	uint32_t i = 0;

	/* set header addr */
	dw_i2c_write(DW7917_REG_BASE_ADDRH, wav[0]);
	dw_i2c_write(DW7917_REG_BASE_ADDRL, wav[1]);

	/* write header data */
	for (i = 0; i < 5; i++)
		dw_i2c_write(DW7917_REG_BASE_DATA, wav[i + 2]);

	/* set pcm addr */
	dw_i2c_write(DW7917_REG_BASE_ADDRH, wav[2]);
	dw_i2c_write(DW7917_REG_BASE_ADDRL, wav[3]);

	/* write pcm data */
	for (i = 0; i < dw_ram_wav1_len; i++)
		dw_i2c_write(DW7917_REG_BASE_DATA, wav[i + 7]);
}

static void dw7917_play_go(uint8_t flag)
{
	dw_i2c_write(DW7917_REG_PLAYBACK, flag);
}

static void dw7917_play_stop(void)
{
	dw_i2c_write(DW7917_REG_PLAYBACK, DW_FALSE);
}

static void dw7917_read_f0(void)
{
	uint32_t f0;
	uint8_t buf[2];
	dw_i2c_read(DW7917_REG_LRA_F0_MH, &buf[0]);
	dw_i2c_read(DW7917_REG_LRA_F0_ML, &buf[1]);
	f0 = ((uint16_t)buf[0] << 8) | buf[1];
	DEBUG((EFI_D_ERROR, "I2CHapticDxe: lra f0=%d\n", f0));
}

static void dw7917_misc_para_init(void)
{
	dw_i2c_write(DW7917_REG_PWM_FREQ, DW7917_REG_DEFAULT);
	dw_i2c_write(DW7917_REG_BOOST_MODE, DW7917_BIT_BOOST_LUMP);
}

struct dw_haptic_func dw7917_func_list = {
	.play_go = dw7917_play_go,
	.play_stop = dw7917_play_stop,
	.set_ram_data = dw7917_set_ram_data,
	.misc_para_init = dw7917_misc_para_init,
	.set_gain = dw7917_set_gain,
	.set_gain_mode = dw7917_gain_mode,
	.boost_mode = dw7917_bst_mode_config,
	.play_mode = dw7917_play_mode,
	.set_wav_seq = dw7917_set_wav_seq,
	.set_wav_loop = dw7917_set_wav_loop,
};

/*
version v0.1.0
make 23.07.05
*/
