#include "bh1750.h"

gmp_ptrdiff_t bh1750::command(uint32_t cmd)
{
	switch (cmd)
	{
	case PHY_CMD_SHUTDOWN:
		m_dev->write(addr, BH1750_CMD_POWER_DOWN, 1);
		break;

	case PHY_CMD_RESET:
		m_dev->write(addr, BH1750_CMD_RESET, 1);
		break;

	case PHY_CMD_INIT:
		m_dev->write(addr, BH1750_CMD_POWER_ON, 1);
		break;
	}

	return cmd;
}

gmp_ptrdiff_t bh1750::command(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{
	return GMP_STATUS_OK;
}

