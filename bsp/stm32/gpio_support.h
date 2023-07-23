


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
