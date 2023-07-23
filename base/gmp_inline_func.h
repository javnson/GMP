// This file will implement all the inline function, user should call.
// And generally these function may be invoked frquently, such as main-loop
// main-isr and etc.

// platform includes
#include <gmp_base.h>

#ifdef __cplusplus
extern "C"
{
#endif

	// This function should be called by user in device mainloop function.
	static inline void gmp_mainloop()
	{

	}

	// This function shold be called by user in device main ISR function.
	static inline void gmp_mainisr()
	{

	}

	static inline void sleep()
	{

	}


	// The following functions are for GPIO operations
	// Here only prototype, user should implement these functions in BSP

#ifndef _GMP_BSP_GPIO_SUPPORT
	
	/**
	 * @brief This function set a GPIO to high level.
	 * @param port_group the gpio group
	 * @param gpio_index the gpio index
	 * @return if gpio is set or clear.
	 * @author : Javnson
	 * @date   : 20230723
	 */
	static inline gmp_stat_t gmp_gpio_set(gmp_addr_t port_group, gmp_size_t gpio_index)
	{
		return GMP_STAT_NOT_IMPL;
	}

	/**
	 * @brief This function set a GPIO to low level.
	 * @param port_group the gpio group
	 * @param gpio_index the gpio index
	 * @return if gpio is set or clear.
	 * @author : Javnson
	 * @date   : 20230723
	 */
	static inline gmp_stat_t gmp_gpio_reset(gmp_addr_t port_group, gmp_size_t gpio_index)
	{
		return GMP_STAT_NOT_IMPL;
	}

	/**
	 * @brief This function change the level of a GPIO.
	 * @param port_group the gpio group
	 * @param gpio_index the gpio index
	 * @return if gpio is set or clear.
	 * @author : Javnson
	 * @date   : 20230723
	 */
	static inline gmp_stat_t gmp_gpio_toggle(gmp_addr_t port_group, gmp_size_t gpio_index)
	{
		return GMP_STAT_NOT_IMPL;
	}

	/**
	 * @brief This function read a specified GPIO port.
	 * @param port_group the gpio group
	 * @param gpio_index the gpio index
	 * @return 0 the GPIO has low level, 1 the GPIO has high level,
	 *	       -1 the read method invalid.
	 * @author : Javnson
	 * @date   : 20230723
	 */
	static inline int8_t gmp_gpio_read(gmp_addr_t port_group, gmp_size_t gpio_index)
	{
		return GMP_STAT_NOT_IMPL;
	}


#endif // end of _GMP_BSP_GPIO_SUPPORT
	

#ifdef __cplusplus
}
#endif
