
/** 
  * @brief General Purpose I/O
  */
typedef struct

{
  volatile unsigned int MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
  volatile unsigned int OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
  volatile unsigned int OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
  volatile unsigned int PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  volatile unsigned int IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
  volatile unsigned int ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
  volatile unsigned int BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
  volatile unsigned int LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  volatile unsigned int AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
} GPIO_TypeDef;

/** 
  * @brief Controller Area Network TxMailBox 
  */
typedef struct
{
  volatile unsigned int TIR;  /*!< CAN TX mailbox identifier register */
  volatile unsigned int TDTR; /*!< CAN mailbox data length control and time stamp register */
  volatile unsigned int TDLR; /*!< CAN mailbox data low register */
  volatile unsigned int TDHR; /*!< CAN mailbox data high register */
} CAN_TxMailBox_TypeDef;

/** 
  * @brief Controller Area Network FIFOMailBox 
  */
typedef struct
{
  volatile unsigned int RIR;  /*!< CAN receive FIFO mailbox identifier register */
  volatile unsigned int RDTR; /*!< CAN receive FIFO mailbox data length control and time stamp register */
  volatile unsigned int RDLR; /*!< CAN receive FIFO mailbox data low register */
  volatile unsigned int RDHR; /*!< CAN receive FIFO mailbox data high register */
} CAN_FIFOMailBox_TypeDef;

/** 
  * @brief Controller Area Network FilterRegister 
  */
  
typedef struct
{
  volatile unsigned int FR1; /*!< CAN Filter bank register 1 */
  volatile unsigned int FR2; /*!< CAN Filter bank register 1 */
} CAN_FilterRegister_TypeDef;


/** 
  * @brief Controller Area Network 
  */
typedef struct
{
  volatile unsigned int              MCR;                 /*!< CAN master control register,         Address offset: 0x00          */
  volatile unsigned int              MSR;                 /*!< CAN master status register,          Address offset: 0x04          */
  volatile unsigned int              TSR;                 /*!< CAN transmit status register,        Address offset: 0x08          */
  volatile unsigned int              RF0R;                /*!< CAN receive FIFO 0 register,         Address offset: 0x0C          */
  volatile unsigned int              RF1R;                /*!< CAN receive FIFO 1 register,         Address offset: 0x10          */
  volatile unsigned int              IER;                 /*!< CAN interrupt enable register,       Address offset: 0x14          */
  volatile unsigned int              ESR;                 /*!< CAN error status register,           Address offset: 0x18          */
  volatile unsigned int              BTR;                 /*!< CAN bit timing register,             Address offset: 0x1C          */
   unsigned int                   RESERVED0[88];       /*!< Reserved, 0x020 - 0x17F                                            */
  CAN_TxMailBox_TypeDef      sTxMailBox[3];       /*!< CAN Tx MailBox,                      Address offset: 0x180 - 0x1AC */
  CAN_FIFOMailBox_TypeDef    sFIFOMailBox[2];     /*!< CAN FIFO MailBox,                    Address offset: 0x1B0 - 0x1CC */
   unsigned int                   RESERVED1[12];       /*!< Reserved, 0x1D0 - 0x1FF                                            */
  volatile unsigned int              FMR;                 /*!< CAN filter master register,          Address offset: 0x200         */
  volatile unsigned int              FM1R;                /*!< CAN filter mode register,            Address offset: 0x204         */
   unsigned int                   RESERVED2;           /*!< Reserved, 0x208                                                    */
  volatile unsigned int              FS1R;                /*!< CAN filter scale register,           Address offset: 0x20C         */
   unsigned int                   RESERVED3;           /*!< Reserved, 0x210                                                    */
  volatile unsigned int              FFA1R;               /*!< CAN filter FIFO assignment register, Address offset: 0x214         */
   unsigned int                   RESERVED4;           /*!< Reserved, 0x218                                                    */
  volatile unsigned int              FA1R;                /*!< CAN filter activation register,      Address offset: 0x21C         */
   unsigned int                   RESERVED5[8];        /*!< Reserved, 0x220-0x23F                                              */ 
  CAN_FilterRegister_TypeDef sFilterRegister[28]; /*!< CAN Filter Register,                 Address offset: 0x240-0x31C   */
} CAN_TypeDef;

int *RCC_clock_enable_GPIO = (int *) 0x40023830;
int *RCC_clock_enable_CAN1 = (int *) 0x40021800;

int main (void)
{
GPIO_TypeDef *GPIOA = (GPIO_TypeDef *) 0x40020000;
GPIO_TypeDef *GPIOG = (GPIO_TypeDef *) 0x40021800;

CAN_TypeDef *CAN1 = ( CAN_TypeDef *) 0x40006400;
CAN_FilterRegister_TypeDef *filterRegister = (CAN_FilterRegister_TypeDef *) (0x400006400 +0x240);
CAN_FIFOMailBox_TypeDef *FiFOMailBox = (CAN_FIFOMailBox_TypeDef *) (0x40006400+0x1B0);
CAN_TxMailBox_TypeDef *TxMailBox = (CAN_TxMailBox_TypeDef *) (0x40006400 +0x180);
	
	*RCC_clock_enable_GPIO |= (1<<0) | (1<<6); 
	*RCC_clock_enable_CAN1 |= (1<<25) | (1<<6); 
	volatile int txcounter = 0 ;
	volatile int rxData ;
	volatile int dlc ; 
	volatile int filterIndex ;
	
	/* Configure GPIO with AF9, CAN1 connected to APB1 with 45MHz clock
   * CAN1_RX  -> PA11 (IN)
   * CAN1_TX  -> PA12 (OUT)
   * ***********************/

  GPIOA->MODER |=(1 << 12);
  GPIOA->MODER |=  (1 << 13);

  /* Select alternate function mode */
	GPIOA->MODER |=(1 << 23);
  GPIOA->MODER |=  (1 << 25);
  
  

  /* Select output speed medium */
  GPIOA->OSPEEDR |= (1 << 24) ;

  /* Select pull up */
  GPIOA->PUPDR |= (1<<23); 
  GPIOA->PUPDR |= (1<<25);

  /* Select AF9 */
  GPIOA->AFR[1] |= (9<<12); 
  GPIOA->AFR[1] |= (9<<16);
	
	 

  /* Set automatic bus-off management */
  CAN1->MCR |= (1<<6);

  /* Set transmit FIFO priority driven by the request order */
  CAN1->MCR |= (1<<2);

  /* Initialization request */
  CAN1->MCR |= (1<<0);

  /* Request to exit Sleep mode */
  CAN1->MCR &= ~(1<<1);

  /* Wait for initialization mode */
	while (( CAN1 ->MSR &= ~ (1<<1) == 1 ) || ((CAN1 ->MSR |= (1<<0)) != (CAN1 -> MSR |= (1<<0) & CAN1 ->MSR )))
  {
    /* Do nothing until initialization mode is entered */
  }

	/*BaudRate = 1 / NominalBitTime = 1 MBits/s */
  CAN1->BTR = 0x00060004;

  /* Reset identifier register, no extension, no remote */
  CAN1->sTxMailBox[0].TIR = 0;

  /* Set CAN standard identifier = 0x100 */
  CAN1->sTxMailBox[0].TIR |= 0x20000000;

  /* Set CAN frame length to 8 */
  
  CAN1->sTxMailBox[0].TDTR |= 0x00000008;

  /* Reset transmit data registers  */
  CAN1->sTxMailBox[0].TDLR = 0;
  CAN1->sTxMailBox[0].TDHR = 0;
	
	if (CAN1 ->TSR & (1<<26))
	{
	
	txcounter++;
		
		/*Set TX counter */
		CAN1 ->sTxMailBox[0].TDLR = txcounter ;
		
		/*Transmission request */
		CAN1 ->sTxMailBox[0].TIR |= (1<<0);
}
	else 
	
	{}
		
		 /* Filter setup,
   * Filter 1 0x25           Turn on/off green led
   * Filter 2 (0x10 - 0x1F)  Turn on/off green led
   * Filter 3 0x15           Turn on/off red led
   * Filter 4 0x20 - 0x2F    Turn on/off red led
   *
   * After reset filters in init mode and for CAN1
   * filter until number 13. Filter scale dual 16-bit */

  /* Set Filter init mode */
  CAN1->FMR |= (1<<0);

  /* Set filer mode, 1 and 3 list, 2 and 4 mask */
  CAN1->FM1R &= ~ ((1<<2) | (1<<4));
  CAN1->FM1R |=  (1<<1) | (1<<3);

  /* Set FIFO 0 for all filters */
  CAN1->FFA1R &= ~((1<<1) | (1<<2) | (1<<3) | (1<<4));
		
		/* Set filter 1, list mode with ID = 0x25 */
  CAN1->sFilterRegister[1].FR1 = 0x04A004A0; 
  CAN1->sFilterRegister[1].FR2 = 0x04A004A0; 

  /* Set filter 2, mask mode with ID = 0x10:0x1F */
  CAN1->sFilterRegister[2].FR1 = 0xFE1F0200; 
  CAN1->sFilterRegister[2].FR2 = 0xFE1F0200; 

  /* Set filter 3, list mode with ID = 0x15 */
  CAN1->sFilterRegister[3].FR1 = 0x02A002A0; 
  CAN1->sFilterRegister[3].FR2 = 0x02A002A0; 

  /* Set filter 4, mask mode with ID = 0x20:0x2F */
  CAN1->sFilterRegister[4].FR1 = 0xFE1F0400; 
  CAN1->sFilterRegister[4].FR2 = 0xFE1F0400;
	
	 /* Activate filters */
  CAN1->FA1R |= (1<<1) | (1<<2) | (1<<3) | (1<<4) ;

  /* Active filters mode */
  CAN1->FMR &= ~(1<<0);

  /* Request normal mode */
  CAN1->MCR &= ~(1<<0) | (1<<1) ;

/* Check FIFO message pending */
  if(0 != (CAN1 ->RF0R & ((1<<0)&(1<<1))))
  {
    /* FIFO not empty, read data */
    rxData = (CAN1->sFIFOMailBox[0].RDLR);

    /* Get filter match index */
    filterIndex = (CAN1->sFIFOMailBox[0].RDTR >> 8);

    /* Get data length code */
    dlc = (CAN1->sFIFOMailBox[0].RDTR);

    /* Check filter index */
    if((2 <= filterIndex) && (7 >= filterIndex) && (1 == dlc))
    {
      /* Green led control */
      if(0x0F == rxData)
      {
        /* Turn off green led */
        GPIOG-> ODR &= ~(1<<12);
      }
      else if (0x1F == rxData)
      {
        /* Turn on green led */
        GPIOG-> ODR |= (1<<13);
      }
     
  else
  {
    /* Do nothing, no new data */
  }

	}
}
	}

