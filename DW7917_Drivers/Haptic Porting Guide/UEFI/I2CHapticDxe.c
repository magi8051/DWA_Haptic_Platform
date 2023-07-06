#include "I2CHapticDxe.h"
#include "I2CHapticDxeReg.h"

static i2c_slave_config cfg;
static void *i2c_handle = NULL;
static struct dw_haptic_func *g_func = NULL;

struct dw_haptic dw_haptic = {
	.f0 = 1700,
	.chipid = 0x59,
};

/**********************************************************
 *
 *[I2C Function For Read/Write]
 *
 *********************************************************/
i2c_status dw_i2c_read(UINT8 addr, UINT8 *val)
{
	UINT32 bRead = 0;
	i2c_status i2cstatus = I2C_SUCCESS;

	i2cstatus = i2c_read(i2c_handle, &cfg, addr, 1, val, 1, &bRead, 2500);
	if (I2C_SUCCESS != i2cstatus)
	{
		DEBUG((EFI_D_ERROR, "Read addr:0x%X error:%d\n", addr, i2cstatus));
	}
	return i2cstatus;
}

i2c_status dw_i2c_write(UINT8 addr, UINT8 val)
{
	UINT32 bWrote = 0;
	i2c_status i2cstatus = I2C_SUCCESS;
	UINT8 wdbuf = 0;

	wdbuf = val & 0x00ff;
	i2cstatus = i2c_write(i2c_handle, &cfg, addr, 1, &wdbuf, 1, &bWrote, 2500);
	if (I2C_SUCCESS != i2cstatus)
	{
		DEBUG((EFI_D_ERROR, "Write addr:0x%X data:0x%X error\n", addr, val));
	}
	return i2cstatus;
}

void dw_i2c_write_bits(UINT8 addr, UINT32 mask, UINT8 value)
{
	UINT8 reg_val = 0;

	dw_i2c_read(addr, &reg_val);
	reg_val &= mask;
	reg_val |= (value & (~mask));
	dw_i2c_write(addr, reg_val);
}

EFI_STATUS dw_i2c_init(void)
{
	i2c_instance dw_i2c_intance;
	i2c_status i2cstatus = I2C_SUCCESS;

	cfg.bus_frequency_khz = 400;
	cfg.slave_address = DW_I2C_ADDR;
	cfg.mode = I2C;
	cfg.slave_max_clock_stretch_us = 500;
	cfg.core_configuration1 = 0;
	cfg.core_configuration2 = 0;

	dw_i2c_intance = DW_I2C_INSTANCE;
	i2cstatus = i2c_open(dw_i2c_intance, &i2c_handle);
	if (I2C_SUCCESS != i2cstatus)
	{
		DEBUG((EFI_D_ERROR, "Failed to initialize instance %d I2C %d\n", DW_I2C_INSTANCE, i2cstatus));
		return EFI_DEVICE_ERROR;
	}

	return EFI_SUCCESS;
}

static void dw_hw_reset(void)
{
	EFI_STATUS Status = EFI_SUCCESS;
	EFI_TLMM_PROTOCOL *TLMMProtocol = NULL;
	UINT32 config;

	if (EFI_SUCCESS != (Status = gBS->LocateProtocol(&gEfiTLMMProtocolGuid, NULL, (void **)&TLMMProtocol)))
	{
		DEBUG((EFI_D_ERROR, "failed to get tlmm protocol\n"));
	}
	if (Status == EFI_SUCCESS)
	{
		config = EFI_GPIO_CFG(DW_GPIO_RSTN, 0, GPIO_OUTPUT, GPIO_PULL_DOWN, GPIO_16MA);
		Status = TLMMProtocol->ConfigGpio((UINT32)config, TLMM_GPIO_ENABLE);
		if (Status != EFI_SUCCESS)
		{
			DEBUG((EFI_D_ERROR, "failed to set gpio %d\n", DW_GPIO_RSTN));
		}
		Status = TLMMProtocol->GpioOut(config, GPIO_LOW_VALUE);
		gBS->Stall(2000);
		Status = TLMMProtocol->GpioOut(config, GPIO_HIGH_VALUE);
		gBS->Stall(8000);
	}
}


EFI_STATUS dw_parse_chipid(void)
{
	EFI_STATUS Status = EFI_SUCCESS;
	UINT8 reg = 0;

	/* hardware reset */
	dw_hw_reset();
	gBS->Stall(2000); /* 2ms */

	Status = dw_i2c_read(DW_REG_CHIPID, &reg);

	if (Status != EFI_SUCCESS)
		DEBUG((EFI_D_ERROR, "I2CHapticDxe: read chip id fail\n"));
	
	switch (reg)
	{
	case DW7917_CHIPID:
		DEBUG((EFI_D_ERROR, "I2CHapticDxe: detected dw7917.\n"));
		g_func = &dw7917_func_list;
		return EFI_SUCCESS;
		
	default:
		DEBUG((EFI_D_ERROR, "I2CHapticDxe: unsupport device revision (0x%02X)\n", reg));
		Status = EFI_UNSUPPORTED;
		break;
	}

	return Status;
}

static void dw_ram_init(void)
{
	DEBUG((EFI_D_ERROR, "I2CHapticDxe: dw ram init\n"));
	g_func->misc_para_init();
	g_func->play_mode(DW_MEM_MODE);
	g_func->set_ram_data(dw_wav1_data);
}

static void dw_haptic_play(void)
{
	DEBUG((EFI_D_ERROR, "I2CHapticDxe: dw ram play\n"));
	g_func->set_wav_seq(0x00, 0x01);
	g_func->set_wav_loop(0x00, 0x04);
	g_func->set_gain_mode(VD_CLAMP_ON_MODE);
	g_func->set_gain(0x80); /* set 5v */
	g_func->play_go(DW_TRUE);
}

EFI_STATUS dw_f0_cali(void)
{
	return EFI_SUCCESS;
}

EFI_STATUS EFIAPI dw_haptic_init(void)
{
	EFI_STATUS Status = EFI_SUCCESS;

	DEBUG((EFI_D_ERROR, "I2CHapticDxe: dw uefi driver version is v0.1.0\n"));
	Status = dw_i2c_init();
	if (Status != EFI_SUCCESS)
	{
		DEBUG((EFI_D_ERROR, "I2CHapticDxe: hw init err %d \n", Status));
		return Status;
	}

	Status = dw_parse_chipid();
	if (Status != EFI_SUCCESS)
	{
		DEBUG((EFI_D_ERROR, "I2CHapticDxe: read chipid fail \n"));
		return Status;
	}

	dw_ram_init();
	
	dw_haptic_play();

	DEBUG((EFI_D_ERROR, "I2CHapticDxe: dw uefi driver end \n"));

	return Status;
}

EFI_QCOM_I2C_HAPTIC_PROTOCOL I2CHapticProtocalImplementation =
	{
		I2C_HAPTIC_REVISION,
		dw_haptic_init,
};

EFI_STATUS EFIAPI I2CHapticDxeInitialize(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
	EFI_STATUS Status;
	Status = gBS->InstallProtocolInterface(
		&ImageHandle,
		&gQcomI2CHapticProtocolGuid,
		EFI_NATIVE_INTERFACE,
		&I2CHapticProtocalImplementation);
	if (EFI_ERROR(Status))
	{
		DEBUG((EFI_D_ERROR, "I2CHapticDxe: Install EFI_QCOM_I2C_HAPTIC_PROTOCOL - %r\n", Status));
		return Status;
	}
	dw_haptic_init();
	DEBUG((EFI_D_ERROR, "I2CHapticDxe: Install EFI_QCOM_I2C_HAPTIC_PROTOCOL - %r\n", Status));
	return EFI_SUCCESS;
}

/*
version v0.1.0
make 23.07.05
*/

