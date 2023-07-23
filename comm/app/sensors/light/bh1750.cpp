#include "bh1750.h"

gmp_stat_t bh1750::cmd(uint32_t cmd)
{
	uint8_t opecode;
	switch (cmd)
	{
	case PHY_CMD_SHUTDOWN:
		opecode = BH1750_CMD_POWER_DOWN;
		//m_dev->write(addr, BH1750_CMD_POWER_DOWN, 1);
		break;

	case PHY_CMD_RESET:
		opecode = BH1750_CMD_RESET;
		//m_dev->write(addr, BH1750_CMD_RESET, 1);
		break;

	case PHY_CMD_INIT:
		opecode = BH1750_CMD_POWER_ON;
		//m_dev->write(addr, BH1750_CMD_POWER_ON, 1);
		break;

	default:
		cmd_device::error(DEVICE_ERR_BAD_CMD);
		return GMP_STAT_GENERAL_WARN;
		break;
	}
	m_dev->write(addr, &opecode, 1);

	return record_dev::cmd(cmd);
}

gmp_stat_t bh1750::cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{
	return GMP_STAT_OK;
}

