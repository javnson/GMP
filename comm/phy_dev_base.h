
// platform includes
#include <base/gmp_base.h>
#include <comm/comm_base.h>
#include <comm/protocol_base.h>

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
#define PHY_STAT_READY          (0x00)
#define PHY_STAT_PREOP          (0x01)
#define PHY_STAT_ONLINE         (0x02)
#define PHY_STAT_BUZY           (0x03)
#define PHY_STAT_SHUTDOWN       (0x04)
#define PHY_STAT_ERROR          (0x05)
				

#pragma endregion

#pragma region ErrorCode
// This region provide error code for phy_device

// unsupported operation
#define PHY_ERR_UNSUPPORT


#pragma endregion ErrorCode

#pragma region AS_COMMAND

// Analog sensor protocol
#define AS_CMD_CALIBRATION  	(0x1001)
#define AS_CMD_GET_SRC_DATA     (0x1002)
#define AS_CMD_SET_ACCURACY     (0x1003)
#define AS_CMD_SET_DURATION		(0x1004)

#pragma endregion AS_COMMAND

#pragma endregion AS_COMMAND



// @brief User should implement a set of command of analog sensor.
// User should derive this class and implement the virtual function `get_src_data()`
class analog_sensor
	:public protocol_base
{
public:
	// UUID

public:
	//ctor & dtor
	analog_sensor()
	{
<<<<<<< HEAD:comm/phy_dev_base.h
		character.all = DEVICE_STATE_CHAR_NULL;
=======

>>>>>>> cc031c38e28838053134ba58b6ec154ba14c6ed4:comm/physical_dev_base.h
	}

	~analog_sensor()
	{

	}

public:
	// kernel virtual function
	gmp_stat_t setup();

	gmp_stat_t set_accuracy(uint32_t accuracy);

	gmp_stat_t set_sample_duartion(uint32_t sr);
	
	virtual uint32_t get_src_data(uint32_t record = 0);

public:
	// kernel virtual function

	// This class will implement the cmd function.
	RESPONSE_CMD

	// This class will implement the R/W functions.
	RESPONSE_RW

public:
	// device physical address on the bus.
	gmp_addr_t dev_addr;


};


// @brief This class provided a set of commands to describe a sensor device.
//class sensor
//	: public cmd_device
//{
//public:
//	// UUID
//
//public:
//	// ctor & dtor
//	
//public:
//	uint32_t get_src_data(uint32_t index);
//
//	gmp_stat_t init();
//
//	gmp_stat_t set_accuracy();
//
//	gmp_stat_t set_sample_duartion();
//
//
//public:
//	// This is the device handle
//	// This handle should implement a set of Command.
//	analog_sensor_protocol* m_dev;
//
//public:
//	// kernel virtual functions
//	RESPONSE_CMD
//
//};

// @brief block read & write device. meanwhile providing a set of command.
//class flash
//	: public io_device_base
//{
//public:
//	// ctor & dtor
//
//};
//
//class phy_dev
//	: public cmd_device
//{
//	// UUID section
//public:
//
//
//protected:
//	// This is the device handle
//	io_device_base* m_dev;
//	// device address on the bus
//	uint32_t dev_addr;
//
//
//
//public:
//	phy_dev() 
//	{
//
//	}
//
//	~phy_dev()
//	{
//
//	}
//
//public:
//	// kernel virtual function
//
//	/**
//	 * @brief This function read a register of the device.
//	 * @param reg_addr the address of the register, sometimes is op-code
//	 * @len the reg length by unit byte,
//	 * @return the value of the register
//	 * @author : Javnson, Gqc
//	 * @date   : 20230723
//	 */
//	virtual uint32_t read(uint32_t reg_addr, uint8_t len);
//
//	/**
//	 * @brief This function write a register of the device.
//	 * @param reg_addr the address of the register, sometimes is op-code
//	 * @param reg_content the content to be write, at most 4 bytes
//	 * @return if the write operation is done correctly.
//	 * @author : Javnson, Gqc
//	 * @date   : 20230723
//	 */
//	virtual gmp_stat_t write(uint32_t reg_addr, uint32_t reg_content, uint8_t len);
//
//
//
//	// This class will implement the cmd function.
//	RESPONSE_CMD
//
//public:
//	/**
//	 * @brief This fucntion attach device handle to the physical device.
//	 * @param dev the device to be attached.
//	 * @return the state of the function
//	 * @author : Javnson(javnson@zju.edu.cn)
//	 * @date   : 20230703
//	 */
//	gmp_stat_t attach(io_device_base* dev);
//
//
//private:
//
//};


//typedef struct _tag_dev_record_t
//{
//	// The name of the record
////	uint32_t key;
//
//	// memory space & local mapping.
//	uint32_t value;
//	uint8_t v_length;
//
//	// The command retrieving the value
//	uint32_t cmd;
//}dev_record_t;
//
//
//
//
//class record_dev
//	: public phy_dev
//{
//public:
//	// ctor & dtor
//	record_dev()
//	{
//		records = nullptr;
//		record_len = 0;
//		addr = 0x0000;
//	}
//
//	~record_dev()
//	{}
//
//public:
//	gmp_stat_t push(gmp_size_t n);
//	gmp_stat_t pull(gmp_size_t n);
//
//	gmp_stat_t push();
//	gmp_stat_t pull();
//
//public:
//	// kernel virtual function
//
//	// This class will implement the cmd function.
//	RESPONSE_CMD
//
//
//public:
//	// utilities
//
//public:
//	// members
//	dev_record_t *records;
//	uint32_t record_len;
//	gmp_addr_t addr;
//};
//
//// @brief This class describe a type of device that owns a lot of registers.
//// Local buffer is not necessary. Every time you request a value or writing a value 
//// this class will synchronize your request to the device, by writing registers.
//class reg_dev
//	:public phy_dev
//{
//public:
//	// ctor & dtor
//	reg_dev()
//	{}
//
//	~reg_dev()
//	{
//
//	}
//
//public:
//	// kernel virtual function
//
//
//	virtual uint32_t read(uint32_t reg_addr,uint8_t len);
//
//	virtual uint32_t write(uint32_t reg_addr, uint32_t reg_content, uint8_t len);
//
//
//	// This class will implement the cmd function.
//	RESPONSE_CMD
//
//public:
//	uint32_t device_addr;
//};

