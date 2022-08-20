/*
 * File:   TMShmStruct.h
 * Author: saranya
 *
 * Created on 23 June, 2022, 10:00 AM
 */

#ifndef TMSHMSTRUCT_H
#define	TMSHMSTRUCT_H

#include "math.h"
#include "stdint.h"

#pragma pack(push,2)

#define MAXPID 20000
#define MAXSETS 4
#define MAXSAMPLES 15  
#define MAXCONDN 8 
#define MAXSTREAMS 16  // Eg: TM1,TM2
#define MAXTRTHTBLCOMB 256 
#define MAXDIGSTS 128 
#define MAXEQNORDER 10 // If more than 5, then to be defined in expression format
#define MAXFRAMECOUNT 64
#define MAXFRMPID  2000
#define MAXPIDLEN 10
#define MAXLUTTBLNAMLEN 15
#define MAXDIGSTSLEN 16
#define MAXMNELEN 31
#define MAXDSCLEN 100
#define MAXDWLADRLEN 8
#define MAXEXPLEN  200
#define MAXLMTEXPLEN  100
#define MAXSUBSYSLEN 6
#define MAXSATIDLEN 8
#define MAXVERNOLEN 10
#define MAXDIGSETS 5000
#define MAXANASETS 5000
#define MAXLUTSETS 500
#define MAXCONDNSETS 500
#define MAXTRTHTBLSETS 4000
#define MAXLMTSETS 5000

#define MAXUNITLEN 10
#define MAXUNITSETS 25
#define MAXDISFRMTLEN 10
#define MAXDISFRMTSETS 25
#define MAXSCPEAUTHLEN 10
#define MAXSCPEAUTHSETS 6

//binary file to contain the following 
//list of display formats 
//list of dig msg
//list of coefficients
//list of cond info 

#define MAXOPTRLEN 3


    struct digitalInfo
    {
        uint8_t  patternKey[MAXDIGSTS]; //direct 
                    /* contains number in the corresponding index location. 
                     * For a parameter with 2 bit status info patternKey[0]=0, patternKey[1]=1, patternKey[2]=2
                     * patternKey[3]=3 */
        uint8_t  digStsMsg[MAXDIGSTS][MAXDIGSTSLEN]; //direct 
                    /* Index location corresponding to the patternKey number will contain the corresponding sts msg
                     * For a parameter with 2 bit status info, digStsMsg[0]=BothOFF, digStsMsg[1]=Sys1-ON, digStsMsg[2]=Sys2-ON
                     * digStsMsg[3]=BothON*/
                     
        uint8_t noOfDigitalSts; //derived For above eg, noOfDigitalSts = 4
    }; 
    
    struct equationInfo
    {
        /* c + a0*TMC + a1*TMC^2 + a2*TMC^3+........... */
        double_t        aCoeff[MAXEQNORDER]; //direct a0,a1...
        double_t        offset; //direct  c in the above equation 
        uint8_t       eqnOrder; //derived  // changed char to short        
    } ;

    struct lutInfo
    {
        uint8_t      lutTblName[MAXLUTTBLNAMLEN]; //direct        
    };

    
    struct expressionInfo
    {
        // AEXP processing type 91
        uint8_t expression[MAXEXPLEN]; //derived        
    };

    
    
    struct conditionInfo
    {
       uint8_t 	cdbPid[MAXPIDLEN]; //direct
       uint8_t     condnOperator[MAXOPTRLEN]; //direct != , == , > , >= , <  , <=    
       union pidvalue
       {
        double_t fValue; //direct
	uint8_t	cValue[MAXDIGSTSLEN] ; //direct
	} pidValue ;
    };  //MAXCONDN ranges from 0 to 7*/
    
    struct  channel{
        uint8_t    samplingRate; // convert from freq(sec) to rate (CDB API does the conversion)
        uint16_t   word; // direct
        uint16_t   frameId;
        uint16_t   startBit; // direct
        uint16_t   endBit ; // direct
        uint8_t    dwellAddr[MAXDWLADRLEN]; // direct       
        //unsigned short   updateFrmNo; // derived
    };
    
    struct  chnInfo{
        uint8_t  noOfChannels ;  //derived
        struct channel  channelInfo[MAXSAMPLES];       
    };
    
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
            uint8_t pattern; // contains the number corresponding to the condition pattern Eg, 0, 1 to 255
            int8_t chnSetNumber;
            int8_t eqnSetNumber;
            /* Index location corresponding to the pattern condition will contain the corresponding set number
             * Other undefined patterns will contain the default value (-1: BLANK, -2: RETAIN) from CDB */
    };
    
    struct limitsInfo{
         double_t lowerLimit; //direct
         double_t upperLimit; //direct
         double_t tolerance; //direct
    };
    
    struct condnLimitsInfo{
         struct limitsInfo condnLimits;
         uint8_t limitExpression[MAXLMTEXPLEN]; //direct
    };
 
    union  limits{
        struct simple{
            struct limitsInfo qualLimit;
        }simpleLmt;

        struct conditional{
            struct condnLimitsInfo qualLimit;
            struct condnLimitsInfo storageLimit;            
        }condnLmt;

        struct none{
             double_t tolerance; //direct
        }noneLmt;
    };
    
    union processSchInfo
    {	
        uint16_t digInfoIndexPtr;
        uint16_t coeffInfoIndexPtr;
        uint16_t lutInfoIndexPtr;
        uint16_t expInfoIndexPtr;
    };
    
    
class TMParameter{

public:
    uint8_t subSystem[MAXSUBSYSLEN] ; // direct
    uint8_t cdbPid[MAXPIDLEN]; // direct
    uint8_t mnemonic[MAXMNELEN];  // direct    
    uint8_t description[MAXDSCLEN]; //direct
    uint8_t type;  // direct 1-A, 2-B, 3-D
    uint8_t processingType;  // direct
    uint8_t dataType; // derived from processing Type 
            /* 1-Normal 2-1's complement 3-2's complement 
            4-Graycode 5-float_ieee_32 6-float_ieee_64 7-float_1750a_32 8-float_1750a_48 */
    uint8_t pidNature ; //derived from processingType /* 1 Analog 0 Status */ (CDB API does the conversion)
    uint16_t vcid; // direct. The list of VCIDs for the spacecraft will 
            /*be available in spacecraft configuration. For Eg, MC1, MC2 and MC3
             If a parameter is applicable for only MC1, this array will contain 1*/
    
    chnInfo chnInfo_S[MAXSETS];

    uint8_t noOfSets ;   // derived /*default noOfSets is 1 and default set number is 0 */
    uint8_t noOfConditions ; //derived /* 0 no condition 1 one condition... max 8*/
    uint8_t conditionType ; //dervied /* 0-Just Process,1-Source,2-Coeffs, 3-Source+Coeffs */
    
    int16_t limitInfoIndexPtr[MAXSETS];

    processSchInfo procScheme_S[MAXSETS];

    int16_t condnInfoIndexptr; 
    int16_t condnTrthTblIndexPtr; 
    
    uint8_t  unit; //direct
    uint8_t  scope; //direct 
    uint8_t  authenticationStage; //direct
    uint8_t limitType; //direct /*0-none 1-simple, 2-conditional*/
    uint8_t testabilityInCkout; //direct    
    uint8_t  displayFormat ; // direct
    uint8_t pidFmtId ; // direct	/* 1 NorTm 2 CTM 3 BTM 4 F2PB 5 F3PB 6 SPS .. */
    
	//Thro API
    //unsigned short noOfDepPids; //derived
    //char   dependentPidlist[MAXDEPPID][MAXPIDLEN] ;  //derived
    
    uint16_t   pidIndex; // index in Binary file Eg: 4 digit PID no /file index/CDB index
    
    uint64_t   modifiedTime; // CDB Modification time epoch in sec
    
    TMParameter();
   
    
};

class globalTMDBInfo{
    
public:

    TMParameter tmPars[MAXPID];
    uint16_t   noOfTmPids;
    uint8_t    pidDefinedInCurrentFrame[MAXFRAMECOUNT][MAXFRMPID][MAXPIDLEN] ; 
    uint16_t   noOfPidsInCurrentFrame[MAXFRAMECOUNT] ;
    uint8_t    CDBDatabaseVersion[MAXVERNOLEN]; /// Version from CDB
    uint64_t   databaseGenTime; //Binary file generation time 
    uint8_t    satTMID[MAXSATIDLEN]; //contains satellite ID EG;EOS-05
    
    
    digitalInfo   digital_S[MAXDIGSETS]; 
    
    equationInfo   equation_S[MAXANASETS];

    lutInfo    lutInfo_S[MAXLUTSETS];
    
    expressionInfo   exp_S[MAXANASETS];
    
    limits limit[MAXLMTSETS];
        
    conditionInfo condnInfo_S[MAXCONDNSETS][MAXCONDN];  //MAXCONDN ranges from 0 to 7*/
    
    truthTable truthTable_S[MAXTRTHTBLSETS][MAXTRTHTBLCOMB];
    
    uint8_t unitsInfo[MAXUNITSETS][MAXUNITLEN];
    
    uint8_t displayFormatInfo[MAXDISFRMTSETS][MAXDISFRMTLEN];
    
    uint8_t scopeAuthInfo[MAXSCPEAUTHSETS][MAXSCPEAUTHLEN];   
   
    
    globalTMDBInfo();
    
//API 
    uint16_t getTotalNumPids(bool * retSts, uint8_t * errMsg); // returns the total number of PIDs
    uint16_t getTotalNumPids(uint8_t pidFmtId, bool * retSts, uint8_t * errMsg); // returns the total number of PIDs corresponding to the given Format ID
    
    uint8_t** getPIDList(bool * retSts, uint8_t * errMsg); //returns the list of CDBPIDs
    uint8_t** getPIDList(uint8_t pidFmtId, bool * retSts, uint8_t * errMsg); //returns the list of CDBPIDs corresponding to the given Format ID
    
    uint16_t getNumPidsInCurFrm(uint16_t frameNo, bool * retSts, uint8_t * errMsg); //returns the number of PIDs defined for the given frame number
    uint16_t getNumPidsInCurFrm(uint16_t frameNo, uint8_t pidFmtId, bool * retSts, uint8_t * errMsg); //returns the number of PIDs defined for the given frame number corresponding to the given Format ID
 
    uint8_t** getPidsInCurFrm(uint16_t frameNo, bool * retSts, uint8_t * errMsg); //returns the PIDs defined for the given frame number
    uint8_t** getPidsInCurFrm(uint16_t frameNo, uint8_t pidFmtId, bool * retSts, uint8_t * errMsg); //returns the PIDs defined for the given frame number corresponding to the given Format ID
    
    uint8_t* getCDBVersionNo(bool * retSts, uint8_t * errMsg); //returns the CDB Version number
    uint8_t* getBinFilGenTime(bool * retSts, uint8_t * errMsg); //returns the time at which binary file was generated, databaseGenTime
    uint8_t* getSatTMID(bool * retSts, uint8_t * errMsg); //returns the Satellite TM ID

    TMParameter getPIDDetails(uint8_t* PID, bool * retSts, uint8_t * errMsg); // returns an object of type TMParameter populated will details corresponding to the given PID

    uint8_t** getListOfAnalogPars(bool * retSts, uint8_t * errMsg, uint16_t * noOfPids); //returns the list of CDBPIDs of Analog Parameters
    uint8_t** getListOfAnalogPars(uint8_t pidFmtId, bool * retSts, uint8_t * errMsg, uint16_t * noOfPids); //returns the list of CDBPIDs of Analog Parameters  corresponding to the given Format ID
    uint8_t** getListOfDigitalPars(bool * retSts, uint8_t * errMsg, uint16_t * noOfPids); //returns the list of CDBPIDs of Digital Parameters
    uint8_t** getListOfDigitalPars(uint8_t pidFmtId, bool * retSts, uint8_t * errMsg, uint16_t * noOfPids); //returns the list of CDBPIDs of Digital Parameters  corresponding to the given Format ID
    
    int searchPID(uint8_t *pid);
    //bool init(short mode); //returns whether the initialisation process is success or not. Mode represents whether to read from Binary file (mode=1) or from shared memory(mode=0). 
    
    //TMParameter related API functions
    
    uint8_t* getPIDSubsystem(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the subsystem of the given PID
    uint8_t* getPIDMnemonic(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the mnemonic of the given PID
    uint8_t* getPIDDescription(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the description of the given PID
    uint8_t getPIDType(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the type of the given PID
    uint8_t getPIDProcType(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the processing type of the given PID
    uint8_t getPIDDataType(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the data type of the given PID
    uint8_t getPIDNature(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the nature of the given PID
    uint8_t getPIDVcids(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns whether the VCID for the given PID 
    uint8_t getPIDNoOfSets(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the num of sets for the given PID
    
    
    //channel information
    uint8_t getPIDNoOfChn(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); // returns the number of channels for the given PID and set number
    uint16_t* getPIDSamplingRate(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); // returns the list of sampling rate for the given PID and set number
    uint16_t* getPIDListOfWords(uint8_t* PID, uint8_t  setNo, bool * retSts, uint8_t * errMsg); // returns the list of word numbers for the given PID and set number
    uint16_t* getPIDListOfFrames(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); // returns the list of frames numbers for the given PID and set number
    uint16_t* getPIDListOfStartBits(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); // returns the list of start bit numbers for the given PID and set number
    uint16_t* getPIDListOfEndBits(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); // returns the list of end bit numbers for the given PID and set number
    uint8_t ** getPIDListOfDwlAddr(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); // returns the list of dwell addresses for the given PID and set number
    
    
    //unsigned short getPIDProcScheme(char* PID); //returns the processing scheme of the given PID
    
    uint8_t getPIDNoOfDigMsg(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); //returns the number of digital messages for the given PID and set number
    uint8_t ** getPIDListOfDigMsg(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); //returns the list of digital messages defined for the given PID and set number
    uint8_t *  getPIDListOfDigPtnKeys(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); //returns the list of digital pattern keys defined for the given PID and set number
    uint8_t *  getPIDDigMsg(uint8_t* PID, uint8_t setNo, uint16_t index, bool * retSts, uint8_t * errMsg); //returns the digital message defined in the given index location for the PID and set number
    
    
    uint8_t getPIDEqnOrder(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); //returns the equation order foe the given PID and set number
    double_t* getPIDEqnCoeffOffset(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); //returns the list of equation coefficients in the form c, a0, a1 etc..
    
    uint8_t* getPIDLutTblName(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); //returns the lookup table name for the given PID and set number
    
    uint8_t* getPIDExp(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); //returns the expression for the given PID and set number
    
    uint8_t* getPIDUnit(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the unit of the given PID
    uint8_t* getPIDScope(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the scope of the given PID
    uint8_t* getPIDAuthStage(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the scope of the given PID
    
    //limit details
    uint8_t getPIDLimitType(uint8_t* PID, bool * retSts, uint8_t * errMsg); // returns the limit type of the given PID
    double_t getPIDUpperLimit(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); //returns the upper limit of the given PID for SIMPLE type
    double_t getPIDLowerLimit(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); //returns the lower limit of the given PID for SIMPLE type
    double_t getPIDQualUpperLimit(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); //returns the qualification upper limit of the given PID for CONDITIONAL TYPE
    double_t getPIDQualLowerLimit(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); //returns the qualification lower limit of the given PID for CONDITIONAL TYPE
    double_t getPIDStrgUpperLimit(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); //returns the storage upper limit of the given PID for CONDITIONAL TYPE
    double_t getPIDStrgLowerLimit(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); //returns the storage lower limit of the given PID for CONDITIONAL TYPE
    double_t getPIDTolerance(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); //returns the tolerance of the given PID
    uint8_t* getPIDExpQualLimit(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); //returns the qualification limit expression of the given PID
    uint8_t* getPIDExpStrgLimit(uint8_t* PID, uint8_t setNo, bool * retSts, uint8_t * errMsg); //returns the storage limit expression of the given PID 
    
    bool getPIDTestability(uint8_t* PID, bool * retSts, uint8_t * errMsg); // returns the testability of the given PID
    uint8_t* getPIDDisplayFormat(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the display format of the given PID
    uint8_t getPIDFormatId(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the format type of the given PID
    
    uint16_t getPIDNoOfDepPids(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the number of dependent PIDs for the given PID
    uint8_t** getPIDListOfDepPids(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the list of dependent PIDs for the given PID
    
    uint16_t getPIDIndex(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the index of the given PID
    uint8_t* getPIDModifiedTime(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the last CDB modified time of the given PID
    
      
    //condition related
    uint8_t getPIDNoOfCondn(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the number of conditions defined for the given PID
    uint8_t getPIDCondnType(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the condition type of the given PID
    uint8_t* getPIDCondnSrcPID(uint8_t* PID, uint8_t condnNo, bool * retSts, uint8_t * errMsg); //returns the condition source PID for the given PID and condition number
    uint8_t* getPIDCondnOptr(uint8_t* PID, uint8_t condnNo, bool * retSts, uint8_t * errMsg); //returns the conditional operator for the given PID and condition number
    uint16_t getCondnSrcPIDValueType(uint8_t* PID, uint8_t condnNo, bool * retSts, uint8_t * errMsg); //returns the conditional value type for the given PID and condition number
    double_t getPIDCondnAnalogVal(uint8_t* PID, uint8_t condnNo, bool * retSts, uint8_t * errMsg); //returns the conditional expression analog value for the given PID and condition number
    uint8_t* getPIDCondnDigValue(uint8_t* PID, uint8_t condnNo, bool * retSts, uint8_t * errMsg); //returns the conditional expression string value for the given PID and condition number
    
    //char getPIDCondnSrcInfo(char* PID, int index, bool * retSts, char * errMsg); //returns the channel set number for the given PID and index location
    //char getPIDCondnEqnInfo(char* PID, int index, bool * retSts, char * errMsg); //returns eqn set number for the given PID and index location
    uint8_t getPIDCondnPattern(uint8_t* PID, uint16_t truthTblIndex, bool * retSts, uint8_t * errMsg); //returns the truth table pattern number for the given PID and truth table index 
    uint8_t getPIDCondnSrcSetNo(uint8_t* PID, uint16_t truthTblIndex, bool * retSts, uint8_t * errMsg); //returns the channel set number for the given PID and truth table index 
    uint8_t getPIDCondnEqnSetNo(uint8_t* PID, uint16_t truthTblIndex, bool * retSts, uint8_t * errMsg); //returns the equation set number for the given PID and truth table index 
    
    
    bool IsAnalogPar(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns true if the Parameter is of Analog type
    bool IsDigitalPar(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns true if the Parameter is of Digital type
    bool IsExpressionPar(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns true if the Parameter is of Expression type
    bool IsLutPar(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns true if the Parameter is of LUT type
    bool IsConditionalPar(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns true if the given PID is of conditional type
 
    
    
    uint8_t* getPIDFromMnemonic(uint8_t* mnemonic, bool * retSts, uint8_t * errMsg); //returns the PID corresponding to the given Mnemonic
    uint8_t** getPIDFromDwellAdrr(uint8_t* dwlAddr, bool * retSts, uint8_t * errMsg, uint16_t * noOfPids); //returns the list of PIDs corresponding to the given dwell address
    uint8_t** getPIDChannelInfo(uint8_t* PID, bool * retSts, uint8_t * errMsg); //returns the channel information (wordNo,FrameNo and bit numbers) for the given PID
    
    
};

#pragma pack(pop)

#endif	/* TMSHMSTRUCT_H */











