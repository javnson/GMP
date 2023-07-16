
// platform includes
#include <comm/app/physical_device_base.h>

gmp_status_t phy_dev::attach(io_device_base* dev)
{
	m_dev = dev;
	return GMP_STATUS_OK;
}

gmp_ptrdiff_t phy_dev::command(uint32_t cmd)
{
	return GMP_STATUS_OK;
}

gmp_ptrdiff_t phy_dev::command(uint32_t cmd, gmp_param_t wparam, gmp_ptraddr_t lparam)
{
	return GMP_STATUS_OK;
}

gmp_status_t record_dev::push(gmp_size_t n)
{

	return GMP_STATUS_OK;
}

gmp_status_t record_dev::pull(gmp_size_t n)
{
	gmp_assert(m_dev != nullptr);
	gmp_assert(n < record_len);

	m_dev->write(addr, (gmp_data_t*)&(records[n].cmd), 1);

	gmp_delay(180);

	m_dev->read(addr, (gmp_data_t*)&(records[n].value), records[n].v_length);

	return GMP_STATUS_OK;
}

gmp_ptrdiff_t record_dev::command(uint32_t cmd)
{
	return GMP_STATUS_OK;
}

gmp_ptrdiff_t record_dev::command(uint32_t cmd, gmp_param_t wparam, gmp_ptraddr_t lparam)
{
	return GMP_STATUS_OK;
}

