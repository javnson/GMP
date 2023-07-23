
// platform include file
#include <comm/comm_basic.h>

// STM32 platform
#include <bsp/hardware_relative_headers.h>

// Usage
// 1. call attach function to link peripheral handle with m_dev (device base address)


class spi_device_stm32
	:public spi_device
{
public:
	// ctor & dtor
	spi_device_stm32()
	{
		m_handle = NULL;
	}

	~spi_device_stm32()
	{

	}

public:
	// kernel virtual function

	/**
	 * @brief read a string of message from UART. This function is strongly related to physical device.
	 * @param addr no use
	 * @param data the pointer for data, which would be treated as a buffer
	 * @param length the capacity of the buffer
	 * @return the real length of the data buffer
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230605
	 */
	virtual gmp_diff_t read_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

	/**
	 * @brief write a string of message for the device. The implement of the function is blocked.
	 * @param addr no use
	 * @param data the pointer for data, which would be sent
	 * @param length the length of the data buffer
	 * @return real length that been written
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230605
	 */
	virtual gmp_diff_t write_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

	// This class will implement the cmd function.
	RESPONSE_CMD

public:
	// peripheral function
	/**
	 * @brief Init function initialize the peripheral, maintaining state machine.
	 *        If you is using initialize code generation, for example CUBEMX, you should using macro GMP_INIT_DEPOSIT.
	 *		  This function would only change the state machine without any substantive operations.
	 * @return if the command can't run correctly, the program would abort.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230704
	 */
	virtual void init();

	/**
	 * @brief This function reset the peripheral, maintaining state machine.
	 *        If you is using initialize code generation, for example CUBEMX, you should using macro GMP_INIT_DEPOSIT.	 
	 *		  This function would only change the state machine without any substantive operations.
	 * @return if the command can't run correctly, the program would abort.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230704
	 */
	virtual void reset();

	/**
	 * @brief Init function initialize the peripheral, maintaining state machine.
	 *        If you is using initialize code generation, for example CUBEMX, you should using macro GMP_INIT_DEPOSIT.	 
	 * @return if the command can't run correctly, the program would abort.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230704
	 */
	virtual void shutdown();

	// register a SPI device
	// @brief bind a SPI channel with SPI handle
	// @param spi_channel, a pointer to SPI peripheral base address, such as SPI6
	// @param spi_handle, STM32 manage SPI device as a handle form so the class will
	//           store SPI handle meantime.
	void reg_device(SPI_TypeDef* spi_channel, SPI_HandleTypeDef* spi_handle);

public:
	// public members
	SPI_HandleTypeDef* m_handle;
};


// @brief IIC device for STM32 series micro-controller
class i2c_device_stm32
	:public i2c_device
{
public:
	// ctor & dtor
	i2c_device_stm32()
	{
		m_handle = NULL;
	}

	~i2c_device_stm32()
	{

	}

public:
	// kernel virtual function
	// 
	// This class will implement the R/W functions.
protected: virtual gmp_diff_t read_ex(gmp_addr_t addr, gmp_data_t* data, gmp_size_t length); virtual gmp_diff_t write_ex(gmp_addr_t addr, gmp_data_t* data, gmp_size_t length);

	// This class will implement the cmd function.
public: virtual gmp_stat_t cmd(uint32_t cmd); virtual gmp_stat_t cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam);


	/**
	 * @brief read a string of message from i2c. This function is strongly related to physical device.
	 * @param addr device address
	 * @param data the pointer for data, which would be treated as a buffer
	 * @param length the capacity of the buffer
	 * @return the real length of the data buffer
	 * @author : Javnson
	 *			 Gqc
	 * @date   : 20230715
	 */
//	virtual gmp_diff_t read_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

	/**
	 * @brief write a string of message for the device. The implement of the function is blocked.
	 * @param addr no use
	 * @param data the pointer for data, which would be sent
	 * @param length the length of the data buffer
	 * @return real length that been written
	 * @note 2 types of warning may be caused by the function.
	 *        + DEVICE_ERR_TIMEOUT : transmit timeout
	 *        + DEVICE_ERR_HAL     : use get_hal_errcode() for details
	 * @author : Javnson
	 * @date   : 20230605
	 */
//	virtual gmp_diff_t write_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

	/**
	 * @brief This function could pass a command.
	 * @param cmd the command.
	 * @return if the command is done corrected.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
//		virtual gmp_stat_t cmd(uint32_t cmd);
//	virtual gmp_stat_t cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam);

public:
	// peripheral function
	/**
	 * @brief Init function initialize the peripheral, maintaining state machine.
	 *        If you is using initialize code generation, for example CUBEMX, you shold using macro GMP_INIT_DEPOSIT.
	 *		  This function would only change the state machine without any substantive operations.
	 * @return if the command can't run correctly, the program would abort.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230704
	 */
//	virtual void init();

	/**
	 * @brief This function reset the peripheral, maintaining state machine.
	 *        If you is using initialize code generation, for example CUBEMX, you shold using macro GMP_INIT_DEPOSIT.	 *		  This function would only change the state machine without any substantive operations.
	 * @return if the command can't run correctly, the program would abort.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230704
	 */
//	virtual void reset();

	/**
	 * @brief Init function initialize the peripheral, maintaining state machine.
	 *        If you is using initialize code generation, for example CUBEMX, you shold using macro GMP_INIT_DEPOSIT.	 * @return if the command can't run correctly, the program would abort.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230704
	 */
//	virtual void shutdown();

	// register a SPI device
	// @brief bind a SPI channel with SPI handle
	// @param spi_channel, a pointer to SPI peripheral base address, such as SPI6
	// @param spi_handle, STM32 manage SPI device as a handle form so the class will
	//           store spi handle meantime.
	void reg_device(I2C_TypeDef* spi_channel, I2C_HandleTypeDef* spi_handle);

public:
	// public members
	I2C_HandleTypeDef* m_handle;

public:
	// utilities
	uint32_t get_hal_errcode()
	{
		gmp_assert(m_handle != 0);
		return m_handle->ErrorCode;
	}
};
