/* 
 * File:   TMDBReader.h
 * Author: saranya
 *
 * Created on 25 July, 2022, 11:54 AM
 */

#ifndef TMDBREADER_H
#define	TMDBREADER_H

#include "Headers.h"
#include "Constants.h"
#include "SharedMemoryTemplate.h"
#include "TMShmStruct.h"


class TMDBReader{
public:
    static int shMemKey;
    static SharedMemoryTemplate <class globalTMDBInfo> shm_TMDB;      
    static globalTMDBInfo* glbTMDB_ptr ;
    FILE *fptr;
    char *binFileName;  
    
    
    TMDBReader();
    //TMDBReader(char binfilename[]);
    
    virtual ~TMDBReader(); 
    
    
    int attachToSM();       
    bool readFromBinFile();
    void printTMPar(TMParameter tmPars);
    void setBinFileName(char* fileName);
    bool init(char * satID, short mode); //returns whether the initialisation process is success or not. Mode represents whether to read from Binary file (mode=1) or from shared memory(mode=0). 
     
};



#endif	/* TMDBREADER_H */

