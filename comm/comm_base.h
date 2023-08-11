
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


// library settings
#if defined GMPDEV_DISABLE_LOCK

#endif // GMP_DEV_DISABLE_LOCK



// Command Definition
#pragma region CommandDefinition

// This is device CMD type
typedef uint32_t gmp_device_cmd;

// Basic Command
#define DEVICE_CMD_NULL		          (0x00)
								      
// State machine transfer command     
#define GMPDEV_CMD_RESET			  (0x01) // ?(!shutdown) -> config
#define GMPDEV_CMD_ENABLE		      (0x02) // shutdown     -> config
#define GMPDEV_CMD_SUSPEND            (0x03)
#define GMPDEV_CMD_READY		      (0x04) // shutdown     -> ready
#define GMPDEV_CMD_SHUTDOWN		      (0x05) // ?			   -> shutdown 
#define GMPDEV_CMD_STANDBY		      (0x06) // 
								      
								      
#define GMPDEV_CMD_LOCK			      (0x07)
#define GMPDEV_CMD_UNLOCK		      (0x08)
#define GMPDEV_CMD_VERBOSE		      (0x09) // Set verbose level
#define GMPDEV_CMD_REGISTER		      (0x0A)


// PNP Command
#define GMPDEV_CMD_PNP_TEST		      (0x10) // Test a PnP device if it is connected to the master
#define GMPDEV_CMD_PNP_EJECT	      (0x11) // Notify the device will be remove.


// Device General Config
#define GMPDEV_CMD_GET_BAUD		      (0x101)
#define GMPDEV_CMD_SET_BAUD		      (0x102)
#define GMPDEV_CMD_GET_UUID		      (0x103)
#define GMPDEV_CMD_SET_UUID		      (0x104)

// UART device COMMAND define


// IIC device COMMAND define
#define GMPDEV_CMD_IIC_SET_MODE	      (0x301)
#define GMPDEV_CMD_IIC_ACK		      (0x302)

// SPI device COMMAND define


// Device User Command Config
#define GMPDEV_CMD_USER_BEGIN	      (0x8000)

#pragma endregion CommandDefinition

// Error Code for device Definition
#pragma region ErrorCode
// io_device_base error code definition
#define GMPDEV_OK					    (0x0000)
#define DEV_INFO_BEGIN			        (GMP_STAT_COMM_INFO_BEGIN)
#define DEV_WARN_BEGIN			        (GMP_STAT_COMM_WARN_BEGIN)
#define DEV_ERRO_BEGIN			        (GMP_STAT_COMM_ERRO_BEGIN)


// unsupported operation happened
// You may change WARN and ERRO to define if the error is fatal.
#define GMPDEV_ERR_UNSUPPORT_R          (DEV_WARN_BEGIN + 0x0010)
#define GMPDEV_ERR_UNSUPPORT_W          (DEV_WARN_BEGIN + 0x0011)
#define GMPDEV_ERR_UNSUPPORT_C          (DEV_WARN_BEGIN + 0x0012)
#define GMPDEV_ERR_LOCKED               (DEV_WARN_BEGIN + 0x0013)
#define GMPDEV_ERR_NOT_READY            (DEV_WARN_BEGIN + 0x0014)
#define GMPDEV_ERR_MEMORY_UNAVAILABLE   (DEV_ERRO_BEGIN + 0x0015)
#define GMPDEV_ERR_UNKNOWN_CMD          (DEV_WARN_BEGIN + 0x0016)
#define GMPDEV_ERR_UNKNOWN_CMDEX	    (DEV_WARN_BEGIN + 0x0017)
#define GMPDEV_ERR_BAD_CMD			    (DEV_WARN_BEGIN + 0x0018)
#define GMPDEV_ERR_CANNOT_READY	        (DEV_ERRO_BEGIN + 0x0019)
#define GMPDEV_RECOVER_FROM_ERROR       (DEV_INFO_BEGIN + 0x001A)
#define GMPDEV_ERR_TIMEOUT			    (DEV_WARN_BEGIN + 0x001B)
#define GMPDEV_ERR_NULL_DEV_HANDLE      (DEV_ERRO_BEGIN + 0x001C)
#define GMPDEV_ERR_HAVE_INITED          (DEV_INFO_BEGIN + 0x001D)
#define GMPDEV_ERR_BUSY                 (DEV_WARN_BEGIN + 0x001E)
#define GMPDEV_ERR_STATE_NOT_CHG        (DEV_INFO_BEGIN + 0x001F)
#define GMPDEV_ERR_CANNOT_SHUTDOWN      (DEV_WARN_BEGIN + 0x0020)
#define GMPDEV_ERR_CANNOT_ENABLE        (DEV_WARN_BEGIN + 0x0021)
#define GMPDEV_ERR_CANNOT_STANDBY       (DEV_WARN_BEGIN + 0x0022)
#define GMPDEV_ERR_UNKNOWN_STAT_TRANS   (DEV_WARN_BEGIN + 0x0023)
#define GMPDEV_ERR_UNSUPPORT_LOCK       (DEV_WARN_BEGIN + 0x0024)
#define GMPDEV_ERR_STILL_ERRO_COND      (DEV_WARN_BEGIN + 0x0025)
#define GMPDEV_ERR_CANNOT_SUSPEND       (DEV_WARN_BEGIN + 0x0026)

// @brief device still in error condition. User should free from the error condition.
#define GMPDEV_ERR_COND                 (DEV_ERRO_BEGIN + 0x00FD)
#define GMPDEV_ERR_ASSERT               (DEV_ERRO_BEGIN + 0x00FE)
#define GMPDEV_ERR_HAL				    (DEV_WARN_BEGIN + 0x00FF)

#pragma endregion ErrorCode

// This section of code provide a lot of macros for user to improve programming efficiency.
#pragma region Tool_Macros
#define RESPONSE_CMD     public:                  \
		virtual gmp_stat_t cmd(uint32_t cmd); \
		virtual gmp_stat_t cmd(uint32_t cmd, gmp_param_t wparam, gmp_addr_t lparam);

#define RESPONSE_RW      protected:               \
		virtual gmp_diff_t read_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t capacity, gmp_size_t length);\
		virtual gmp_diff_t write_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

// Check if device still in error condition and think err_cond should resolve first.
#define CHECK_ERROR_COND if(erro_cond && error(DEVICE_ERR_COND))

	


#pragma endregion Tool_Macros

// General classes definition
#pragma region CMD_DEVICE


// cmd_device::bits::warn_cond
// cmd_device::bits::erro_cond
#define GMPDEV_STAT_WARN               (0x01)
#define GMPDEV_STAT_ERRO               (0x01)
#define GMPDEV_STAT_NORM               (0x00)

// cmd_device::bits::verbose
#define GMPDEV_STAT_VERBOSE_NULL       (0x00) // No print
#define GMPDEV_STAT_VERBOSE_1          (0x01) // less print, mainly error
#define GMPDEV_STAT_VERBOSE_ERROR      (0x01)
#define GMPDEV_STAT_VERBOSE_2          (0x02) // more print, mainly warning
#define GMPDEV_STAT_VERBOSE_WARN       (0x02)
#define GMPDEV_STAT_VERBOSE_3          (0x03) // mostly print, mainly info
#define GMPDEV_STAT_VERBOSE_INFO       (0x04)


// @brief This class is the base class of all the others.
//        This class provided a set of basic virtual function, named cmd.
//        All the derive class should support cmd mode.
//        And the cmd mode is the key method for RPC.
class cmd_device
{
public:
	// ctor & dtor
	cmd_device()
		:m_dev_unique_id(0),
		m_last_error(GMPDEV_OK),
		erro_cond(GMPDEV_STAT_NORM),warn_cond(GMPDEV_STAT_NORM),
		verbose(GMPDEV_STAT_VERBOSE_ERROR)
	{

	}
	~cmd_device()
	{

	}

private:
	// Unique identifier
	// This field tells user the usage of the device(class).
	// This variable shouldn't be change
	uint32_t m_dev_unique_id;

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

	/**
	 * @brief This function are called when any class state changes.
	 * @return null
	 * @author : Javnson
	 * @date   : 20230810
	 */
//	virtual gmp_stat_t callback()

public:
	// utilities
	/**
	 * @brief This function may get m_dev_unique_id.
	 * @param null
	 * @return m_dev_unique_id
	 * @author : Javnson
	 * @date   : 20230716
	 * @last update : 2023.08.11
	 */
	uint32_t get_device_unique_id()
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

// Device State definition

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
		uint32_t roles : 4;
		uint32_t pnp : 4;
		uint32_t rpc : 4;
	}bits;

	uint32_t all;
}gmp_device_state_t;



// io_device_base::gmp_device_state_t::bits::state_machine 
// provides 5 inner states
#define GMPDEV_STAT_SHUTDOWN        (0x00) // Power-Off
#define GMPDEV_STAT_CONFIG          (0x01) // Preparing & config
#define GMPDEV_STAT_READY           (0x02) // ready to be busy
#define GMPDEV_STAT_BUSY            (0X03) // running receiving or transmitting
#define GMPDEV_STAT_LOWPOWER        (0x04) // low-power mode, waiting to be weak up, and then ready


// io_device_base::gmp_device_state_t::bits::roles
#define GMPDEV_STAT_ROLE_NULL		(0x00)
#define GMPDEV_STAT_ROLE_MASTER	    (0x01)
#define GMPDEV_STAT_ROLE_SLAVE		(0x02)
#define GMPDEV_STAT_ROLE_MGR		(0x03)
#define GMPDEV_STAT_ROLE_SPECIAL    (0x04)


// io_device_base::gmp_device_state_t::bits::pnp
#define GMPDEV_STAT_PNP_DISBALE     (0x00) // PNP method is disable
#define GMPDEV_STAT_PNP_DISCONNET   (0x01)
#define GMPDEV_STAT_PNP_ATTACHED    (0x02)
#define GMPDEV_STAT_PNP_EJECT       (0x03)

// io_device_base::gmp_device_state_t::bits::rpc
#define GMPDEV_STAT_RPC_DISABLE    (0x00) // disable RPC

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



// io_device_base::characters provides 5 inner characters.
#define GMPDEV_STAT_CHAR_NULL      (0x00)
#define GMPDEV_STAT_CHAR_W         (0x01) // write
#define GMPDEV_STAT_CHAR_R         (0x02) // read
#define GMPDEV_STAT_CHAR_C         (0x04) // config
#define GMPDEV_STAT_CHAR_P         (0x08) // power mgr: init, reset, halt
#define GMPDEV_STAT_CHAR_PNP       (0X10)

// io_device_base::lock
#define GMPDEV_CHAR_LOCKED		   (1)
#define GMPDEV_CHAR_UNLOCKED	   (0)

#define GMPDEV_CHAR_PERMIT         (1)
#define GMPDEV_CHAR_DISABLE        (0)



class io_device_base
	:public cmd_device
{
protected:
	// protected members
	// @brief device base address
	void* m_dev;

	// @brief device state machine
	gmp_GMPDEV_STAT_t m_state;

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
		character.all = GMPDEV_STAT_CHAR_W | GMPDEV_STAT_CHAR_R;
#if defined GMP_DEV_DISABLE_LOCK
		lock.all = GMPDEV_STAT_CHAR_NULL;
#endif
	}

	io_device_base(void* dev_addr)
		:m_dev(dev_addr)
	{
		character.all = GMPDEV_STAT_CHAR_W | GMPDEV_STAT_CHAR_R;
#if defined GMP_DEV_DISABLE_LOCK
		lock.all = GMPDEV_STAT_CHAR_NULL;
#endif

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
	 * @param capacity the capacity of the data pointer
	 * @return real length that been read
	 * @author : Javnson
	 * @date   : 20230605
	 */
	gmp_diff_t read(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t capacity);

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
	 * @param addr the address for the device
	 * @param data the pointer for data, which are treated as a read buffer
	 * @param capacity the capacity of the data pointer
	 * @param length the length of the data buffer for sending
	 * @note  at the time (length == 0), the functionality of `read_write` function is equivalent to `read`.
	 *        `read_write` need both W and R character, but `read` only R character.
	 * @return real length that been read
	 * @author : Javnson
	 * @date   : 20230805
	 */
	gmp_diff_t read_write(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t capacity, gmp_size_t length = 0);

	/**
	 * Old function prototype backup. 
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
	  * Old function prototype backup. 
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
	virtual gmp_diff_t read_ex(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t capacity, gmp_size_t length);

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
//	virtual void refuse(_IN gmp_addr_t addr, _OUT gmp_data_t* data, gmp_size_t length);

//public:
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
	 *        state machine GMPDEV_STAT_SHUTDOWN -> GMPDEV_STAT_READY
	 * @return if the command can't run correctly, the program would abort.
	 * @author : Javnson
	 * @date   : 20230704
	 * @update : 20230811
	 */
	gmp_stat_t init();

	/**
	 * @brief enable function would send a ENABLE command to change state machine.
	 *        state machine GMPDEV_STAT_? -> GMPDEV_STAT_CONFIG
	 * @param null
	 * @return if the command is done correctly.
	 * @author : Javnson
	 * @date   : 20230718
	 * @update : 20230811
	 */
	gmp_stat_t enable();

	/**
	 * @brief Ready function would send a READY command to change state machine.
	 *        state machine GMPDEV_STAT_? -> GMPDEV_STAT_READY
	 * @param null
	 * @return if the command is done correctly.
	 * @author : Javnson
	 * @date   : 20230718
	 * @update : 20230811
	 */
	gmp_stat_t ready();

	/**
	 * @brief This function reset the peripheral, maintaining state machine.
	 *        State machine GMPDEV_STAT_? -> GMPDEV_STAT_CONFIG
	 * @return if the command is done correctly.
	 * @author : Javnson
	 * @date   : 20230704
	 * @update : 20230811
	 */
	gmp_stat_t reset();

	/**
	 * @brief Shutdown function would send a SHUTDOWN command to change state machine.
	 * @return if the command is done correctly.
	 * @author : Javnson
	 * @date   : 20230704
	 * @update : 20230811
	 */
	gmp_stat_t shutdown();

	/**
	 * @brief Suspend function would send a SUSPEND command to change state machine.
	 * @return if the command is done correctly.
	 * @author : Javnson
	 * @date   : 20230811
	 */
	gmp_stat_t suspend();


public:
	// utility functions
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
	// utility functions for derived class

	/**
	 * @brief This function should only be called when derived class need to lock the device.
	 * @param null
	 * @return null
	 * @author : Javnson
	 * @date   : 20230606
	 */
	 //inline void lock()
	 //{
	 //    m_state.bits.lock = GMPDEV_STAT_LOCK;
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
	  //    m_state.bits.lock = GMPDEV_STAT_UNLOCK;
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

	/**
	 * @brief (change device state) 
	 *        This function change device state machine to the target state.
	 * @param target_stat: target state.
	 * @return null
	 * @author : Javnson
	 * @date   : 20230810
	 */
	//gmp_stat_t chg_dev_stat(uint8_t target_stat);


public:

	/**
	 * @brief get if device is readbale.
	 * @param null
	 * @retval 1 : readable; 0 : not readable
	 * @author : Javnson
	 * @date   : 20230810
	 */
	gmp_fast_t is_readable();


	/**
	 * @brief get if device is writeable.
	 * @param null
	 * @retval 1 : writeable; 0 : not writeable
	 * @author : Javnson
	 * @date   : 20230810
	 */
	gmp_fast_t is_writeable();

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
		character.all = GMPDEV_STAT_CHAR_R | GMPDEV_STAT_CHAR_W;

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
		character.all = GMPDEV_STAT_CHAR_R | GMPDEV_STAT_CHAR_W;
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
		character.all = GMPDEV_STAT_CHAR_R | GMPDEV_STAT_CHAR_W;

	}
	i2c_device(void* device_addr)
	{
		character.all = GMPDEV_STAT_CHAR_R | GMPDEV_STAT_CHAR_W;

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




#pragma endregion GPIO_DEVICE_DEF

// utility functions
// You may disable utility functions by define GMP_DISABLE_DEVICE_UTILITY
#ifndef GMP_DISABLE_DEVICE_UTILITY


gmp_diff_t put_format_string(io_device_base* device, gmp_addr_t addr, const gmp_data_t* p_fmt, ...);




#endif // end of _DISABLE_UTILITY

#endif // _FILE_COMM_BASIC_H_
