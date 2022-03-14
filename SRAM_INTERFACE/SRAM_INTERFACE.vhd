library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

ENTITY SRAM_INTERFACE IS
PORT
(
  datain			    		:	IN	STD_LOGIC_VECTOR(3 DOWNTO 0);	-- SW3-0
  Clock						:	IN	STD_LOGIC;						-- KEY0
  
  WriteEnable				:	IN	STD_LOGIC_VECTOR(0 DOWNTO 0);	-- SW9	
  WriteEnableLEDG			:	OUT STD_LOGIC_VECTOR(0 DOWNTO 0);	-- LEDG 
  WriteEnableSRAM_WE_N	:	OUT STD_LOGIC_VECTOR(0 DOWNTO 0);
  datainHEX0				:	OUT STD_LOGIC_VECTOR(6 DOWNTO 0);	-- HEX0
  
  dataBusSRAM_DQ			:	INOUT	STD_LOGIC_VECTOR(15 DOWNTO 0) := "ZZZZZZZZZZZZZZZZ";
  dataOutHEX1				:	OUT	STD_LOGIC_VECTOR(6 DOWNTO 0);	-- HEX1
  
  Adress					   :	IN	STD_LOGIC_VECTOR(3 DOWNTO 0);	-- SW7-4
  AdressBusSRAM_ADDR		:	OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
  AdressBusHEX3			:	OUT STD_LOGIC_VECTOR(6 DOWNTO 0);	-- HEX3
	
  SRAM_CE_N					:	OUT STD_LOGIC;
  SRAM_OE_N					:	OUT STD_LOGIC;
  SRAM_UB_N					:	OUT STD_LOGIC;
  SRAM_LB_N					:	OUT STD_LOGIC;
  
  HEX2OFF					: OUT STD_LOGIC_VECTOR(6 DOWNTO 0)
);
END SRAM_INTERFACE;

ARCHITECTURE rtl OF SRAM_INTERFACE IS

  COMPONENT flip_flop IS
	  GENERIC
	  (
		 N : INTEGER RANGE 0 to 3 := 3
	  );
	  PORT
	  (
		 d   :  IN  STD_LOGIC_VECTOR(N DOWNTO 0);
		clk	:  IN  STD_LOGIC;
		
		 q   :  OUT STD_LOGIC_VECTOR(N DOWNTO 0)		
	  );
  END COMPONENT;
  
  COMPONENT binto7seg IS
	  PORT
	  (
		 bin :  IN	STD_LOGIC_VECTOR(3 DOWNTO 0);
		 HEX :  OUT	STD_LOGIC_VECTOR(6 DOWNTO 0)
	  );
  END COMPONENT;

  SIGNAL sig_WE_N     : STD_LOGIC_VECTOR(0 DOWNTO 0);
  SIGNAL sig_SRAM_DQ  : STD_LOGIC_VECTOR(3 DOWNTO 0);
  SIGNAL sig_SAM_ADDR : STD_LOGIC_VECTOR(3 DOWNTO 0);

BEGIN

  WriteEnableLEDG      <= sig_WE_N;
  WriteEnableSRAM_WE_N <= sig_WE_N;
  
  dataBusSRAM_DQ       <= ("000000000000" & sig_SRAM_DQ(3 DOWNTO 0)) WHEN sig_WE_N(0) = '0' ELSE
                           (OTHERS=>'Z');
									
  
  AdressBusSRAM_ADDR   <= sig_SAM_ADDR;
  
   SRAM_CE_N <= '0';
   SRAM_OE_N <= '0';
   SRAM_UB_N <= '0';
   SRAM_LB_N <= '0';
  
  ff_WE:	flip_flop
  GENERIC MAP(N => 0)
  PORT MAP(WriteEnable,Clock,sig_WE_N);
	
  ff_DQ: flip_flop
  GENERIC MAP(N => 3)
  PORT MAP(datain,Clock,sig_SRAM_DQ);
	
  ff_ADDR:	flip_flop
  GENERIC MAP(N => 3)
  PORT MAP(Adress,Clock,sig_SAM_ADDR);
	
  tr_WE:	binto7seg
  PORT MAP(datain,datainHEX0);
  
  tr_DQ:	binto7seg
  PORT MAP(dataBusSRAM_DQ(3 DOWNTO 0),dataOutHEX1);
	
  tr_ADDR:	binto7seg
  PORT MAP(sig_SAM_ADDR,AdressBusHEX3);
  
  HEX2OFF <= "1111111";

end rtl;
