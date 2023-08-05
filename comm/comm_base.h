
#include <string.h>

// basic objects
#include <./base/gmp_base.h>
#include <./base/uuid.h>

#ifndef _FILE_COMM_BASIC_H_
#define _FILE_COMM_BASIC_H_

// communication classes Universally Unique IDentifiers
//#define UUID_IO_DEVICE_BASE { 0xb18a,0xbc1c,0xf9aa,0x4634,0x944a,0x39b7,0xdcef,0xe829 }
//#define UUID_IO_DEVICE_UART { 0xb663,0x087d,0x5661,0x475c,0xb85e,0x939e,0xa806,0x0c99 }
//#define UUID_IO_DEVICE_IIC  { 0x966d,0xd336,0x2e52,0x4d4e,0xa4e8,0x7f14,0x6db9,0x23c4 }
//#define UUID_IO_DEVICE_SPI  { 0xc584,0x2a4b,0xe89b,0x428d,0x8465,0xa668,0xd85e,0x596a }
//#define UUID_IO_DEVICE_DAC  { 0xb18a,0xbc1c,0xf9aa,0x4634,0x944a,0x39b7,0xdcef,0xe829 }

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
//		uint32_t characters : 4;
//		uint32_t lock : 4;
		uint32_t roles : 4;
		//uint32_t warning : 1;
		//uint32_t verbose : 2;
		uint32_t pnp : 4;
		uint32_t rpc : 4;
	}bits;

	uint32_t all;
}gmp_device_state_t;



// gmp_device_state_t::bits::state_machine provides 4 inner states
#define DEVICE_STATE_SHUTDOWN       (0x00) // Power-Off
#define DEVICE_STATE_CONFIG         (0x01) // Preparing & config
#define DEVICE_STATE_READY          (0x02) // ready to be busy
#define DEVICE_STATE_BUSY           (0X03) // running receiving or transmitting
#define DEVICE_STATE_LOWPOWER       (0x04) // low-power mode, waiting to be weak up, and then ready
//#define DEVICE_STATE_ERROR          (0x03) // meet a fatal error, need to be re initialization
//#define DEVICE_STATE_RUNTIME_ERROR  (0x04) // meet a runtime error, should reset error reg, and then ready again


// gmp_device_state_t::bits::roles
#define DEVICE_STATE_ROLE_NULL		(0x00)
#define DEVICE_STATE_ROLE_MASTER	(0x01)
#define DEVICE_STATE_ROLE_SLAVE		(0x02)
#define DEVICE_STATE_ROLE_MGR		(0x03)
#define DEVICE_STATE_ROLE_SPECIAL   (0x04)


// gmp_device_state_t::bits::pnp
#define DEVICE_STATE_PNP_DISBALE    (0x00) // PNP method is disable
#define DEVICE_STATE_PNP_DISCONNET  (0x01)
#define DEVICE_STATE_PNP_ATTACHED   (0x02)
#define DEVICE_STATE_PNP_EJECT      (0x03)

// gmp_device_state_t::bits::rpc
#define DEVICE_STATE_RPC_DISABLE    (0x00) // disable RPC



// io_device_base::characters provides 5 inner characters.
#define DEVICE_STATE_CHAR_NULL      (0x00)
#define DEVICE_STATE_CHAR_W         (0x01) // write
#define DEVICE_STATE_CHAR_R         (0x02) // read
#define DEVICE_STATE_CHAR_C         (0x04) // config
#define DEVICE_STATE_CHAR_P         (0x08) // power mgr: init, reset, halt
#define DEVICE_STATE_CHAR_PNP       (0X10) // 

// io_device_base::lock
#define DEVICE_STATE_LOCK_DISABLE   (0x00)
#define DEVICE_STATE_LOCK           (0x01)
#define DEVICE_STATE_UNLOCK         (0x02)

typedef union _tag_dev_char_t
{
	struct
	{
		uint32_t write : 1;
		uint32_t read : 1;
		uint32_t config : 1;
		uint32_t power : 1;
		uint32_t pnp : 1;
	} bits;

	uint32_t all;
} dev_char_t;

#define DEV_CHAR_LOCKED				(1)
#define DEV_CHAR_UNLOCKED			(0)

#define DEV_CHAR_PERMIT             (1)
#define DEV_CHAR_DISABLE            (0)

// gmp_device_state_t::bits::warning
#define DEVICE_STATE_WARNING         (0x01)
#define DEVICE_STATE_NORMAL          (0x00)

// gmp_device_state_t::bits::verbose
#define DEVICE_STATE_VERBOSE_NULL    (0x00) // No print
#define DEVICE_STATE_VERBOSE_1       (0x01) // less print, mainly error
#define DEVICE_STATE_VERBOSE_ERROR   (0x01)
#define DEVICE_STATE_VERBOSE_2       (0x02) // more print, mainly warning
#define DEVICE_STATE_VERBOSE_WARNING (0x02)
#define DEVICE_STATE_VERBOSE_3       (0x03) // mostly print, mainly info
#define DEVICE_STATE_VERBOSE_INFO    (0x04)


#pragma endregion DEVICE_STATE_DEF

// Command Definition
#pragma region CommandDefinition

// This is device CMD type
typedef uint32_t gmp_device_cmd;

// Basic Command
#define DEVICE_CMD_NULL		(0x00)
#define DEVICE_CMD_RESET	(0x01) // ?(!shutdown) -> config
#define DEVICE_CMD_ENABLE   (0x02) // shutdown     -> config
#define DEVICE_CMD_INIT		(0x03) // config       -> ready 
#define DEVICE_CMD_SETUP    (0x04) // shutdown     -> ready
#define DEVICE_CMD_SHUTDOWN (0x05) // ?			   -> shutdown 
#define DEVICE_CMD_LOWPOWER (0x06) // 
#define DEVICE_CMD_LOCK     (0x07)
#define DEVICE_CMD_UNLOCK   (0x08)
#define DEVICE_CMD_VERBOSE  (0x09) // Set verbose level
#define DEVICE_CMD_REGISTER (0x0A)


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

// Error Code for device Definition
#pragma region ErrorCode
// io_device_base error code definition
#define DEVICE_OK                     (0x0000)
#define DEVICE_INFO_BEGIN          (GMP_STAT_COMM_INFO_BEGIN)
#define DEVICE_WARN_BEGIN          (GMP_STAT_COMM_WARN_BEGIN)
#define DEVICE_ERRO_BEGIN          (GMP_STAT_COMM_ERRO_BEGIN)


// unsupported operation happened
// You may change WARN and ERRO to define if the error is fatal.
#define DEVICE_ERR_UNSUPPORT_R        (DEVICE_WARN_BEGIN + 0x0010)
#define DEVICE_ERR_UNSUPPORT_W        (DEVICE_WARN_BEGIN + 0x0011)
#define DEVICE_ERR_UNSUPPORT_C        (DEVICE_WARN_BEGIN + 0x0012)
#define DEVICE_ERR_LOCKED             (DEVICE_WARN_BEGIN + 0x0013)
#define DEVICE_ERR_NOT_READY          (DEVICE_WARN_BEGIN + 0x0014)
#define DEVICE_ERR_MEMORY_UNAVAILABLE (DEVICE_ERRO_BEGIN + 0x0015)
#define DEVICE_ERR_UNKNOWN_CMD        (DEVICE_WARN_BEGIN + 0x0016)
#define DEVICE_ERR_UNKNOWN_CMDEX	  (DEVICE_WARN_BEGIN + 0x0017)
#define DEVICE_ERR_BAD_CMD			  (DEVICE_WARN_BEGIN + 0x0018)
#define DEVICE_ERR_CANNOT_INIT	      (DEVICE_ERRO_BEGIN + 0x0019)
#define DEVICE_RECOVER_FROM_ERROR     (DEVICE_INFO_BEGIN + 0x001A)
#define DEVICE_ERR_TIMEOUT			  (DEVICE_WARN_BEGIN + 0x001B)
#define DEVICE_ERR_NULL_DEV_HANDLE    (DEVICE_ERRO_BEGIN + 0x001C)
#define DEVICE_ERR_HAVE_INITED        (DEVICE_INFO_BEGIN + 0x001D)


// @brief device still in error condition. User should free from the error condition.
#define DEVICE_ERR_COND               (DEVICE_ERRO_BEGIN + 0x00FD)
#define DEVICE_ERR_ASSERT             (DEVICE_ERRO_BEGIN + 0x00FE)
#define DEVICE_ERR_HAL				  (DEVICE_WARN_BEGIN + 0x00FF)

// fatal errors definition
//#define DEIVCE_IVALID_OPER			  (GMP_STATUS_FATAL_ERROR+0x01)
#pragma endregion ErrorCode

// This section of code provide a lot of macros for user to improve programming efficiency.
#pragma region Tool_Macros
#define RESPONSE_CMD     public:                  \
		virtual gmp_stat_t cmd(uint32_t cmd); \
		virtual gmp_stat_t cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam);

#define RESPONSE_RW      protected:               \
		virtual gmp_diff_t read_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length); \
		virtual gmp_diff_t write_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

// Check if device still in error condition and think err_cond should resolve first.
#define CHECK_ERROR_COND if(erro_cond && error(DEVICE_ERR_COND))

	


#pragma endregion Tool_Macros

// General classes definition
#pragma region CMD_DEVICE
// @brief This class is the base class of all the others.
//        This class provided a set of basic virtual function, named cmd.
//        All the derive class should support cmd mode.
//        And the cmd mode is the key method for RPC.
class cmd_device
{
public:
	// ctor & dtor
	cmd_device()
	{
		m_device_usage_label = 0;
		m_last_error = DEVICE_OK;
//		m_error_cnt = 0;
//		m_warn_cnt = 0;
		erro_cond = 0;
		warn_cond = 0;
		verbose = DEVICE_STATE_VERBOSE_1;
	}
	~cmd_device()
	{

	}

protected:
	// Unique identifier
	// This field tells user the usage of the device(class).
	uint32_t m_device_usage_label;

public:
	// error control
	uint32_t m_last_error;

	// the counter of error
	// update: replace with global variables `g_warn_cnt`
//	gmp_fast_t m_error_cnt;

	// the counter of warning
	// update: replace with global variables `g_warn_cnt`
//	gmp_fast_t m_warn_cnt;

protected:
	// 0 no error happen, the device is running normally.
	// 1 error happened, you should recover from error first.
	uint8_t erro_cond : 1;
	
	// 0 no warning happen, 1 warning is triggered and you may choose to ignore.
	uint8_t warn_cond : 1;

	// Set the verbose level of the device.
	// the higher value, the more verbose.
	uint8_t verbose : 2;

public:
	// kernel virtual function
	/**
	 * @brief This function could execute a command.
	 * @param cmd the command.
	 * @return if the command is done corrected.
	 * @note If you need to test if command is running correctly, please send Null command.
	 *       If you get Null command print, that's the correct answer.
	 * @author : Javnson
	 * @date   : 20230606
	 */
	virtual gmp_stat_t cmd(uint32_t cmd) = 0;

	/**
	 * @brief This function could execute a command.
	 * @param cmd the command.
	 * @param wparam the word parameter
	 * @param lparam the long ptr parameter
	 * @return if the command is done corrected.
	 * @note If you need to test if command is running correctly, please send Null command.
			 If you get Null command print, that's the correct answer.
	 * @author : Javnson
	 * @date   : 20230606
	 */
	virtual gmp_stat_t cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam) = 0;

public:
	// utilities
	/**
	 * @brief This function may get device_usage_label.
	 * @param null
	 * @return device_usage_label
	 * @author : Javnson
	 * @date   : 20230716
	 */
	uint32_t get_device_usage_label()
	{
		return m_device_usage_label;
	}

	/**
	 * @brief This function set error counter to 0.
	 *        Meanwhile this function set warn_cond to 0.
	 * @param null.
	 * @return null.
	 * @author : Javnson
	 * @date   : 20230716
	 */

//	void clear_error_cnt();


	/**
	 * @brief You may set verbose level by the function
	 * @param null
	 * @return null
	 * @author : Javnson
	 * @date   : 20230606
	 */
	void set_verbose(uint8_t verbose);
	
	/**
	 * @brief You may get verbose level by the function
	 * @param null
	 * @return null
	 * @author : Javnson, Gqc
	 * @date   : 20230719
	 * @edition: [Gqc] return value type: void --> uint8_t
	 */
	inline uint8_t get_verbose()
	{
		return this->verbose;
	}

protected:
	/**
	 * @brief This function trigger a warning or error.
	 * @param error code
	 * @return if the function should be terminated right now the function return 1, or the function returns 0.
	 * @note This function will judge the errcode type is fatal or just warning.
	 * @author : Javnson
	 * @date   : 20230716
	 */
	gmp_fast_t error(uint32_t errcode);

	/**
	 * @brief This function may release the error state.
	 * @param null.
	 * @return null
	 * @note DANGEROUS! This function should only be called when error disappeared.
	 * @author : Javnson
	 * @date   : 20230716
	 */
	void clear_erro();
	
public:
	/**
	 * @brief This function may release the warning state.
	 * @param null.
	 * @return null
	 * @note User have no necessary to call this function. 
	 * @author : Javnson
	 * @date   : 20230716
	 */
	void clear_warn()
	{
		warn_cond = 0;
	}


};


#pragma endregion CMD_DEVICE


// Io device base, memory space
#pragma region DeviceBase
class io_device_base
	:public cmd_device
{
protected:
	// protected members
	// @brief device base address
	void* m_dev;

	// @brief device state machine
	gmp_device_state_t m_state;

	// @brief This variable describe the character of the device.
	// 0 is not permit, 1 is permit
	dev_char_t character;

	// @brief This variable describe the lock of the characters.
	// 0 is not lock, 1 is locked.
	dev_char_t lock;

public:
	// ctor & dtor
	io_device_base()
		:m_dev(nullptr)
	{
//		m_dev = NULL;

		m_state.all = 0;
		//m_state.bits.state_machine = DEVICE_STATE_SHUTDOWN;
		//m_state.bits.roles = DEVICE_STATE_ROLE_NULL;
		//m_state.bits.pnp = DEVICE_STATE_PNP_DISBALE;
		//m_state.bits.rpc = DEVICE_STATE_RPC_DISABLE;

		character.all = DEVICE_STATE_CHAR_W | DEVICE_STATE_CHAR_R;
//		character = DEVICE_STATE_CHAR_NULL;

		lock.all = DEVICE_STATE_CHAR_NULL;
	}

	io_device_base(void* dev_addr)
		:m_dev(dev_addr)
	{
//		m_dev = dba;

		m_state.all = 0;
		//m_state.bits.state_machine = DEVICE_STATE_SHUTDOWN;
		//m_state.bits.roles = DEVICE_STATE_ROLE_NULL;
		//m_state.bits.pnp = DEVICE_STATE_PNP_DISBALE;
		//m_state.bits.rpc = DEVICE_STATE_RPC_DISABLE;

		character.all = DEVICE_STATE_CHAR_W | DEVICE_STATE_CHAR_R;
		lock.all = DEVICE_STATE_CHAR_NULL;

	}

	~io_device_base()
	{
		// This variable is treated as a symbol of whether this module is running.
		m_dev = nullptr;

	}

public: // Core functions
	/**
	 * @brief read a string of message from the device, which will call read_ex to fulfill the task.
	 * @param addr the address for the device
	 * @param data the pointer for data, which are treated as a read buffer
	 * @param length the length of the data buffer
	 * @return real length that been read
	 * @author : Javnson
	 * @date   : 20230605
	 */
	gmp_diff_t read(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);
	/**
	 * @brief write a string of message for the device, which will call write_ex to fulfill the task.
	 * @param addr the address for the device
	 * @param data the pointer for  data, which would be sent
	 * @param length the length of the data buffer
	 * @return real length that been written
	 * @author : Javnson
	 * @date   : 20230605
	 */
	gmp_diff_t write(_IN gmp_addr_t addr, _IN gmp_data_t* data, gmp_size_t length);

	/**
	 * @brief read a string of message from the device, which will call read_ex to fulfill the task.
	 *        based on the position of m_pos, you may change the position by seek.
	 * @param data the pointer for data, which are treated as a read buffer
	 * @param length the length of the data buffer
	 * @return real length that been read
	 * @author : Javnson
	 * @date   : 20230605
	 */
	 //gmp_diff_t read(_OUT gmp_data_t* data, gmp_size_t length);
	 /**
	  * @brief write a string of message for the device, which will call write_ex to fulfill the task.
	  *        based on the position of m_pos, you may change the position by seek.
	  * @param addr the address for the device
	  * @param data the pointer for data, which would be sent
	  * @param length the length of the data buffer
	  * @return real length that been written
	  * @author : Javnson
	  * @date   : 20230605
	  */
	  //gmp_diff_t write(_OUT gmp_data_t* data, gmp_size_t length);


	// This class will implement the cmd function.
	RESPONSE_CMD

	// This class will implement the R/W functions
//	RESPONSE_RW


protected: // core virtual function
	/**
	 * @brief read a string of message for the device, this functions shouldn't be called by user,
	 *        This function may ignore all the constrains and just implement read function.
	 *        All the access control would be complete in read() function.
	 * @param addr the address for the device
	 * @param data the pointer for data, which would be treated as a buffer
	 * @param length the capacity of the buffer
	 * @return the real length of the data buffer
	 * @author : Javnson
	 * @date   : 20230605
	 */
	virtual gmp_diff_t read_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);
	/**
	 * @brief write a string of message for the device, which will call write_ex to fulfill the task.
	 * @param addr the address for the device
	 * @param data the pointer for data, which would be sent
	 * @param length the length of the data buffer
	 * @return real length that been written
	 * @author : Javnson
	 * @date   : 20230605
	 */
	virtual gmp_diff_t write_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

	/**
	 * @brief This function would be called, only if read or write are refused.
	 *        When the device is locked, or the device don't support the method.
	 *		  The param for the function is to backtrack the scene.
	 * @param addr the address for the device
	 * @param data the pointer to the TX&RX buffer
	 * @param length the length of the buffer
	 * @return real length that been written
	 * @author : Javnson
	 * @date   : 20230605
	 */
	virtual void refuse(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

public:
	/**
	 * @brief This function is to give relative position to a inner position,
	 *	      when read and write without address function are called,
	 *        the function would be called, with the parameter of the factual read or write length.
	 * @param realtive_pos the distance to be moved, related to the current position.
	 * @return null
	 * @author : Javnson
	 * @date   : 20230606
	 */
	 //virtual void seek_ex(_IN gmp_diff_t relative_pos);

	 // This member is default address for the device to read or write.
	 //gmp_addr_t m_pos;

public:

	// peripheral function,
	// All the following functions would be implemented in cmd function.

	/**
	 * @brief Init function initialize the peripheral, maintaining state machine.
	 *        If you is using initialize code generation, for example CUBEMX, you should using macro GMP_INIT_DEPOSIT.
	 *		  This function would only change the state machine without any substantive operations.
	 *        state machine DEVICE_STATE_SHUTDOWN -> DEVICE_STATE_READY
	 * @return if the command can't run correctly, the program would abort.
	 * @author : Javnson
	 * @date   : 20230704
	 */
	gmp_stat_t init();

	/**
	 * @brief Preinit function initialize the peripheral, but still waiting user to change some params.
	 *        state machine DEVICE_STATE_SHUTDOWN -> DEVICE_STATE_CONFIG
	 * @param null
	 * @return if the command is done correctly.
	 * @author : Javnson
	 * @date   : 20230718
	 */
	gmp_stat_t preinit();

	/**
	 * @brief Ready function check if device fulfill the requirement, and maintaining state machine.
	 *        state machine DEVICE_STATE_CONFIG -> DEVICE_STATE_READY
	 * @param null
	 * @return if the command is done correctly.
	 * @author : Javnson
	 * @date   : 20230718
	 */
	gmp_stat_t ready();

	/**
	 * @brief This function reset the peripheral, maintaining state machine.
	 *        If you is using initialize code generation, for example CUBEMX, you should using macro GMP_INIT_DEPOSIT.
	 *		  And under this condition, this function would only change the state machine without any substantive operations.
	 *        State machine DEVICE_STATE_? -> DEVICE_STATE_READY
	 * @return if the command can't run correctly, the program would abort.
	 * @author : Javnson
	 * @date   : 20230704
	 */
	gmp_stat_t reset();

	/**
	 * @brief Init function initialize the peripheral, maintaining state machine.
	 *        If you is using initialize code generation, for example CUBEMX, you should using macro GMP_INIT_DEPOSIT.
	 * @return if the command can't run correctly, the program would abort.
	 *		  This function would only change the state machine without any substantive operations.
	 * @author : Javnson
	 * @date   : 20230704
	 */
	gmp_stat_t shutdown();


public:
	// utility functions
	/**
	 * @brief When error occurred, call the function.
	 * @param null
	 * @return null
	 * @author : Javnson
	 * @date   : 20230606
	 */
	//inline void error(gmp_errcode_t error_code)
	//{
	//	this->last_err = error_code;
	//	this->m_state.bits.state_machine = DEVICE_STATE_ERROR;
	//}

	/**
	 * @brief When warning occurred, call the function.
	 * @param null
	 * @return null
	 * @author : Javnson
	 * @date   : 20230606
	 */
	//inline void warning(gmp_errcode_t error_code)
	//{
	//	this->last_err = error_code;
	//	this->m_state.bits.warning = DEVICE_STATE_WARNING;
	//}

	/**
	 * @brief When error occurred, call the function.
	 * @param null
	 * @return null
	 * @author : Javnson
	 * @date   : 20230606
	 */
	//inline void clear_warning()
	//{
	//	this->m_state.bits.warning = DEVICE_STATE_NORMAL;
	//}

	/**
	 * @brief You may set verbose level by the function
	 * @param null
	 * @return null
	 * @author : Javnson
	 * @date   : 20230606
	 */
	//inline void set_verbose_level(uint16_t verbose)
	//{
	//	m_state.bits.verbose = verbose;
	//}

	/**
	 * @brief You may get pnp state by the function.
	 * @param null
	 * @return null
	 * @author : Javnson
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
	 * @author : Javnson
	 * @date   : 20230606
	 */
	inline void attach(void* device_addr)
	{
		m_dev = device_addr;
	}

protected:
	// utility functions for child class

	/**
	 * @brief This function should only be called when derived class need to lock the device.
	 * @param null
	 * @return null
	 * @author : Javnson
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
	  * @author : Javnson
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
	   * @author : Javnson
	   * @date   : 20230606
	   */
	inline void set_pnp_state(uint32_t pnp_state)
	{
		m_state.bits.pnp = pnp_state;
	}


public:

	// @return 1 : readable
	//		   0 : not readable
	inline gmp_fast_t is_readable()
	{
		// Condition 0: If any fatal error haven't been clear
		CHECK_ERROR_COND
			return  0;

		// Condition 1: read the device is permitted.
		if (character.bits.read == DEV_CHAR_DISABLE) // read operation is not permitted.	
		{
			refuse(addr, data, length);
			if (error(DEVICE_ERR_UNSUPPORT_R))
				return 0;
		}

		// Condition 2: Device is locked
		if (lock.bits.read != DEV_CHAR_LOCKED)
		{
			refuse(addr, data, length);
			if (error(DEVICE_ERR_LOCKED))
				return 0;
		}

		// Condition 3: Device is not yet ready
		if (m_state.bits.state_machine != DEVICE_STATE_READY)
		{
			refuse(addr, data, length);
			if (error(DEVICE_ERR_NOT_READY))
				return  0;
		}

		return 1;
	}

	// @return 1 : writeable
	//		   0 : not writeable
	inline gmp_fast_t is_writeable()
	{
		// Condition 0: If any fatal error haven't been clear
		CHECK_ERROR_COND
			return  0;

		// Condition 1: write the device is permitted.
		if (character.bits.write == 0) // write oper is not permitted.	
		{
			refuse(addr, data, length);
			if (error(DEVICE_ERR_UNSUPPORT_W))
				return 0;
		}

		// Condition 2: Device is locked
		if (lock.bits.write != 0)
		{
			refuse(addr, data, length);
			if (error(DEVICE_ERR_LOCKED))
				return 0;
		}

		// Condition 3: Device is not yet ready
		if (m_state.bits.state_machine != DEVICE_STATE_READY)
		{
			refuse(addr, data, length);
			if (error(DEVICE_ERR_NOT_READY))
				return 0;
		}
		return 1;
	}
};


#pragma endregion IoDeviceBase

// Universal Asynchronous Receiver/Transmitter (UART)
#pragma region UART_DEVICE_DEF

class uart_device :
	public io_device_base
{
public:
	uart_device()
	{
		//		this->m_uuid = UUID_IO_DEVICE_UART;
		character.all = DEVICE_STATE_CHAR_R | DEVICE_STATE_CHAR_W;

	}
	/**
	 * @brief Ctor with params, pass device address for the UART device.
	 * @param device_addr is the device base address.
	 * @return None
	 * @author : Javnson
	 * @date   : 20230606
	 */
	uart_device(void* device_addr)
	{
		character.all = DEVICE_STATE_CHAR_R | DEVICE_STATE_CHAR_W;
		this->m_dev = device_addr;
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
	 * @author : Javnson
	 * @date   : 20230605
	 */
//	virtual gmp_diff_t read_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

	/**
	 * @brief write a string of message for the device. The implement of the function is blocked.
	 * @param addr no use
	 * @param data the pointer for data, which would be sent
	 * @param length the length of the data buffer
	 * @return real length that been written
	 * @author : Javnson
	 * @date   : 20230605
	 */
//	virtual gmp_diff_t write_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

	// This class will implement the R/W functions.
	RESPONSE_RW
	
	// This class will implement the cmd function.
	RESPONSE_CMD


public:


protected:


};

#pragma endregion UART_DEVICE_DEF

// inter - integrated circuit(IIC)
#pragma region IIC_DEVICE_DEF

class i2c_device
	:public io_device_base
{
public:
	// ctor & dtor
	i2c_device()
	{
		//		m_uuid.uuid = UUID_IO_DEVICE_IIC;
		character.all = DEVICE_STATE_CHAR_R | DEVICE_STATE_CHAR_W;

	}
	i2c_device(void* device_addr)
	{
		character.all = DEVICE_STATE_CHAR_R | DEVICE_STATE_CHAR_W;

		m_dev = device_addr;

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
	 * @author : Javnson
	 * @date   : 20230605
	 */
//	virtual gmp_diff_t read_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);


	/**
	 * @brief write a string of message for the device. The implement of the function is blocked.
	 * @param addr no use
	 * @param data the pointer for data, which would be sent
	 * @param length the length of the data buffer
	 * @return real length that been written
	 * @author : Javnson
	 * @date   : 20230605
	 */
//	virtual gmp_diff_t write_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

	// This class will implement the R/W functions.
	RESPONSE_RW

	// This class will implement the cmd function.
	RESPONSE_CMD



public:
	// service functions
	/**
	 * @brief This function sets baud rate of i2c bus.
	 * @param baud the target.
	 * @return the last baud rate if done correctly, 0 otherwise.
	 * @author : Gqc
	 * @date   : 20230714
	 */
	 //virtual gmp_param_t set_baud(gmp_param_t baud);

	 /**
	  * @brief This function sends an acknowledge bit.
	  * @param null
	  * @return GMP_OK if done correctly, GMP_ERROR for other conditions.
	  * @author : Gqc
	  * @date   : 20230714
	  */
	  //virtual gmp_stat_t ack();



};

#pragma endregion IIC_DEVICE_DEF

// Serial Peripheral Interface(SPI)
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
	 * @author : Javnson
	 * @date   : 20230605
	 */
	//virtual gmp_diff_t read_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);


	/**
	 * @brief write a string of message for the device. The implement of the function is blocked.
	 * @param addr no use
	 * @param data the pointer for data, which would be sent
	 * @param length the length of the data buffer
	 * @return real length that been written
	 * @author : Javnson
	 * @date   : 20230605
	 */
	//virtual gmp_diff_t write_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

	// This class will implement the R/W functions.
	RESPONSE_RW

	// This class will implement the cmd function.
	RESPONSE_CMD

};

#pragma endregion SPI_DEVICE_DEF

// General-Purpose Input/Output(GPIO)
// NOTE: This class hasn't been implement correctly.
#pragma region GPIO_DEVICE_DEF


// @brief This class abstract the whole GPIO to a device.
// This device may exist only one instance. The GMP library use this class to implement PRC of GPIO.
class gpio_dev
	: public cmd_device
{
public:
	// ctor & dtor
	gpio_dev()
	{}

	~gpio_dev()
	{}

public:
	// Kernel function
	/**
	 * @brief This function set a GPIO to high level.
	 * @param port_group the gpio group
	 * @param gpio_index the gpio index
	 * @return if gpio is set or clear.
	 * @author : Javnson
	 * @date   : 20230723
	 */
	static gmp_stat_t set(gmp_addr_t port_group, gmp_size_t gpio_index);

	/**
	 * @brief This function set a GPIO to low level.
	 * @param port_group the gpio group
	 * @param gpio_index the gpio index
	 * @return if gpio is set or clear.
	 * @author : Javnson
	 * @date   : 20230723
	 */
	static gmp_stat_t reset(gmp_addr_t port_group, gmp_size_t gpio_index);

	/**
	 * @brief This function change the level of a GPIO.
	 * @param port_group the gpio group
	 * @param gpio_index the gpio index
	 * @return if gpio is set or clear.
	 * @author : Javnson
	 * @date   : 20230723
	 */
	static gmp_stat_t toggle(gmp_addr_t port_group, gmp_size_t gpio_index);

	/**
	 * @brief This function read a specified GPIO port.
	 * @param port_group the gpio group
	 * @param gpio_index the gpio index
	 * @return 0 the GPIO has low level, 1 the GPIO has high level,
	 *	       -1 the read method invalid.
	 * @author : Javnson
	 * @date   : 20230723
	 */
	static int8_t read(gmp_addr_t port_group, gmp_size_t gpio_index);

public:
	// This class will implement the cmd function.
	RESPONSE_CMD

};


// @brief This class implement a suspended GPIO handle. User may use this handle easily.
class gpio_port
{
public:
	// ctor & dtor
	gpio_port(gmp_addr_t gpio_group, gmp_size_t index)
		:port_group(gpio_group), gpio_index(index)
	{}

	gpio_port()
	{

	}

	~gpio_port()
	{
	}

public:
	// members GPIO index

	// @brief The group index of the port
	gmp_addr_t port_group;

	// @brief the GPIO index of the group.
	gmp_size_t gpio_index;


public:
	// kernel functions
	/**
	 * @brief This function set a GPIO to high level.
	 * @return if gpio is set or clear.
	 * @note This function will call ::set(), so user should implement the function in BSP file.
	 * @author : Javnson
	 * @date   : 20230723
	 */
	gmp_stat_t set()
	{
		return gpio_dev::set(port_group, gpio_index);
	}

	/**
	 * @brief This function set a GPIO to low level.
	 * @return if gpio is set or clear.
	 * @author : Javnson
	 * @date   : 20230723
	 */
	gmp_stat_t reset()
	{
		return gpio_dev::reset(port_group, gpio_index);
	}

	/**
	 * @brief This function change the level of a GPIO.
	 * @return if gpio is set or clear.
	 * @author : Javnson
	 * @date   : 20230723
	 */
	gmp_stat_t toggle()
	{
		return gpio_dev::toggle(port_group, gpio_index);
	}

	/**
	 * @brief This function read a specified GPIO port.
	 * @return 0 the GPIO has low level, 1 the GPIO has high level,
	 *	       -1 the read method invalid.
	 * @author : Javnson
	 * @date   : 20230723
	 */
	int8_t read()
	{
		return gpio_dev::read(port_group, gpio_index);
	}


};



#pragma endregion GPIO_DEVICE_DEF

// utility functions
// You may disable utility functions by define GMP_DISABLE_DEVICE_UTILITY
#ifndef GMP_DISABLE_DEVICE_UTILITY


gmp_diff_t put_format_string(io_device_base* device, gmp_addr_t addr, const gmp_data_t* p_fmt, ...);




#endif // end of _DISABLE_UTILITY

#endif // _FILE_COMM_BASIC_H_
