#ifndef __EFI_I2C_HAPTIC_H__
#define __EFI_I2C_HAPTIC_H__

#define I2C_HAPTIC_REVISION 0x0000000000010005

#define EFI_I2C_HAPTIC_PROTOCOL_GUID { 0x038f1af5, 0x1c8d, 0x408f, { 0xab, 0x25, 0x30, 0xae, 0xb5, 0x96, 0x5d, 0x6e } }

extern EFI_GUID gQcomI2CHapticProtocolGuid;

typedef struct _EFI_QCOM_I2C_HAPTIC_PROTOCOL EFI_QCOM_I2C_HAPTIC_PROTOCOL;

typedef EFI_STATUS (EFIAPI *EFI_I2C_HAPTIC_INIT) (VOID);

struct _EFI_QCOM_I2C_HAPTIC_PROTOCOL {
	UINT64 Revision;
	EFI_I2C_HAPTIC_INIT Init;
};

#endif  /* __EFI_I2C_HAPTIC_H__ */