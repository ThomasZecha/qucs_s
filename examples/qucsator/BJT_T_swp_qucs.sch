<Qucs Schematic 24.3.99>
<Properties>
  <View=-167,-51,979,578,1.22452,0,0>
  <Grid=10,10,1>
  <DataSet=BJT_T_swp_qucs.dat>
  <DataDisplay=BJT_T_swp_qucs.dpl>
  <OpenDisplay=0>
  <Script=BJT_T_swp_qucs.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=Title>
  <FrameText1=Drawn By:>
  <FrameText2=Date:>
  <FrameText3=Revision:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <.DC DC1 1 40 340 0 38 0 0 "26.85" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "no" 0 "150" 0 "no" 0 "none" 0 "CroutLU" 0>
  <.SW SW1 1 220 340 0 64 0 0 "DC1" 1 "lin" 1 "TEMP" 1 "-40" 1 "85" 1 "20" 1>
  <R R2 1 140 140 15 -26 0 1 "1k" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <IProbe Pr1 1 220 90 -12 -58 1 2>
  <GND * 1 140 260 0 0 0 0>
  <Vdc V1 1 280 140 18 -26 0 1 "12 V" 1>
  <GND * 1 280 180 0 0 0 0>
  <R R1 1 70 140 -77 -21 0 1 "300k" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <_BJT Q2N2222A_1 1 140 220 8 -26 0 0 "npn" 0 "8.11e-14" 0 "1" 0 "1" 0 "0.5" 0 "0.225" 0 "113" 0 "24" 0 "1.06e-11" 0 "2" 0 "0" 0 "2" 0 "205" 1 "4" 0 "0" 0 "0" 0 "0.137" 0 "0.343" 0 "1.37" 0 "2.95e-11" 0 "0.75" 0 "0.33" 0 "1.52e-11" 0 "0.75" 0 "0.33" 0 "1" 0 "0" 0 "0.75" 0 "0" 0 "0.5" 0 "3.97e-10" 0 "0" 0 "0" 0 "0" 0 "8.5e-08" 0 "TEMP" 1 "0" 0 "1" 0 "1" 0 "0" 0 "1" 0 "1" 0 "0" 0 "1.5" 0 "3" 0 "1.11" 0 "26.85" 1 "1" 0 "no" 0>
</Components>
<Wires>
  <140 90 140 110 "" 0 0 0 "">
  <140 90 190 90 "" 0 0 0 "">
  <140 170 140 190 "" 0 0 0 "">
  <140 250 140 260 "" 0 0 0 "">
  <250 90 280 90 "" 0 0 0 "">
  <280 90 280 110 "" 0 0 0 "">
  <280 170 280 180 "" 0 0 0 "">
  <70 90 140 90 "" 0 0 0 "">
  <70 90 70 110 "" 0 0 0 "">
  <70 220 110 220 "" 0 0 0 "">
  <70 170 70 220 "" 0 0 0 "">
  <140 190 140 190 "Vce" 160 160 0 "">
</Wires>
<Diagrams>
  <Rect 430 377 387 297 3 #c0c0c0 1 00 1 -40 20 85 0 0 2 10 0 0 0.002 0.01 315 0 225 1 0 0 "" "" "">
	<"Vce.V" #0000ff 0 3 0 0 0>
	<"Pr1.I" #ff0000 0 3 0 0 1>
  </Rect>
</Diagrams>
<Paintings>
  <Text 20 -10 12 #082dff 0 "Temp Sweep Qucsator">
</Paintings>
