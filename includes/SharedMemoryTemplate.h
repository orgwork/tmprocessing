/* 
 * File:   SharedMemoryTemplate.h
 * Author: gsat20
 *
 * Created on July 22, 2022, 4:48 PM
 */

#ifndef SHAREDMEMORYTEMPLATE_H
#define	SHAREDMEMORYTEMPLATE_H

#include <string>
#include <iostream>
using namespace std;
template <class class_type> 


class SharedMemoryTemplate {
public:
    SharedMemoryTemplate();
    SharedMemoryTemplate(const SharedMemoryTemplate& orig);
    SharedMemoryTemplate(class_type *(*pointer_object), int shared_mem_key, string shared_mem_name,int create_or_attach); 
    virtual ~SharedMemoryTemplate();
    int createOrAttachSharedMemorySegment(class_type **pointer_object, int shared_mem_key,string shared_mem_name,int create_or_attach);
    int removeSharedMemorySegment(class_type *(*pointer_object),int shared_mem_key,string shared_mem_name,int create_or_attach);
private:

};

#endif	/* SHAREDMEMORYTEMPLATE_H */

