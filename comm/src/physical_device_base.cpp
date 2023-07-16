
// platform includes
#include <comm/app/physical_device_base.h>

gmp_status_t phy_dev::attach(io_device_base* dev)
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

	//m_dev->write(dev_addr,)

	return GMP_STATUS_OK;
}
