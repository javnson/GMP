
#include <string.h>

// basic objects
#include <./basic/gmp_base.h>
#include <./basic/uuid.h>

// communication classes Universally Unique IDentifiers
//#define UUID_IO_DEVICE_BASE { 0xb18a,0xbc1c,0xf9aa,0x4634,0x944a,0x39b7,0xdcef,0xe829 }
#define UUID_IO_DEVICE_UART { 0xb663,0x087d,0x5661,0x475c,0xb85e,0x939e,0xa806,0x0c99 }
#define UUID_IO_DEVICE_IIC  { 0x966d,0xd336,0x2e52,0x4d4e,0xa4e8,0x7f14,0x6db9,0x23c4 }
#define UUID_IO_DEVICE_SPI  { 0xc584,0x2a4b,0xe89b,0x428d,0x8465,0xa668,0xd85e,0x596a }
#define UUID_IO_DEVICE_DAC  { 0xb18a,0xbc1c,0xf9aa,0x4634,0x944a,0x39b7,0xdcef,0xe829 }

// Device State definition
#pragma region DEVICE_STATE_DEF

// This type is to store device state.
// + 0-3  , 4 bits, device state machine.
// + 4    , 1 bit , device lock status.
// + 5    , 1 bit , warning alarm
// + 6-7  , 1 bit , verbose level
// + 8-11 , 4 bits, PnP(Plug and Play) status
// + 12-15, 4 bits, characters, W(rite) | R(ead) | C(onfigure) | R(eserve)
typedef union _tag_device_state
{
	struct
	{
		uint32_t state_machine : 4;
		uint32_t characters : 4;
		uint32_t roles : 4;
		uint32_t lock : 2;
		uint32_t warning : 1;
		uint32_t verbose : 2;
		uint32_t pnp : 4;
		uint32_t rpc : 4;
	}bits;

	uint32_t all;
}gmp_device_state_t;

// ready <-> buzy
// gmp_device_state_t::bits::state_machine provides 4 inner states
#define DEVICE_STATE_SHUTDOWN       (0x00) // Power-Off
#define DEVICE_STATE_READY          (0x01) // ready to be buzy
#define DEVICE_STATE_BUZY           (0X02) // running receiving or transmitting
#define DEVICE_STATE_ERROR          (0x03) // meet a fatal error, need to be re initialization
#define DEVICE_STATE_RUNTIME_ERROR  (0x04) // meet a runtime error, should reset error reg, and then ready again
#define DEVICE_STATE_LOWPOWER       (0x05) // lowpower mode, waiting to be weakup, and then ready


// gmp_device_state_t::bits::characters provides 4 inner characters.
#define DEVICE_STATE_CHAR_NULL      (0x00)
#define DEVICE_STATE_CHAR_W         (0x01)
#define DEVICE_STATE_CHAR_R         (0x02)
#define DEVICE_STATE_CHAR_C         (0x04)
//#define DEVICE_STATE_CHAR_R         (0x08)

// gmp_device_state_t::bits::roles
#define DEVICE_STATE_ROLE_NULL		(0x00)
#define DEVICE_STATE_ROLE_MASTER	(0x01)
#define DEVICE_STATE_ROLE_SLAVE		(0x02)
#define DEVICE_STATE_ROLE_MGR		(0x03)

// gmp_device_state_t::bits::lock
#define DEVICE_STATE_LOCK_DISABLE   (0x00)
#define DEVICE_STATE_LOCK           (0x01)
#define DEVICE_STATE_UNLOCK         (0x02)

// gmp_device_state_t::bits::warning
#define DEVICE_STATE_WARNING        (0x01)
#define DEVICE_STATE_NORMAL         (0x00)

// gmp_device_state_t::bits::verbose
#define DEVICE_STATE_VERBOSE_NULL   (0x00)
#define DEVICE_STATE_VERBOSE_1      (0x01) // less print
#define DEVICE_STATE_VERBOSE_2      (0x02)
#define DEVICE_STATE_VERBOSE_3      (0x03) // more print

// gmp_device_state_t::bits::pnp
#define DEVICE_STATE_PNP_DISBALE    (0x00) // PNP method is disable
#define DEVICE_STATE_PNP_DISCONNET  (0x01)
#define DEVICE_STATE_PNP_ATTACHED   (0x02)
#define DEVICE_STATE_PNP_EJECT      (0x03)

// gmp_device_state_t::bits::rpc
#define DEVICE_STATE_RPC_DISABLE    (0x00) // disable RPC

#pragma endregion DEVICE_STATE_DEF

// Command Definition
#pragma region CommandDefinition

// This is device CMD type
typedef uint32_t gmp_device_cmd;

// Basic Command
#define DEVICE_CMD_NULL		(0x00)
#define DEVICE_CMD_RESET	(0x01)
#define DEVICE_CMD_INIT     (0x02)
#define DEVICE_CMD_SHUTDOWN (0x03)
#define DEVICE_CMD_LOCK     (0x04)
#define DEVICE_CMD_UNLOCK   (0x05)
#define DEVICE_CMD_VERBOSE  (0x06) // Set verbose level
#define DEVICE_CMD_REGISTER (0x07)

// PNP Command
#define DEVICE_CMD_PNP_TEST  (0x10) // Test a PnP device if it is connected to the master
#define DEVICE_CMD_PNP_EJECT (0x11) // Notify the device will be remove.


// Device General Config
#define DEVICE_CMD_GET_BAUD  (0x101)
#define DEVICE_CMD_SET_BAUD  (0x102)
#define DEVICE_CMD_GET_UUID  (0x103)
#define DEVICE_CMD_SET_UUID  (0x104)

// UART device COMMAND define


// IIC device COMMAND define
#define IIC_CMD_SET_MODE	(0x301)
#define IIC_CMD_ACK			(0x302)

// SPI device COMMAND define


// Device User Command Config
#define DEVICE_CMD_USER_BEGIN (0x8000)

#pragma endregion CommandDefinition

// Error Code Definition
#pragma region ErrorCode
// io_device_base error code definition

// unsupported operation happened
#define DEVICE_ERR_UNSUPPORT_R        (0x0010)
#define DEVICE_ERR_UNSUPPORT_W        (0x0011)
#define DEVICE_ERR_UNSUPPORT_C        (0x0012)
#define DEVICE_ERR_LOCKED             (0x0013)
#define DEVICE_ERR_NOT_READY          (0x0014)
#define DEVICE_ERR_MEMORY_UNAVAILABLE (0x0015)
#define DEVICE_ERR_UNKNOWN_CMD		  (0x0016)
#define DEVICE_ERR_UNKNOWN_CMDEX	  (0x0017)
#define DEIVCE_ERR_BAD_CMD			  (0x0018)
#define DEVICE_ERR_CANNOT_INIT	      (0x0019)
#define DEVICE_RECOVER_FROM_ERROR     (0x001A)
#define DEVICE_RECOVER_FROM_RUNTIME   (0x001B)
#define DEVICE_ERR_TIMEOUT			  (0x001C)

#define DEVICE_ERR_HAL				  (0x00FF)
// fatal errors definition
#define DEIVCE_IVALID_OPER			  (GMP_STATUS_FATAL_ERROR+0x01)
#pragma endregion ErrorCode


// Main class definition
#pragma region DeviceBase
class io_device_base
{
public:
	uuid_t m_uuid;
	gmp_errcode_t last_err;
protected:
	void* m_dba; // device base address
	gmp_device_state_t m_state;

public:
	// ctor & dtor
	io_device_base()
		//        :m_uuid(UUID_ZERO),last_err(0),
		//        m_dba(nullptr),m_state(0)
	{
//		m_uuid.uuid = UUID_ZERO;

		last_err = 0;
		m_dba = NULL;
		m_state.all = 0;

		m_state.bits.state_machine = DEVICE_STATE_SHUTDOWN;
		m_state.bits.characters = DEVICE_STATE_CHAR_NULL;
		m_state.bits.roles = DEVICE_STATE_ROLE_NULL;
		m_state.bits.lock = DEVICE_STATE_LOCK_DISABLE;
		m_state.bits.warning = DEVICE_STATE_NORMAL;
		m_state.bits.verbose = DEVICE_STATE_VERBOSE_1;
		m_state.bits.pnp = DEVICE_STATE_PNP_DISBALE;
	}
	~io_device_base()
	{}

public: // Core functions
	/**
	 * @brief read a string of message from the device, which will call read_ex to fulfill the task.
	 * @param addr the address for the device
	 * @param data the pointer for data, which are treated as a read buffer
	 * @param length the length of the data buffer
	 * @return real length that been read
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230605
	 */
	gmp_ptrdiff_t read(_IN gmp_ptraddr_t addr, _OUT gmp_data_t* data, gmp_size_t length);
	/**
	 * @brief write a string of message for the device, which will call write_ex to fulfill the task.
	 * @param addr the address for the device
	 * @param data the pointer for data, which would be sent
	 * @param length the length of the data buffer
	 * @return real length that been written
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230605
	 */
	gmp_ptrdiff_t write(_IN gmp_ptraddr_t addr, _IN gmp_data_t* data, gmp_size_t length);

	/**
	 * @brief read a string of message from the device, which will call read_ex to fulfill the task.
	 *        based on the position of m_pos, you may change the position by seek.
	 * @param data the pointer for data, which are treated as a read buffer
	 * @param length the length of the data buffer
	 * @return real length that been read
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230605
	 */
	 //gmp_ptrdiff_t read(_OUT gmp_data_t* data, gmp_size_t length);
	 /**
	  * @brief write a string of message for the device, which will call write_ex to fulfill the task.
	  *        based on the position of m_pos, you may change the position by seek.
	  * @param addr the address for the device
	  * @param data the pointer for data, which would be sent
	  * @param length the length of the data buffer
	  * @return real length that been written
	  * @author : Javnson(javnson@zju.edu.cn)
	  * @date   : 20230605
	  */
	  //gmp_ptrdiff_t write(_OUT gmp_data_t* data, gmp_size_t length);

	/**
	 * @brief This function could pass a command.
	 * @param cmd the command.
	 * @return if the command is done corrected.
	 * @note If you need to test if command is running correctly, please send Null command.
	 *       If you get Null command print, that's the correct answer.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	virtual gmp_ptrdiff_t command(uint32_t cmd);

	/**
	 * @brief This function could pass a command.
	 * @param cmd the command.
	 * @param wparam the word parameter
	 * @param lparam the long ptr parameter
	 * @return if the command is done corrected.
	 * @note If you need to test if command is running correctly, please send Null command.
             If you get Null command print, that's the correct answer.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	virtual gmp_ptrdiff_t command(uint32_t cmd, gmp_param_t wparam, gmp_ptraddr_t lparam);

protected: // core virtual function
	/**
	 * @brief read a string of message for the device, this functions shouldn't be called by user,
	 *        This function may ignore all the constrains and just implement read function.
	 *        All the access control would be complete in read() function.
	 * @param addr the address for the device
	 * @param data the pointer for data, which would be treated as a buffer
	 * @param length the capacity of the buffer
	 * @return the real length of the data buffer
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230605
	 */
	virtual gmp_ptrdiff_t read_ex(_IN gmp_ptraddr_t addr, _OUT gmp_data_t* data, gmp_size_t length);
	/**
	 * @brief write a string of message for the device, which will call write_ex to fulfill the task.
	 * @param addr the address for the device
	 * @param data the pointer for data, which would be sent
	 * @param length the length of the data buffer
	 * @return real length that been written
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230605
	 */
	virtual gmp_ptrdiff_t write_ex(_IN gmp_ptraddr_t addr, _OUT gmp_data_t* data, gmp_size_t length);
	/**
	 * @brief This function would be called, only if read or write are refused.
	 *        When the device is locked, or the device don't support the method.
	 *		  The param for the function is to backtrack the scene.
	 * @param addr the address for the device
	 * @param data the pointer to the TX&RX buffer
	 * @param length the length of the buffer
	 * @return real length that been written
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230605
	 */
	virtual void refuse(_IN gmp_ptraddr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

public:
	/**
	 * @brief This function is to give relative position to a inner position,
	 *	      when read and write without address function are called,
	 *        the function would be called, with the parameter of the factual read or write length.
	 * @param realtive_pos the distance to be moved, related to the current position.
	 * @return null
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	 //virtual void seek_ex(_IN gmp_ptrdiff_t relative_pos);

	 // This member is default address for the device to read or write.
	 //gmp_ptraddr_t m_pos;

public:
	// pheripheral fucntion
	/**
	 * @brief Init function initialize the peripheral, maintaining state machine.
	 *        If you is using initialize code generation, for example CUBEMX, you shold using macro GMP_INIT_DEPOSIT.
	 *		  This function would only change the state machine without any substantive operations.
	 * @return if the command can't run correctly, the program would abort.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230704
	 */
	virtual void init();

	/**
	 * @brief This function reset the peripheral, maintaining state machine.
	 *        If you is using initialize code generation, for example CUBEMX, you shold using macro GMP_INIT_DEPOSIT.	 *		  This function would only change the state machine without any substantive operations.
	 *		  This function would only change the state machine without any substantive operations.
	* @return if the command can't run correctly, the program would abort.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230704
	 */
	virtual void reset();

	/**
	 * @brief Init function initialize the peripheral, maintaining state machine.
	 *        If you is using initialize code generation, for example CUBEMX, you shold using macro GMP_INIT_DEPOSIT.	 * @return if the command can't run correctly, the program would abort.
	 *		  This function would only change the state machine without any substantive operations.
	* @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230704
	 */
	virtual void shutdown();


public:
	// utility functions
	/**
	 * @brief When error occurred, call the function.
	 * @param null
	 * @return null
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	inline void error(gmp_errcode_t error_code)
	{
		this->last_err = error_code;
		this->m_state.bits.state_machine = DEVICE_STATE_ERROR;
	}

	/**
	 * @brief When warning occurred, call the function.
	 * @param null
	 * @return null
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	inline void warning(gmp_errcode_t error_code)
	{
		this->last_err = error_code;
		this->m_state.bits.warning = DEVICE_STATE_WARNING;
	}

	/**
	 * @brief When error occurred, call the function.
	 * @param null
	 * @return null
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	inline void clear_warning()
	{
		this->m_state.bits.warning = DEVICE_STATE_NORMAL;
	}

	/**
	 * @brief You may set verbose level by the function
	 * @param null
	 * @return null
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	inline void set_verbose_level(uint16_t verbose)
	{
		m_state.bits.verbose = verbose;
	}

	/**
	 * @brief You may get pnp state by the function.
	 * @param null
	 * @return null
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	inline uint32_t get_pnp_state()
	{
		return m_state.bits.pnp;
	}

	/**
	 * @brief Attach device base addr to device base address(DBA)
	 * @param null
	 * @return null
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	inline void attach(void* device_addr)
	{
		m_dba = device_addr;
	}

protected:
	// utility functions for child class

	/**
	 * @brief This function should only be called when derived class need to lock the device.
	 * @param null
	 * @return null
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	 //inline void lock()
	 //{
	 //    m_state.bits.lock = DEVICE_STATE_LOCK;
	 //}

	 /**
	  * @brief This function should only be called when derived class need to unlock the device.
	  * @param null
	  * @return null
	  * @author : Javnson(javnson@zju.edu.cn)
	  * @date   : 20230606
	  */
	  //inline void unlock()
	  //{
	  //    m_state.bits.lock = DEVICE_STATE_UNLOCK;
	  //}

	  /**
	   * @brief This function may set pnp state.
	   * @param null
	   * @return null
	   * @author : Javnson(javnson@zju.edu.cn)
	   * @date   : 20230606
	   */
	inline void set_pnp_state(uint32_t pnp_state)
	{
		m_state.bits.pnp = pnp_state;
	}

};

#pragma endregion DeviceBase

// Derived class for UART
#pragma region UART_DEVICE_DEF



class uart_device :
	public io_device_base
{
public:
	uart_device()
	{
//		this->m_uuid = UUID_IO_DEVICE_UART;
		m_state.bits.characters = DEVICE_STATE_CHAR_R + DEVICE_STATE_CHAR_W;
	}
	/**
	 * @brief Ctor with params, pass device address for the UART device.
	 * @param device_addr is the device base address.
	 * @return None
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	uart_device(void* device_addr)
	{
		this->m_dba = device_addr;
	}
	~uart_device()
	{

	}

protected:
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
	virtual gmp_ptrdiff_t read_ex(_IN gmp_ptraddr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

	/**
	 * @brief write a string of message for the device. The implement of the function is blocked.
	 * @param addr no use
	 * @param data the pointer for data, which would be sent
	 * @param length the length of the data buffer
	 * @return real length that been written
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230605
	 */
	virtual gmp_ptrdiff_t write_ex(_IN gmp_ptraddr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

public:
	/**
	 * @brief This function could pass a command.
	 * @param cmd the command.
	 * @return if the command is done corrected.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	virtual gmp_ptrdiff_t command(uint32_t cmd);

	/**
	 * @brief This function could pass a command.
	 * @param cmd the command.
	 * @param wparam the word parameter
	 * @param lparam the long ptr parameter
	 * @return if the command is done corrected.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	virtual gmp_ptrdiff_t command(uint32_t cmd, gmp_param_t wparam, gmp_ptraddr_t lparam);


protected:


};

#pragma endregion UART_DEVICE_DEF

// inter - integrated circuit(IIC) class definition
#pragma region IIC_DEVICE_DEF

class i2c_device
	:public io_device_base
{
public:
	// ctor & dtor
	i2c_device()
	{
//		m_uuid.uuid = UUID_IO_DEVICE_IIC;
	}
	~i2c_device()
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
	virtual gmp_ptrdiff_t read_ex(_IN gmp_ptraddr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

	/**
	 * @brief write a string of message for the device. The implement of the function is blocked.
	 * @param addr no use
	 * @param data the pointer for data, which would be sent
	 * @param length the length of the data buffer
	 * @return real length that been written
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230605
	 */
	virtual gmp_ptrdiff_t write_ex(_IN gmp_ptraddr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

	/**
	 * @brief This function could pass a command.
	 * @param cmd the command.
	 * @return if the command is done corrected.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	virtual gmp_ptrdiff_t command(uint32_t cmd);

	/**
	 * @brief This function could pass a command.
	 * @param cmd the command.
	 * @param wparam the word parameter
	 * @param lparam the long ptr parameter
	 * @return if the command is done corrected.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	virtual gmp_ptrdiff_t command(uint32_t cmd, gmp_param_t wparam, gmp_ptraddr_t lparam);


public:
	// service funcitons
	/**
	 * @brief This function sets baud rate of i2c bus.
	 * @param baud the target.
	 * @return the last baud rate if done correctly, 0 otherwise.
	 * @author : Javnson
	 *			 Gqc
	 * @date   : 20230714
	 */
	//virtual gmp_param_t set_baud(gmp_param_t baud);
	
	/**
	 * @brief This function sends an acknowledge bit.
	 * @param null
	 * @return GMP_OK if done correctly, GMP_ERROR for other conditions.
	 * @author : Javnson
	 *			 Gqc
	 * @date   : 20230714
	 */
	//virtual gmp_status_t ack();

	 

};

#pragma endregion IIC_DEVICE_DEF

#pragma region SPI_DEVICE_DEF

class spi_device
	:public io_device_base
{
public:
	// ctor & dtor
	spi_device()
	{

	}

	~spi_device()
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
	virtual gmp_ptrdiff_t read_ex(_IN gmp_ptraddr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

	/**
	 * @brief write a string of message for the device. The implement of the function is blocked.
	 * @param addr no use
	 * @param data the pointer for data, which would be sent
	 * @param length the length of the data buffer
	 * @return real length that been written
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230605
	 */
	virtual gmp_ptrdiff_t write_ex(_IN gmp_ptraddr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

	/**
	 * @brief This function could pass a command.
	 * @param cmd the command.
	 * @return if the command is done corrected.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	virtual gmp_ptrdiff_t command(uint32_t cmd);

	/**
	 * @brief This function could pass a command.
	 * @param cmd the command.
	 * @param wparam the word parameter
	 * @param lparam the long ptr parameter
	 * @return if the command is done corrected.
	 * @author : Javnson(javnson@zju.edu.cn)
	 * @date   : 20230606
	 */
	virtual gmp_ptrdiff_t command(uint32_t cmd, gmp_param_t wparam, gmp_ptraddr_t lparam);


};

#pragma endregion SPI_DEVICE_DEF



// utility functions
// You may disable utility functions by define GMP_DISABLE_DEVICE_UTILITY
#ifndef GMP_DISABLE_DEVICE_UTILITY


gmp_ptrdiff_t put_format_string(io_device_base* device, gmp_ptraddr_t addr, const gmp_data_t* p_fmt, ...);




#endif // end of _DISABLE_UTILITY
