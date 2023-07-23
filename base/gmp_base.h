
// All the contents in this file shouldn't be changed.
// All the config for GMP should & could be done in <userconfig.h>.
// 


// system include
#include <stdint.h>

// user config 
#include <./base/userconfig.h>


// disable warning of unrecognized #pragma
// You may add a -diag_supress=161 to Option/C/C++/Misc Controls

//#pragma clang diagnostic ignored "161" 

// This files only contains the basic macros for EM-LIB

#ifndef _FILE_BMP_MAIN_H_
#define _FILE_BMP_MAIN_H_

// Basic definition of constant values.S
#pragma region BasicMacros

// The descriptor for function parameters
#ifndef _OUT
#define _OUT
#endif

#ifndef _IN
#define _IN
#endif

#ifndef nullptr
#define nullptr ((0))
#endif

// Definition of numerical parameters

// This parameter is for general string definition
#ifndef SHOW_BUF_LEN
#define SHOW_BUF_LEN 128
#endif

// This parameter is for counting waiting.
#ifndef GMP_MAX_WAIT_CNT
#define GMP_MAX_WAIT_CNT 0x1FF
#endif

#ifndef GMP_MAX_WAIT_MS
#define GMP_MAX_WAIT_MS 2
#endif

#ifndef __weak
#define __weak   __attribute__((weak))
#endif /* __weak */


#pragma endregion BasicMacros

// The type definition for GMP
#pragma region Tytpedef_GMP

// unit of GMP data type
// generally is 16 bits or 8 bits
// All the informations would have 
#ifdef GMP_DATA_T
typedef GMP_DATA_T gmp_data_t;
#else
typedef uint8_t gmp_data_t;
#define gmp_data_s ((1))
#endif // GMP_DATA_T

// container of a address
// generally is an unsigned type 
#ifdef GMP_PTRADDR_T
typedef EM_PTRADDR_T gmp_addr_t;
#else
typedef uint32_t gmp_addr_t;
#define gmp_addr_s ((4))
#endif // GMP_PTRADDR_T

// container of a offset of address
// generally is a signed type
#ifdef GMP_PTRDIFF_T
typedef GMP_PTRDIFF_T gmp_diff_t;
#else
typedef int32_t gmp_diff_t;
#define gmp_diff_s ((4))
#endif // GMP_PTRDIFF_T

// container of a counter
// generally is a unsigned type
#ifdef GMP_SIZE_T
typedef GMP_SIZE_T gmp_size_t;
#else
typedef uint32_t gmp_size_t;
#define gmp_size_s ((4))
#endif // GMP_SIZE_T

// container of error code
#ifdef GMP_ERRCODE_T
typedef GMP_ERRCODE_T gmp_errcode_t;
#else
typedef uint_least32_t gmp_errcode_t;
#define gmp_errcode_s ((sizeof(uint_least32_t)))
#endif

// fast container
#ifdef GMP_FAST_T
typedef GMP_FAST_T gmp_fast_t;
#else
typedef uint16_t gmp_fast_t;
#define gmp_fast_s ((2))
#endif

// param container
#ifdef GMP_PARAM_T
typedef GMP_PARAM_T gmp_param_t;
#else
typedef uint_least32_t gmp_param_t;
#define gmp_param_s ((sizeof(uint_least32_t)))
#endif

#define _TEXT(X) ((const char*)(X))


#pragma endregion Tytpedef_GMP

// The definition of Status definition
#pragma region GMP_Status_def

// status type
// This type is for function return 
// The lower 8bits is for warning type return
// The higher 8 bits is for error type return
// 0 is normal type
#ifdef GMP_STAT_T
typedef GMP_STATUS_T gmp_stat_t;
#else
typedef uint_fast16_t gmp_stat_t;
#define gmp_status_s ((sizeof(uint_fast16_t)))
#endif




#define GMP_STAT_INFO_BEGIN        (0x00000000)
#define GMP_STAT_WARN_BEGIN        (0x80000000)
#define GMP_STAT_ERRO_BEGIN        (0xC0000000)


// This area to define all infos, warnings or errors.
#define GMP_STAT_OK				        (0x00000000)
#define GMP_STAT_GENERAL_WARN           (GMP_STAT_WARN_BEGIN + 1)
#define GMP_STAT_GENERAL_ERROR	        (GMP_STAT_ERRO_BEGIN + 1)
#define GMP_STAT_WARN_PRINT             (GMP_STAT_WARN_BEGIN + 2)
#define GMP_STAT_UNDEFINED_ACTION       (GMP_STAT_WARN_BEGIN + 3)
#define GMP_STAT_HARD_ERROR             (GMP_STAT_ERRO_BEGIN + 4)
#define GMP_STAT_NOT_IMPL               (GMP_STAT_ERRO_BEGIN + 5)


// This macro helps to judge if a status code is a fatal error
#define IS_GMP_ERROR(_x) ((_x > GMP_STAT_ERRO_BEGIN))

#define IS_GMP_WARN(_x) ((_X > GMP_STAT_WARN_BEGIN))


#define GMP_STATUS_WARN_TYPE       (0x0001)
#define GMP_STATUS_ERRO_TYPE       (0x8001)

// This area to define all warnings or errors.
#define GMP_STATUS_OK				(0x0000)
#define GMP_STATUS_GENERAL_WARN     (GMP_STATUS_WARN_TYPE + 0)
#define GMP_STATUS_GENERAL_ERROR	(GMP_STATUS_ERRO_TYPE + 0)
#define GMP_STATUS_WARN_PRINT       (GMP_STATUS_WARN_TYPE + 1)
#define GMP_STATUS_UNKNOWN_CMD      (GMP_STATUS_WARN_TYPE + 2)





// This area to define all errors.
// When these error happened, the software usually abort.
// This value is for unknown fatal error.
#define GMP_STATUS_FATAL_ERROR	(0xFFFF)

// This macro helps to judge if a status code is a fatal error
#define IS_GMP_ERROR(x) ((x & 0x8000) != 0)


#pragma endregion GMP_Status_def



#ifdef __cplusplus
extern "C"
{
#endif

    //////////////////////////////////////////////////////////////////////////
    // System basic function which should be defined explicit by users.
    // GMP has provided a weak function which is a null function. User may define this function 
    // in BSP files or other source files.

    // @brief Debug log default print function
    // @param content: the debug log to be printed.
    // @param len    : the length of the content.
    // @retval       : This function returns nothing. This function should designed as nothrow function.
    void gmp_dbg_write_default(_IN const char* content, _IN gmp_size_t len);


    // @brief Get system tick number
    // @param null
    // @retval : Get system tick counter
    gmp_size_t gmp_get_tick(void);

    // @brief this function would be called, when gmp_abort() function is invoked.
    // That is, the time when the GMP library meet fatal error, which can't resolve.
    // The function would be the last function being called.
    // @param null
    // @retval null
    void gmp_abort_callback(void);

    // System basic function which was been defined by GMP library

    // @param Debug formatted string print function
    // @param s     : the formatted string to be printed
    // @param ...   : the explicit content of formatted variables.
    gmp_stat_t gmp_print(_IN const char* content, ...);

    // @brief This function will be called when GMP meets a fatal error
    // generally this function would contain a endless loop.
    // @param null
    // @retval null
    void gmp_abort(void);

    // @brief This function would stop the processing, and wait a few microseconds.
    // @param us the time to be delayed.
    // @note This function would be called when init or communicate, if necessary.
    void gmp_delay(gmp_size_t us);

    //////////////////////////////////////////////////////////////////////////
    // GMP library is based on ASSERT, this section provided a ASSERT independently. 
    // This function will be called when it's necessary to ensure param format.

    // This function would be called, when assert was trigger.
    // This function will not return, and this function will call gmp_abort() function before shutdown.
    // @param msg : The note to be show.
    // @param filepath : The file path to be showed
    // @param line : The assert line position.
    uint16_t gmp_assert_print(_IN const char* msg,
        _IN const char* filepath, _IN gmp_size_t line);

    // This macro function provide a easy-using assert for GMP library.
    // This macro would be called when assert is triggered.
#ifdef GMP_DEBUG
#define gmp_assert(expression) (void)(                              \
            (!!(expression)) ||                                     \
            (gmp_assert_print((const gmp_data_t*)(#expression),     \
                (__FILE__), (gmp_size_t)(__LINE__)))                \
        )
#else
#define gmp_assert(expression) ((void)(0))
#endif // _DEBUG

    //////////////////////////////////////////////////////////////////////////
    // System global variables
    // This is a function address of a function, point to debug output target.
    // Originally, this variable point to gmp_dbg_write_default function.
    // User may change this function if necessary.
    // Attention: this function should defined by C call protocol.
    extern void (*dbg_write)(_IN const char*, _IN gmp_size_t);

    // There are several functions user should call them manually.

    // @brief This function will startup the whole library in order.
    //        And user should call this function in init process.
    void gmp_startup(void);


    // @brief This variable is used to be a counter of all the infos, warnings and errors.
    extern uint32_t g_info_cnt;


    // @brief This variable is used to be a counter of all warnings
    extern uint32_t g_warn_cnt;

    // @brief This variable is used to be a counter of all errors.
    extern uint32_t g_erro_cnt;


#ifdef __cplusplus
}
#endif

#endif // _FILE_BMP_MAIN_H_
