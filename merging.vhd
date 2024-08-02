library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
USE ieee.numeric_std.ALL;

use work.ipbus.all;
use work.emp_data_types.all;
use work.emp_project_decl.all;

use work.emp_device_decl.all;
use work.emp_ttc_decl.all;
 --- to do check, which clock to use

entity emp_payload is
	port(
		clk: in std_logic; -- ipbus signals
		rst: in std_logic;
		ipb_in: in ipb_wbus;
		ipb_out: out ipb_rbus;
		clk_payload: in std_logic_vector(2 downto 0);
		rst_payload: in std_logic_vector(2 downto 0);
		clk_p: in std_logic; -- data clock
		rst_loc: in std_logic_vector(N_REGION - 1 downto 0);
		clken_loc: in std_logic_vector(N_REGION - 1 downto 0);
		ctrs: in ttc_stuff_array;
		bc0: out std_logic;
		d: in ldata(4 * N_REGION - 1 downto 0); -- data in
		q: out ldata(4 * N_REGION - 1 downto 0); -- data out
		gpio: out std_logic_vector(29 downto 0); -- IO to mezzanine connector
		gpio_en: out std_logic_vector(29 downto 0) -- IO to mezzanine connector (three-state enables)
	);
end emp_payload;

architecture rtl of emp_payload is

  constant nRNGs : natural := 8;
  constant nBigRand : natural := 4600;
  constant seedOffset : integer := 111111;
  constant seedProduct : integer := 13;
  type rng_vector is array(nRNGs-1 downto 0) of std_logic_vector(63 downto 0);
  signal r_has_set_seed : std_logic_vector( nRNGs-1 downto 0 );
  signal r_all_randos : rng_vector;
  signal r_all_seeds  : rng_vector;
  signal r_big_rand_temp : std_logic_vector(nBigRand-1 downto 0); -- here we
                                                                  -- accumulate
                                                                  -- in each
                                                                  -- clock
                                                                  -- cycle the
                                                                  -- random numbers
  signal r_big_rand : std_logic_vector(nBigRand-1 downto 0); -- we read this
                                                             -- from
                                                             -- r_big_rand_temp
                                                             -- each 9 clock cycles

  signal r_clock_counter : natural range 0 to 8 := 0;
  signal r_start_showering : std_logic := '0';
  signal r_particle_container : ldata(127 downto 0); -- here we will put the result every
                                         -- clock cycle

  component xoshiro256hw
    port (
      ap_clk : IN STD_LOGIC;
      ap_clk_rst : IN STD_LOGIC;
      ap_start : IN STD_LOGIC;
      ap_done : OUT STD_LOGIC;
      ap_idle : OUT STD_LOGIC;
      ap_ready : OUT STD_LOGIC;
      setseed : IN STD_LOGIC_VECTOR (0 downto 0);
      seed : IN STD_LOGIC_VECTOR (63 downto 0);
      ap_return : OUT STD_LOGIC_VECTOR (63 downto 0) );
  end component;


  component bitpattern_pileup -- this is simplia
    port (
    ap_clk : IN STD_LOGIC;
    ap_rst : IN STD_LOGIC;
    ap_start : IN STD_LOGIC;
    ap_done : OUT STD_LOGIC;
    ap_idle : OUT STD_LOGIC;
    ap_ready : OUT STD_LOGIC;
    bitpattern_0 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_1 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_2 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_3 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_4 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_5 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_6 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_7 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_8 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_9 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_10 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_11 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_12 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_13 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_14 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_15 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_16 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_17 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_18 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_19 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_20 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_21 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_22 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_23 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_24 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_25 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_26 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_27 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_28 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_29 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_30 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_31 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_32 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_33 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_34 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_35 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_36 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_37 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_38 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_39 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_40 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_41 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_42 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_43 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_44 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_45 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_46 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_47 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_48 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_49 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_50 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_51 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_52 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_53 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_54 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_55 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_56 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_57 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_58 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_59 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_60 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_61 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_62 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_63 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_64 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_65 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_66 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_67 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_68 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_69 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_70 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_71 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_72 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_73 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_74 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_75 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_76 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_77 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_78 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_79 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_80 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_81 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_82 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_83 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_84 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_85 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_86 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_87 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_88 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_89 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_90 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_91 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_92 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_93 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_94 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_95 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_96 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_97 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_98 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_99 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_100 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_101 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_102 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_103 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_104 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_105 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_106 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_107 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_108 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_109 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_110 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_111 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_112 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_113 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_114 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_115 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_116 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_117 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_118 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_119 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_120 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_121 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_122 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_123 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_124 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_125 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_126 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bitpattern_127 : OUT STD_LOGIC_VECTOR (63 downto 0);
    bigrand : IN STD_LOGIC_VECTOR (4599 downto 0) );

end component;

begin


  setting_seeds: process(clk_p)
  begin
    if rising_edge(clk_p) then
      for i in 0 to nRNGs-1 loop
        if r_has_set_seed(i) = '0' then
          r_all_seeds(i)<=std_logic_vector(to_unsigned(i, r_all_seeds(i)'length)); -- to do ,set a reasonable seed
          r_has_set_seed(i) <= '1';
        end if;
      end loop;
    end if;   
  end process;

  gen:
  for i in nRNGs-1 downto 0 generate

    rng : xoshiro256hw
      port map(
        ap_clk => clk_p,
        ap_clk_rst => rst,
        ap_start => '1',
        ap_done => open,
        ap_idle => open,
        ap_ready => open,
        setseed => r_has_set_seed(i),
        seed => r_all_seeds(i),
        ap_return => r_all_randos(i));
    
  end generate gen;

  propagating_rands: process(clk_p)
  begin
    if rising_edge(clk_p) then
      for i in nRNGs-1 downto 0 loop
        for j in 63 downto 0 loop
          if j + 63*i<=nBigRand-1 then
            r_big_rand_temp(j + 63*i+nBigRand*63*r_clock_counter)<=r_all_randos(i)(j);
          end if;
        end loop;
      end loop;       
      
      
      if r_clock_counter = 8 then
        r_clock_counter <= 0;
        r_big_rand <= r_big_rand_temp;
        r_start_showering <= '1';
      else
        r_clock_counter <= r_clock_counter + 1;
        
      end if;
    end if;
  end process propagating_rands;

  simplia : bitpattern_pileup
    port map (
      ap_clk => clk_p,
      ap_rst => rst,
      ap_start => r_start_showering,
      ap_done => open,
      ap_idle => open,
      ap_ready => open,
      bitpattern_0 => r_particle_container(0),
      bitpattern_1 => r_particle_container(1),
      bitpattern_2 => r_particle_container(2),
      bitpattern_3 => r_particle_container(3),
      bitpattern_4 => r_particle_container(4),
      bitpattern_5 => r_particle_container(5),
      bitpattern_6 => r_particle_container(6),
      bitpattern_7 => r_particle_container(7),
      bitpattern_8 => r_particle_container(8),
      bitpattern_9 => r_particle_container(9),
      bitpattern_10 => r_particle_container(10),
      bitpattern_11 => r_particle_container(11),
      bitpattern_12 => r_particle_container(12),
      bitpattern_13 => r_particle_container(13),
      bitpattern_14 => r_particle_container(14),
      bitpattern_15 => r_particle_container(15),
      bitpattern_16 => r_particle_container(16),
      bitpattern_17 => r_particle_container(17),
      bitpattern_18 => r_particle_container(18),
      bitpattern_19 => r_particle_container(19),
      bitpattern_20 => r_particle_container(20),
      bitpattern_21 => r_particle_container(21),
      bitpattern_22 => r_particle_container(22),
      bitpattern_23 => r_particle_container(23),
      bitpattern_24 => r_particle_container(24),
      bitpattern_25 => r_particle_container(25),
      bitpattern_26 => r_particle_container(26),
      bitpattern_27 => r_particle_container(27),
      bitpattern_28 => r_particle_container(28),
      bitpattern_29 => r_particle_container(29),
      bitpattern_30 => r_particle_container(30),
      bitpattern_31 => r_particle_container(31),
      bitpattern_32 => r_particle_container(32),
      bitpattern_33 => r_particle_container(33),
      bitpattern_34 => r_particle_container(34),
      bitpattern_35 => r_particle_container(35),
      bitpattern_36 => r_particle_container(36),
      bitpattern_37 => r_particle_container(37),
      bitpattern_38 => r_particle_container(38),
      bitpattern_39 => r_particle_container(39),
      bitpattern_40 => r_particle_container(40),
      bitpattern_41 => r_particle_container(41),
      bitpattern_42 => r_particle_container(42),
      bitpattern_43 => r_particle_container(43),
      bitpattern_44 => r_particle_container(44),
      bitpattern_45 => r_particle_container(45),
      bitpattern_46 => r_particle_container(46),
      bitpattern_47 => r_particle_container(47),
      bitpattern_48 => r_particle_container(48),
      bitpattern_49 => r_particle_container(49),
      bitpattern_50 => r_particle_container(50),
      bitpattern_51 => r_particle_container(51),
      bitpattern_52 => r_particle_container(52),
      bitpattern_53 => r_particle_container(53),
      bitpattern_54 => r_particle_container(54),
      bitpattern_55 => r_particle_container(55),
      bitpattern_56 => r_particle_container(56),
      bitpattern_57 => r_particle_container(57),
      bitpattern_58 => r_particle_container(58),
      bitpattern_59 => r_particle_container(59),
      bitpattern_60 => r_particle_container(60),
      bitpattern_61 => r_particle_container(61),
      bitpattern_62 => r_particle_container(62),
      bitpattern_63 => r_particle_container(63),
      bitpattern_64 => r_particle_container(64),
      bitpattern_65 => r_particle_container(65),
      bitpattern_66 => r_particle_container(66),
      bitpattern_67 => r_particle_container(67),
      bitpattern_68 => r_particle_container(68),
      bitpattern_69 => r_particle_container(69),
      bitpattern_70 => r_particle_container(70),
      bitpattern_71 => r_particle_container(71),
      bitpattern_72 => r_particle_container(72),
      bitpattern_73 => r_particle_container(73),
      bitpattern_74 => r_particle_container(74),
      bitpattern_75 => r_particle_container(75),
      bitpattern_76 => r_particle_container(76),
      bitpattern_77 => r_particle_container(77),
      bitpattern_78 => r_particle_container(78),
      bitpattern_79 => r_particle_container(79),
      bitpattern_80 => r_particle_container(80),
      bitpattern_81 => r_particle_container(81),
      bitpattern_82 => r_particle_container(82),
      bitpattern_83 => r_particle_container(83),
      bitpattern_84 => r_particle_container(84),
      bitpattern_85 => r_particle_container(85),
      bitpattern_86 => r_particle_container(86),
      bitpattern_87 => r_particle_container(87),
      bitpattern_88 => r_particle_container(88),
      bitpattern_89 => r_particle_container(89),
      bitpattern_90 => r_particle_container(90),
      bitpattern_91 => r_particle_container(91),
      bitpattern_92 => r_particle_container(92),
      bitpattern_93 => r_particle_container(93),
      bitpattern_94 => r_particle_container(94),
      bitpattern_95 => r_particle_container(95),
      bitpattern_96 => r_particle_container(96),
      bitpattern_97 => r_particle_container(97),
      bitpattern_98 => r_particle_container(98),
      bitpattern_99 => r_particle_container(99),
      bitpattern_100 => r_particle_container(100),
      bitpattern_101 => r_particle_container(101),
      bitpattern_102 => r_particle_container(102),
      bitpattern_103 => r_particle_container(103),
      bitpattern_104 => r_particle_container(104),
      bitpattern_105 => r_particle_container(105),
      bitpattern_106 => r_particle_container(106),
      bitpattern_107 => r_particle_container(107),
      bitpattern_108 => r_particle_container(108),
      bitpattern_109 => r_particle_container(109),
      bitpattern_110 => r_particle_container(110),
      bitpattern_111 => r_particle_container(111),
      bitpattern_112 => r_particle_container(112),
      bitpattern_113 => r_particle_container(113),
      bitpattern_114 => r_particle_container(114),
      bitpattern_115 => r_particle_container(115),
      bitpattern_116 => r_particle_container(116),
      bitpattern_117 => r_particle_container(117),
      bitpattern_118 => r_particle_container(118),
      bitpattern_119 => r_particle_container(119),
      bitpattern_120 => r_particle_container(120),
      bitpattern_121 => r_particle_container(121),
      bitpattern_122 => r_particle_container(122),
      bitpattern_123 => r_particle_container(123),
      bitpattern_124 => r_particle_container(124),
      bitpattern_125 => r_particle_container(125),
      bitpattern_126 => r_particle_container(126),
      bitpattern_127 => r_particle_container(127),
      bigrand => r_big_rand);

  pf2serial : entity work.parallel2serial
    generic map(NITEMS  => 128, NSTREAM => 2)
    port map( ap_clk => clk_p,
              roll   => open,
              data_in  => r_particle_container,
              valid_in => (others => '1'),
              data_out  => d,
              valid_out => open,
              roll_out  => open);

end;
