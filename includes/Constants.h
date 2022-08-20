/* 
 * File:   Constants.h
 * Author: gsat20
 *
 * Created on July 22, 2022, 4:22 PM
 */

#ifndef CONSTANTS_H
#define	CONSTANTS_H

class Global_Constants {
public:
    Global_Constants();
    Global_Constants(const Global_Constants& orig);
    virtual ~Global_Constants();
    
        
    static const int MSG_SERVER_WRITE_PORT=4040;
    static const int IQ_SHARED_MEMORY_KEY=1060;
    static const int CIIA_SHARED_MEMORY_KEY=1062;
    static const int CREATE_SHARED_MEMORY=1;
    static const int ATTACH_SHARED_MEMORY=2;
    static int const Create_Shared_Memory = 1;
    static int const Attach_Shared_Memory = 2;
    static int const Create_Attach_SM_Success = 3;
    static int const Create_Attach_SM_Failure = 4;
    static int const Remove_Detach_SM_Success = 5;
    static int const Remove_Detach_SM_Failure = 6;
    static int const SM_Wrt_Success = 7;
    static int const SM_Wrt_Failure = 8;
    static int const BIN_FILE_READ_ERROR=9;
    static int const MAXERRMSGLEN=100; 
    static int const MAXTIMEFMTLEN=100; 
    static int const LUT_CDB_PROC_TYPE = 92; 
    static int const EXP_CDB_PROC_TYPE = 91; 
    static const char ERR_STR_INVALIDPID[MAXERRMSGLEN];
    static const char ERR_STR_INVALIDINDEX[MAXERRMSGLEN];
    static const char ERR_STR_LMTNOTDEFINED[MAXERRMSGLEN];
    static const char ERR_STR_LMTEXPNOTDEFINED[MAXERRMSGLEN];
    static const char ERR_STR_NOT_CONDN[MAXERRMSGLEN];
    static const char ERR_STR_INVALID_CONDN_INDEX[MAXERRMSGLEN];
    static const char ERR_STR_INVALID_TRTHTBL_INDEX[MAXERRMSGLEN];

    
    

    
    

};

#endif	/* CONSTANTS_H */

