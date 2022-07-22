/*
 * File:   TMShmStruct.h
 * Author: saranya
 *
 * Created on 23 June, 2022, 10:00 AM
 */

#ifndef TMSHMSTRUCT_H
#define	TMSHMSTRUCT_H

#include <string.h>
using namespace std;

#define MAXPID 20000
#define MAXDEPPID 20
#define MAXSETS 4
#define MAXSAMPLES 15
#define MAXCONDN 8
#define MAXSTREAMS 8
#define MAXCOMBINATION 256
#define MAXEQNORDER 10
#define MAXFRAMECOUNT 64
#define MAXFRMPID  2000
#define MAXPIDLEN 10
#define MAXMNELEN 32
#define MAXDSCLEN 100
#define MAXDWLADRLEN 5
#define MAXEXPLEN  200
#define MAXTMESTAMPLEN 26
#define MAXSUBSYSLEN 6
#define MAXSATIDLEN 8
#define MAXUNITLEN 8
#define MAXSCOPELEN 10


class TMParameter{

public:
    char subSystem[MAXSUBSYSLEN] ; // direct
    char CDBPid[MAXPIDLEN]; // direct
    char mnemonic[MAXMNELEN];  // direct
    char description[MAXDSCLEN]; //direct
    unsigned short type;  // direct 1-A, 2-B, 3-D
    unsigned short processingType;  // direct
    unsigned short dataType; // derived from processing Type 
            /* 1-Normal 2-1's complement 3-2's complement 
            4-Graycode 5-float_ieee_32 6-float_ieee_64 7-float_1750a_32 8-float_1750a_48 */
    unsigned short pidNature ; //derived from processingType /* 1 Analog 0 Status -  */
    bool VCID[MAXSTREAMS]; // direct. The list of VCIDs for the spacecraft will 
            /*be available in spacecraft configuration. For Eg, MC1, MC2 and MC3
             If a paremeter is applicable for only MC1 and MC3 then, this array will contain VCID[0]=1,VCID[1]=0, VCID[2]=1*/
    
    
    
    struct  channelInfo{
        unsigned short   noOfChannels ;  //derived
        unsigned short   samplingRate[MAXSAMPLES] ; // direct 
        unsigned short   word[MAXSAMPLES] ; // direct
        unsigned short   frameId[MAXSAMPLES] ;
        unsigned short   startBit[MAXSAMPLES] ; // direct
        unsigned short   endBit[MAXSAMPLES] ; // direct
        char    dwellAddr[MAXSAMPLES][MAXDWLADRLEN]; // direct       
        //unsigned short   updateFrmNo; // derived
    }chnInfo_S[MAXSETS];

    unsigned short noOfSets ;   // derived /*default noOfSets is 1 and default set number is 0 */
    unsigned short noOfConditions ; //derived /* 0 no condition 1 one condition... max 8*/
    unsigned short conditionType ; //dervied /* 0-Just Process,1-Source,2-Coeffs, 3-Source+Coeffs */
    

    
    struct digitalInfo
    {
        unsigned char  patternKey[MAXCOMBINATION]; //direct 
                    /* contains number in the corresponding index location. 
                     * For a parameter with 2 bit status info patternKey[0]=0, patternKey[1]=1, patternKey[2]=2
                     * patternKey[3]=3 */
        char         digStsMsg[MAXCOMBINATION][MAXPIDLEN]; //direct 
                    /* Index location corresponding to the patternKey number will contain the corresponding sts msg
                     * For a parameter with 2 bit status info, digStsMsg[0]=BothOFF, digStsMsg[1]=Sys1-ON, digStsMsg[2]=Sys2-ON
                     * digStsMsg[3]=BothON*/
                     
        unsigned short noOfDigitalSts; //derived For above eg, noOfDigitalSts = 4
    } digital_S[MAXSETS]; 

    struct limitsInfo{
         float lowerLimit; //direct
         float upperLimit; //direct
         float tolerance; //direct
    };
 
    union  limits{
        struct simple{
            struct limitsInfo qualLimit;
        };

        struct conditional{
            struct limitsInfo qualLimit;
            struct limitsInfo storageLimit;
            char expressionQualLmt[MAXEXPLEN]; //direct
            char expressionStrgLmt[MAXEXPLEN]; //direct
        };

        struct none{
             float tolerance; //direct
        };


    };

    struct equationInfo
    {
        /* c + a0*TMC + a1*TMC^2 + a2*TMC^3+........... */
        float        acoeff[MAXEQNORDER]; //direct a0,a1...
        float        offset; //direct  c in the above equation 
        unsigned short         eqnOrder; //derived  // changed char to short
        limits       limit; 
    } equation_S[MAXSETS];

    struct lutInfo
    {
        char        lutTblName[MAXPIDLEN]; //direct
        limits      limit;

    }lutInfo_S[MAXSETS];

    
    struct expressionInfo
    {
        // AEXP processing type 91
        char expression[MAXEXPLEN]; //derived
        limits   limit;
    }exp_S[MAXSETS];

    union
    {	
        struct digitalInfo digital_S;
        struct equationInfo equation_S; 
        struct lutInfo lutInfo_S;
        struct expressionInfo exp_S;
    } procScheme_S;

    //unsigned short procScheme; //1-digital_S, 2-equation_S, 3-lutInfo_S, 4-exp_S
    
    
    struct conditionInfo
    {
       char 	CDBPid[MAXPIDLEN]; //direct
       char     condnOperator; //direct != , == , > , >= , <  , <=    
       union pidvalue
       {
        double	Fvalue; //direct
	char	Cvalue[MAXPIDLEN] ; //direct
	} PidValue ;
    } condnInfo_S[MAXCONDN];  //MAXCONDN ranges from 0 to 7
    
    /*********************************************************************
	* Pattern is to represent outcome of 8 conditions
    	* Each of the value in pattern represents one out of
    	* 8 conditions i.e. if Parameter has 2 conditions, so
    	* Probable pattern possible are 00, 01, 10, 11 where,
    	* 00 represents both conditions results are false
    	* 01 represents condition-1 is false and condition-2 is true
    	* 10 represents condition-1 is true and condition-2 is false
    	* 11 represents both condition-1 and 2 are true
        *
    	* Set Number represent what needs to be done for a pattern
    	* -1: BLANK, -2: RETAIN, 0-3: SetNumber ; Default is -99 for No Condition
    	* Set Number Starts from 0
	    * 
		* For 8 Conditions total number of Truth Table Entries
        * possible is 256
        * The number is arived with 2^8=256 Outcomes for 8 Conditions
        ***********************************************************************/

    struct truthTable
    {
            char Pattern[MAXCONDN]; // contains the number corresponding to the condition pattern Eg, 0, 1 to 255
            char chnSetNumber;
            char eqnSetNumber;
            /* Index location corresponding to the pattern condition will contain the corresponding set number
             * Other undefined patterns will contain the default value (-1: BLANK, -2: RETAIN) from CDB */
    } truthTable_S[MAXCOMBINATION] ;



    char  unit[MAXUNITLEN]; //direct
    char  scope[MAXSCOPELEN]; //direct 
    char  authenticationStage[MAXSCOPELEN]; //direct
    unsigned short limit_type; //direct /*0-none 1-simple, 2-conditional*/
    bool testabilityInCkout; //direct    
    char  displayFormat[MAXPIDLEN] ; // direct
    unsigned short pidFmtId ; // direct	/* 1 NorTm 2 CTM 3 BTM 4 F2PB 5 F3PB 6 SPS .. */
    
    unsigned short noOfDepPids; //derived
    char   dependentPidlist[MAXDEPPID][MAXPIDLEN] ;  //derived
    unsigned int     pidIndex; // index in Binary file
    
    char   modified_time[MAXTMESTAMPLEN]; // CDB Modification time 
    
//API 

    char* getPIDSubsystem(char* PID); //returns the subsystem of the given PID
    char* getPIDMnemonic(char* PID); //returns the mnemonic of the given PID
    char* getPIDDescription(char* PID); //returns the description of the given PID
    unsigned short getPIDType(char* PID); //returns the type of the given PID
    unsigned short getPIDProcType(char* PID); //returns the processing type of the given PID
    unsigned short getPIDDataType(char* PID); //returns the data type of the given PID
    unsigned short getPIDNature(char* PID); //returns the nature of the given PID
    bool getPIDVcids(char* PID, int VCID_Index); //returns whether the PID is applicable for the given VCID index   
    unsigned short getPIDNoOfSets(char* PID); //returns the num of sets for the given PID
    
    
    //channel information
    unsigned short getPIDNoOfChn(char* PID, unsigned short setNo); // returns the number of channels for the given PID and set number
    unsigned short* getPIDSamplingRate(char* PID, unsigned short setNo); // returns the list of sampling rate for the given PID and set number
    unsigned short* getPIDListOfWords(char* PID, unsigned short setNo); // returns the list of word numbers for the given PID and set number
    unsigned short* getPIDListOfFrames(char* PID, unsigned short setNo); // returns the list of frames numbers for the given PID and set number
    unsigned short* getPIDListOfStartBits(char* PID, unsigned short setNo); // returns the list of start bit numbers for the given PID and set number
    unsigned short* getPIDListOfEndBits(char* PID, unsigned short setNo); // returns the list of end bit numbers for the given PID and set number
    char ** getPIDListOfDwlAddr(char* PID, unsigned short setNo); // returns the list of dwell addresses for the given PID and set number
    
    
    //unsigned short getPIDProcScheme(char* PID); //returns the processing scheme of the given PID
    
    unsigned short getPIDNoOfDigMsg(char* PID, char setNo); //returns the number of digital messages for the given PID and set number
    char ** getPIDListOfDigMsg(char* PID, char setNo); //returns the list of digital messages defined for the given PID and set number
    char ** getPIDListOfDigPtnKeys(char* PID, char setNo); //returns the list of digital pattern keys defined for the given PID and set number
    char *  getPIDDigMsg(char* PID, char setNo, short index); //returns the digital message defined in the given index location for the PID and set number
    
    
    unsigned short getPIDEqnOrder(char* PID, unsigned short setNo); //returns the equation order foe the given PID and set number
    float* getPIDEqnCoeffOffset(char* PID, unsigned short setNo); //returns the list of equation coefficients in the form c, a0, a1 etc..
    
    char* getPIDLutTblName(char* PID, unsigned short setNo); //returns the lookup table name for the given PID and set number
    
    char* getPIDExp(char* PID, unsigned short setNo); //returns the expression for the given PID and set number
    
    char* getPIDUnit(char* PID); //returns the unit of the given PID
    char* getPIDScope(char* PID); //returns the scope of the given PID
    char* getPIDAuthStage(char* PID); //returns the scope of the given PID
    
    //limit details
    unsigned short getPIDLimitType(char* PID); // returns the limit type of the given PID
    float getPIDUpperLimit(char* PID); //returns the upper limit of the given PID for SIMPLE type
    float getPIDLowerLimit(char* PID); //returns the lower limit of the given PID for SIMPLE type
    float getPIDQualUpperLimit(char* PID); //returns the qualification upper limit of the given PID for CONDITIONAL TYPE
    float getPIDQualLowerLimit(char* PID); //returns the qualification lower limit of the given PID for CONDITIONAL TYPE
    float getPIDStrgUpperLimit(char* PID); //returns the storage upper limit of the given PID for CONDITIONAL TYPE
    float getPIDStrgLowerLimit(char* PID); //returns the storgae lower limit of the given PID for CONDITIONAL TYPE
    float getPIDTolerance(char* PID); //returns the tolerance of the given PID
    char* getPIDExpQualLimit(char* PID); //retunrs the qualification limit expression of the given PID
    char* getPIDExpStrgLimit(char* PID); //retunrs the storage limit expression of the given PID 
    
    bool getPIDTestability(char* PID); // returns the testability of the given PID
    char* getPIDDisplayFormat(char* PID); //returns the display format of the given PID
    unsigned short getPIDFormatId(char* PID); //returns the format type of the given PID
    
    unsigned short getPIDNoOfDepPids(char* PID); //returns the number of dependent PIDs for the given PID
    char** getPIDListOfDepPids(char* PID); //returns the list of dependent PIDs for the given PID
    
    unsigned short getPIDIndex(char* PID); //returns the index of the given PID
    char* getPIDModifiedTime(char* PID); //returns the last CDB modified time of the given PID
    
      
    //condition related
    unsigned short getPIDNoOfCondn(char* PID); //retuns the number of conditions defined for the given PID
    unsigned short getPIDCondnType(char* PID); //returns the condition type of the given PID
    char* getPIDCondnSrcPID(char* PID, unsigned short condnNo); //returns the condition source PID for the given PID and condition number
    char* getPIDCondnOptr(char* PID, unsigned short condnNo); //returns the conditional operator for the given PID and condition number
    unsigned short getCondnSrcPIDValueType(char* PID, unsigned short condnNo); //returns the conditional value type for the given PID and condition number
    float getPIDCondnAnalogVal(char* PID, unsigned short condnNo); //returns the conditional expression analog value for the given PID and condition number
    char* getPIDCondnDigValue(char* PID, unsigned short condnNo); //returns the conditional expression string value for the given PID and condition number
    
    char getPIDCondnSrcInfo(char* PID, int index); //returns the channel set number for the given PID and index location
    char getPIDCondnEqnInfo(char* PID, int index); //returns eqn set number for the given PID and index location
    char  getPIDCondnPattern(char* PID, unsigned short truthTblIndex); //returns the truth table pattern number for the given PID and truth table index 
    char  getPIDCondnSrcSetNo(char* PID, unsigned short truthTblIndex); //returns the channel set number for the given PID and truth table index 
    char  getPIDCondnEqnSetNo(char* PID, unsigned short truthTblIndex); //returns the equation set number for the given PID and truth table index 
    
    
    bool IsAnalogPar(char* PID); //returns true if the Parameter is of Analog type
    bool IsDigitalPar(char* PID); //returns true if the Parameter is of Digital type
    bool IsConditionalPar(char* PID); //returns true if the given PID is of conditional type
 
    
    
    char* getPIDFromMnemonic(char* mnemonic); //retunrs the PID corresponding to the given Mnemonic
    char** getPIDFromDwellAdrr(char* PID); //returns the list of PIDs corresponding to the given dwell address
    char** getPIDChannelInfo(char* PID); //returns the channel information (wordNo,FrameNo and bit numbers) for the given PID
    
    
};

class globalTMDBInfo{
    
public:
    
    TMParameter tmPars[MAXPID];
    unsigned int noOfTmPids;
    char    PidDefinedInCurrentFrame[MAXFRAMECOUNT][MAXFRMPID][MAXPIDLEN] ; 
    unsigned int     NoOfPidsInCurrentFrame[MAXFRAMECOUNT] ;
    char    CDBDatabaseVersion[MAXTMESTAMPLEN]; /// Version from CDB
    char    databaseGenTime[MAXTMESTAMPLEN]; //Binary file generation time
    char    satTMID[MAXSATIDLEN]; //contains satellite ID EG;EOS-05
    
//API 
    unsigned int getTotalNumPids(); // returns the total number of PIDs
    unsigned int getTotalNumPids(unsigned short pidFmtId); // returns the total number of PIDs corresponding to the given Format ID
    
    char** getPIDList(); //returns the list of CDBPIDs
    char** getPIDList(unsigned short pidFmtId); //returns the list of CDBPIDs corresponding to the given Format ID
    
    unsigned int getNumPidsInCurFrm(int FrameNo); //returns the number of PIDs defined for the given frame number
    unsigned int getNumPidsInCurFrm(int FrameNo,unsigned short pidFmtId); //returns the number of PIDs defined for the given frame number corresponding to the given Format ID
 
    char** getPidsInCurFrm(int FrameNo); //returns the PIDs defined for the given frame number
    char** getPidsInCurFrm(int FrameNo,unsigned short pidFmtId); //returns the PIDs defined for the given frame number corresponding to the given Format ID
    
    char* getCDBVersionNo(); //returns the CDB Version number
    char* getBinFilGenTime(); //returns the time at which binary file was generated, databaseGenTime
    char* getSatTMID(); //returns the Satellite TM ID

    TMParameter getPIDDetails(char* PID); // returns an object of type TMParameter populated will details corresponding to the given PID

    char* getListOfAnalogPars(); //returns the list of CDBPIDs of Analog Parameters
    char* getListOfAnalogPars(unsigned short pidFmtId); //returns the list of CDBPIDs of Analog Parameters  corresponding to the given Format ID
    char* getListOfDigitalPars(); //returns the list of CDBPIDs of Digital Parameters
    char* getListOfDigitalPars(unsigned short pidFmtId); //returns the list of CDBPIDs of Digital Parameters  corresponding to the given Format ID
    
    bool init(short mode); //returns whether the initialisation process is success or not. Mode represents whether to read from Binary file (mode=1) or from shared memory(mode=0). 
    
    
};



#endif	/* TMSHMSTRUCT_H */



