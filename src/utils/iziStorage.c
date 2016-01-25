#include <stdio.h>
#include <string.h>

#include <izi/avr/core/kernel.h>
#include <izi/avr/core/mutex.h>
#include <izi/avr/core/memory.h>
#include <izi/avr/driver/eeprom.h>
#include <izi/avr/utils/crc.h>
#include <izi/avr/utils/storage.h>

#include <device/iziDevicePriv.h>

extern uint8_t iziEepromWriteNative(IziEepromAddr_t position, const char *data, uint8_t dataCount);
extern uint8_t iziEepromReadNative(IziEepromAddr_t position, char *data, uint8_t dataCount);

typedef struct SIziStorageElement TIziStorageElement;
struct SIziStorageElement {
	IziEepromAddr_t position_;
	void *ptr_;
};

static TIziStorageElement *iziStorage = NULL;
static uint8_t iziStorageSize = 0;
TIziMutex iziStorageMutex;

//------------------------------------------------------------------------------------------
static void iziStorageUpdateTable()
{
	IziCrc_t storageCrc = iziCrcCalculate((const char*)(iziStorage),iziStorageSize*sizeof(TIziStorageElement));
	iziEepromWriteNative(IZI_EE_STORAGE_START,(char*)iziStorage,iziStorageSize*sizeof(TIziStorageElement));
	iziEepromWriteNative(IZI_EE_STORAGE_START+iziStorageSize*sizeof(TIziStorageElement),(char*)(&storageCrc),sizeof(IziCrc_t));
}
//------------------------------------------------------------------------------------------
void iziStorageInit(uint8_t storageSize)
{
	if(!iziKernelCheckState(eIziStorageInit))
	{
		IziCrc_t storageCrc,readCrc;
		TIziStorageElement* storagePtr = iziMalloc(storageSize * sizeof(TIziStorageElement));
		iziMutexCreate(&iziStorageMutex);
		iziEepromInit();
		// Allocate memory for iziStorage table
		iziStorage = storagePtr;
		iziStorageSize = storageSize;
		// Load iziStorage from eeprom
		iziEepromReadNative(IZI_EE_STORAGE_START,(char*)iziStorage,storageSize*sizeof(TIziStorageElement));
		iziEepromReadNative(IZI_EE_STORAGE_START+storageSize*sizeof(TIziStorageElement),(char*)(&readCrc),sizeof(IziCrc_t));
		storageCrc = iziCrcCalculate((const char*)(iziStorage),storageSize*sizeof(TIziStorageElement));
		if(storageCrc != readCrc) { // There was not proper storage table in Eeprom
			while(storageSize--){
				storagePtr->position_ = 0;
				storagePtr->ptr_ = NULL;
				storagePtr++;
			}
			iziStorage->position_ = IZI_EE_STORAGE_START+iziStorageSize*sizeof(TIziStorageElement)+sizeof(IziCrc_t);
			iziStorageUpdateTable();
		}
		iziKernelSetState(eIziStorageInit);
	}
}
//------------------------------------------------------------------------------------------
IziBool_t iziStorageFetch(void* data,size_t dataSize)
{
	void *tmpPtr = iziMalloc(dataSize + sizeof(IziCrc_t));
	uint8_t i=0;
	
	iziMutexTake(&iziStorageMutex,IZI_MAX_DELAY);
	// Check if data has already been synchronized with iziStorage
	for(;(i < iziStorageSize) && (iziStorage[i].ptr_ != data) && (iziStorage[i].ptr_ != NULL); i++);
	// Data has been found in iziStorage
	if(iziStorage[i].ptr_ == data) {
		iziEepromReadNative(iziStorage[i].position_,(char*)tmpPtr,dataSize + sizeof(IziCrc_t));
		if(iziCrcCalculate((const char*)(tmpPtr),dataSize) == (IziCrc_t)(*((char*)tmpPtr+dataSize))) {
			memcpy(data,tmpPtr,dataSize);
		} else {
			memcpy(tmpPtr,data,dataSize);
			*((IziCrc_t*)((char*)(tmpPtr+dataSize))) = iziCrcCalculate(data,dataSize);
			iziEepromWriteNative(iziStorage[i].position_,(char*)tmpPtr,dataSize + sizeof(IziCrc_t));
		}
	// First synchronization
	} else if(i < iziStorageSize){
		iziStorage[i].ptr_ = data;
		iziEepromWriteNative(iziStorage[i].position_,(char*)data,dataSize);
		*((IziCrc_t*)tmpPtr) =  iziCrcCalculate(data,dataSize);
		iziEepromWriteNative(iziStorage[i].position_+dataSize,(char*)tmpPtr,sizeof(IziCrc_t));
		if(i < iziStorageSize - 1) {
			iziStorage[i+1].position_ = iziStorage[i].position_+dataSize+sizeof(IziCrc_t);
		}
		iziStorageUpdateTable();
	// No more place in storage
	} else {
		iziMutexGive(&iziStorageMutex);
		iziFree(tmpPtr);
		return IziFalse;
		
	}
	iziMutexGive(&iziStorageMutex);
	iziFree(tmpPtr);
	return IziTrue;
}
//------------------------------------------------------------------------------------------
IziBool_t iziStorageUpdate(const void* data,size_t dataSize)
{
	uint8_t i=0;
	IziCrc_t newCrc;
	iziMutexTake(&iziStorageMutex,IZI_MAX_DELAY);
	// Check if data has already been synchronized with iziStorage
	for(;(i < iziStorageSize) && (iziStorage[i].ptr_ != data) && (iziStorage[i].ptr_ != NULL); i++);
	// Data has been found in iziStorage
	if(iziStorage[i].ptr_ == data) {
		newCrc = iziCrcCalculate(data,dataSize);
		iziEepromWriteNative(iziStorage[i].position_,(char*)data,dataSize);
		iziEepromWriteNative(iziStorage[i].position_+dataSize,(char*)(&newCrc),sizeof(IziCrc_t));
	// Data not found in storage
	} else {
		iziMutexGive(&iziStorageMutex);
		return IziFalse;
	}
	iziMutexGive(&iziStorageMutex);
	return IziTrue;
}
