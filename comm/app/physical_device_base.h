
// platform includes
#include <./base/gmp_base.h>
#include <./comm/comm_basic.h>

#pragma region PHY_DEF

// This type is for phy_device definition

// + 0-3, 
typedef union _tag_phy_state_t
{
	struct
	{
		uint32_t state_machine : 4;

	}bits;

	uint32_t all;
} phy_state_t;

// ready <-> preop <-> online <-> buzy
//  ^                     |
//  \ - - - - - - - - - - /
// gmp_device_state_t::bits::state_machine provides 4 inner states
#define PHY_STATE_READY          (0x00)
#define PHY_STATE_PREOP          (0x01)
#define PHY_STATE_ONLINE         (0x02)
#define PHY_STATE_BUZY           (0x03)
#define PHY_STATE_SHUTDOWN       (0x04)
#define PHY_STATE_ERROR          (0x05)


#pragma endregion


#define PHY_CMD_NULL			(0x00)
#define PHY_CMD_RESET			(0x01)
#define PHY_CMD_INIT			(0x02)
#define PHY_CMD_SHUTDOWN		(0x03)


#pragma region ErrorCode
// This region provide error code for phy_device

// unsupport operation
#define PHY_ERR_UNSUPPORT


#pragma endregion ErrorCode

class phy_dev
	: public cmd_device
{
	// UUID section
public:


protected:
	// This is the device handle
	io_device_base* m_dev;
	// device address on the bus
	uint32_t dev_addr;



public:
	phy_dev() 
	{

	}

	~phy_dev()
	{

	}

public:

	// This class will implement the cmd function.
	RESPONSE_CMD

public:
	/**
	 * @brief This fucntion attach device handle to the physical device.
	 * @param dev the device to be attached.
	 * @return the state of the function
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230703
	 */
	gmp_stat_t attach(io_device_base* dev);

	/**
	 * @brief This fucntion attach device handle to the physical device.
	 * @param dev the device to be attached.
	 * @return the state of the function
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230703
	 */


private:

};


typedef struct _tag_dev_record_t
{
	// The name of the record
//	uint32_t key;

	// memory space & local mapping.
	uint32_t value;
	uint8_t v_length;

	// The command retriving the value
	uint32_t cmd;
}dev_record_t;




class record_dev
	: public phy_dev
{
public:
	// ctor & dtor
	record_dev()
	{
		records = nullptr;
		record_len = 0;
		addr = 0x0000;
	}

	~record_dev()
	{}

public:
	gmp_stat_t push(gmp_size_t n);
	gmp_stat_t pull(gmp_size_t n);

	gmp_stat_t push();
	gmp_stat_t pull();

public:
	// kernal virtual function

	// This class will implement the cmd function.
	RESPONSE_CMD


public:
	// utilities

public:
	// members
	dev_record_t *records;
	uint32_t record_len;
	gmp_addr_t addr;
};
