# TODO

1. PID Index to be used for accessing shared memory location of a Parameter
1. New Index to be derived from PID Index. Beacuse of high index number for CTM, BTM.
But the total number of parameters present in this format may be less than 500.
Deriving a new index helps in reducing the size of shared memory.


# DB Issues
1. getNumPidsInCurFrm int initializing with 0
2. ATC parameters no of digital messages greater than 50
3. PLC parameters invalid characters in digital messages
4. MAG_BASED_RATE_ROL		EqnOrder = 79
5. For some parameters constant value is populated with some high value;

CAL_POS_VOL_SEL_RT
0		5.39199e+67
1		0.039063


BAT_CHRG_CUR_TCR_NSEL_RT
0		5.39199e+67
1		0.09274

6. what is display format??

7. words not populated properly for channel information. same word is repeating multiple times

8. using pidsincurframe api function, frame_id is coming only in even frame numbers ie, 0,2,4,...30

9. SSR-2_SW_SCRUB_PTR variable address not populated properly
