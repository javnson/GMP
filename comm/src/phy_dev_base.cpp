
// platform includes
#include <comm/phy_dev_base.h>

gmp_stat_t analog_sensor::setup()
{
	return cmd(AS_CMD_SETUP);
}

gmp_stat_t analog_sensor::set_accuracy(uint32_t accuracy)
{
	return cmd(AS_CMD_SET_ACCURACY, accuracy, 0);
}

gmp_stat_t analog_sensor::set_sample_duartion(uint32_t sr)
{
	return cmd(AS_CMD_SET_DURATION, sr, 0);
}

uint32_t analog_sensor::get_src_data(uint32_t record)
{
	if (error(DEVICE_ERR_UNSUPPORT_R)) {
		return 0;
	}
	return 0;
}



gmp_stat_t analog_sensor::cmd(uint32_t cmd)
{
	switch (cmd) {
	case AS_CMD_GET_SRC_DATA:
		if (is_readable())
		{
			get_src_data(0);
			return GMP_STAT_OK;
		}
		return GMP_STAT_GENERAL_ERROR;

	case AS_CMD_CALIBRATION:
		return GMP_STAT_OK;

	case AS_CMD_SET_ACCURACY:
		return GMP_STAT_OK;

	case AS_CMD_SET_DURATION:
		return GMP_STAT_OK;
	}

	return protocol_base::cmd(cmd);
}

gmp_stat_t analog_sensor::cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
{
	switch (cmd) {
	case AS_CMD_GET_SRC_DATA:
		if (is_readable())
		{
			gmp_assert(lparam != 0);
			*lparam = get_src_data(wparam);
			return GMP_STAT_OK;
		}
		return GMP_STAT_GENERAL_ERROR;

	case AS_CMD_CALIBRATION:
		return GMP_STAT_OK;

	case AS_CMD_SET_ACCURACY:
		return GMP_STAT_OK;

	case AS_CMD_SET_DURATION:
		return GMP_STAT_OK;
	}

	return protocol_base::cmd(cmd, wparam, lparam);
}

gmp_diff_t analog_sensor::read_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length)
{

	filter->write(dev_addr,addr,length)
	filter->read(dev_addr,)
	return 0;
}

gmp_diff_t analog_sensor::write_ex(_IN gmp_addr_t addr, _IN gmp_data_t* data, gmp_size_t length)
{
	return 0;
}

//gmp_stat_t phy_dev::attach(io_device_base* dev)
//{
//	m_dev = dev;
//	return GMP_STAT_OK;
//}
//
//gmp_stat_t phy_dev::cmd(uint32_t cmd)
//{
//	return GMP_STAT_OK;
//}
//
//gmp_stat_t phy_dev::cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
//{
//	return GMP_STAT_OK;
//}

//gmp_stat_t record_dev::push(gmp_size_t n)
//{
//
//	return GMP_STAT_OK;
//}
//
//gmp_stat_t record_dev::pull(gmp_size_t n)
//{
//	gmp_assert(m_dev != nullptr);
//	gmp_assert(n < record_len);
//
//	m_dev->write(addr, (gmp_data_t*)&(records[n].cmd), 1);
//
//	gmp_delay(180);
//
//	m_dev->read(addr, (gmp_data_t*)&(records[n].value), records[n].v_length);
//
//	return GMP_STAT_OK;
//}
//
//gmp_stat_t record_dev::cmd(uint32_t cmd)
//{
//	return GMP_STAT_OK;
//}
//
//gmp_stat_t record_dev::cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam)
//{
//	return GMP_STAT_OK;
//}

