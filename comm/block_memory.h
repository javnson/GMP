// This file is a abstract of FLASH memory.
// Not only for SPI FLASH or IIC EEPROM

// platform headers
#include <comm/comm_base.h>
#include <comm/protocol_base.h>

#ifndef _FILE_BLOCK_MEMORY_H_
#define _FILE_BLOCK_MEMORY_H_

// This class contains an abstract of memory.
// This type of memory should be erased first and then write (program) it.

class block_memory
	: public protocol_base
{
public:
	// ctor & dtor
	block_memory()
	{}

	~block_memory()
	{}

public:
	// kernel virtual function
	
	// command react
	RESPONSE_CMD

	// read write react
	RESPONSE_RW

	// erase react
	// NOTE: User should call erase function firstly, and then call write function to write data.
	
	/**
	 * @brief This function erase the range of [start_addr, cmpt_addr].
	 *        The erase range will fullly cover the range by the unit for Flash erase.
	 * @param start_addr the start address to be erased.
	 * @param cmpt_addr the completion address to be erased.
	 * @return if the function is completed correctly.
	 * @author : Javnson
	 * @date   : 20230809
	 */
	virtual gmp_stat_t erase_range(gmp_addr_t start_addr, gmp_addr_t cmpt_addr);


public:
	// utilities

public:
	// members
	uint32_t m_memory_type;

	// The capacity of the FLASH memory.
	uint32_t m_capacity;

	// sector is the least unit for Flash erase.
	uint32_t m_sector_size;

};

#pragma region CFI_SPI_FLASH


// Joint Electron Device Engineering Council (JEDEC) ID instruction
// Common Flash Interface (CFI) 2003 JESD69.01
// specification outlines a device and host system software interrogation handshake that allows specific software algorithms to be used for entire families of devices. 
// This allows device-independent, JEDEC ID-independent, and forward- and backward-compatible software support for the specific flash families. 
// It allows flash vendors to standardize their existing interfaces for long-term compatibility. 
//

// Reg operation
// The Read Status Register instructions allow the 8-bit Status Registers to be read.
#define CFI_CMD_READ_STAT_REG1       (0x05)
#define CFI_CMD_READ_STAT_REG2       (0x35)

// The Write Status Register instruction allows the Status Register to be written.
// Only non-volatile Status Register bits can be written to.
// All other Status Register bit locations are read - only and will not be affected by the Write Status Register instruction.
#define CFI_CMD_WRITE_STAT_REG       (0x01)

// The Read Manufacturer/Device ID instruction is an alternative to the Release from Power-down / Device
// ID instruction that provides both the JEDEC assigned manufacturer IDand the specific device ID.
#define CFI_CMD_GET_MANUFACT		 (0x90)

// The Read Unique ID Number instruction accesses a factory-set read-only 64-bit number that is unique to
// each W25Q64BV device.
#define CFI_CMD_GET_UNIQUE_ID		 (0x4B)

// The Read JEDEC ID instruction is compatible with the JEDEC standard for SPI
// compatible serial memories that was adopted in 2003.
#define CFI_CMD_GET_JEDEC_ID		 (0x9F)

// Power Control

// Although the standby current during normal operation is relatively low, standby current can be further
// reduced with the Power - down instruction.
#define CFI_CMD_POWERDOWN            (0xB9)

// The High Performance Mode (HPM) instruction must be executed prior to Dual or Quad I/O instructions
// when operating at high frequencies. This instruction allows pre - charging of internal charge pumps
// so the voltages required for accessing the Flash memory array are readily available.
#define CFI_CMD_HPERFORM_MODE        (0xA3)

// This command does nothing
#define CFI_CMD_CONT_READ_MODE_RESET (0xFF)

// The Release from Power-down or High performance Mode
#define CFI_CMD_RELEASE_HPM			 (0xAB)
#define CFI_CMD_RELEASE_POWERDOWN	 (0xAB)

// Write Command

// The command will set Write Enable Latch (WEL) bit in Stat Register.
// The WEL bit must be set prior to every Page Program, Sector Erase, 
// Block Erase, Chip Erase and Write Status Register instruction.
// Note that the WEL bit is automatically reset after Power-up and upon
// completion of the Write Status Register, Page Program, Sector Erase, 
// Block Eraseand Chip Erase instructions.
#define CFI_CMD_WRITE_ENABLE         (0x06)

// The command will clear WEL bit.
#define CFI_CMD_WRITE_DISABLE        (0x04)

// The Page Program instruction allows from one byte to 256 bytes (a page) of data to be programmed at
// previously erased (FFh) memory locations.
#define CFI_CMD_PAGE_PROG            (0x02)

// The Quad Page Program instruction allows up to 256 bytes of data to be programmed at previously
// erased(FFh) memory locations using four pins : IO0, IO1, IO2, and IO3.
#define CFI_CMD_QUAD_PAGE_PROG       (0x32)

// The Block Erase instruction sets all memory within a specified block (64K-bytes) to the erased state of all
// 1s(FFh).A Write Enable instruction must be executed before the device will accept the Block Erase
// Instruction(Status Register bit WEL must equal 1).
#define CFI_CMD_BLOCK_ERASE64        (0xD8)

// The Block Erase instruction sets all memory within a specified block (32K-bytes) to the erased state of all
// 1s(FFh).A Write Enable instruction must be executed before the device will accept the Block Erase
// Instruction(Status Register bit WEL must equal 1).
#define CFI_CMD_BLOCK_ERASE32        (0x52)

// The Sector Erase instruction sets all memory within a specified sector (4K-bytes) 
// to the erased state of all 1s(FFh).
#define CFI_CMD_SECTOR_ERASE         (0x20)

// The Chip Erase instruction sets all memory within the device to the erased state of all 1s (FFh). A Write
// Enable instruction must be executed before the device will accept the Chip Erase Instruction(Status
// Register bit WEL must equal 1).
#define CFI_CMD_CHIP_ERASE           (0x60)

// The Erase Suspend instruction ¡°75h¡±, allows the system to interrupt a Sector or Block Erase operation
// and then read from or program data to, any other sectors or blocks.
// The Write Status Register instruction (01h) and Erase instructions (20h, 52h, D8h, C7h, 60h) are not
// allowed during Erase Suspend.Erase Suspend is valid only during the Sector or Block erase operation.
#define CFI_CMD_ERASE_SUSPEND        (0x75)

// The Erase Resume instruction must be written to resume the Sector or Block Erase operation 
// after an Erase Suspend.
#define CFI_CMD_ERASE_RESUME         (0x7A)

// Read Instruction

// The Read Data instruction allows one more data bytes to be sequentially read from the memory.
// This instruction will get the flash data until the clock is stop and CS signal goes high.
#define CFI_CMD_READ_DATA            (0x03)

// The Fast Read instruction is similar to the Read Data instruction except 
// that it can operate at the highest possible frequency
#define CFI_CMD_FAST_READ            (0x0B)

// The Fast Read Dual Output (3Bh) instruction is similar to the standard Fast Read (0Bh) instruction 
// except that data is output on two pins; IO0 and IO1.
#define CFI_CMD_FREAD_DUAL_OUT       (0x3B)

// The Fast Read Dual I/O (BBh) instruction allows for improved random access 
// while maintaining two IO pins, IO0 and IO1.
#define CFI_CMD_FREAD_DUAL_IO        (0xBB)

// The Fast Read Quad Output (6Bh) instruction is similar to the Fast Read Dual Output (3Bh) instruction
// except that data is output on four pins, IO0, IO1, IO2, and IO3
#define CFI_CMD_FREAD_QUAD_OUT       (0x6B)

// The Fast Read Quad I/O (EBh) instruction is similar to the Fast Read Dual I/O (BBh) instruction except
// that addressand data bits are inputand output through four pins IO0, IO1, IO2 and IO3 and 
// four Dummy clocks are required prior to the data output.
#define CFI_CMD_FREAD_QUAD_IO        (0xEB)

// The Octal Word Read Quad I/O (E3h) instruction is similar to the Fast Read Quad I/O (EBh) instruction
// except that the lower four Address bits(A0, A1, A2, A3) must equal 0.
#define CFI_CMD_READ_OCT_WORD        (0xE3)

class cfi_spi_flash
	: public block_memory
{

};

#pragma endregion CFI_SPI_FLASH


#endif // _FILE_BLOCK_MEMORY_H_

