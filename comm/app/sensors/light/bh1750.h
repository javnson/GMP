// system include


// platform include
#include <comm/app/physical_device_base.h>

#ifndef _FILE_BH1750_H_

// opecode table
#define BH1750_OPC_POWER_DOWN		(0b00000000)
#define BH1750_OPC_POWER_ON			(0b00000001)
#define BH1750_OPC_RESET			(0b00000111)
#define BH1750_OPC_CONT_HE			(0b00010000)
#define BH1750_OPC_CONT_H2			(0b00010001)
#define BH1750_OPC_CONT_L			(0b00010011)
#define BH1750_OPC_ONET_H			(0b00100000)
#define BH1750_OPC_ONET_H2			(0b00100001)
#define BH1750_OPC_ONET_L			(0b00100011)
#define BH1750_OPC_CHG_TIME_H(MT)	(MT & 0b00000111 | 0b01000000)
#define BH1750_OPC_CHG_TIME_L(MT)	(MT & 0b00011111 | 0b01100000)

class bh1750
	: public phy_dev
{
public:


public:
	// ctor & dtor
	bh1750()
	{
		record_len = 1;
		records = &temp_rec;
		addr =
#if ADDR_SEL == 0
			0b0100011
#else
			0b1011100
#endif // ADDR_SEL 
			;

		records[0].cmd = BH1750_CMD_ONET_H;
		records[0].v_length = 2;
	}

	~bh1750()
	{}

public:
	
	// This class will implement the cmd function.
	RESPONSE_CMD


public:
	dev_record_t temp_rec;
		
};

#endif // _FILE_BH1750_H_
