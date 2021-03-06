LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;
USE ieee.std_logic_unsigned.ALL;

ENTITY flip_flop IS
GENERIC
(
	N : INTEGER RANGE 0 to 3 := 3
);

PORT
(
	d	:	IN	STD_LOGIC_VECTOR(N DOWNTO 0);
	clk	:	IN	STD_LOGIC;
	
	q	:	OUT	STD_LOGIC_VECTOR(N DOWNTO 0)
	
);
END flip_flop;

ARCHITECTURE rtl OF flip_flop IS

BEGIN

	PROCESS(clk)
	BEGIN
		IF rising_edge(clk) THEN
				q <= d;
		END IF;
	END PROCESS; 
	
END rtl;
