#ifndef IZISTORAGE_H_
#define IZISTORAGE_H_

#include <iziAvr.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \addtogroup iziUtils
 * @{
 */

/** \file iziStorage.h
*/

/** \fn iziStorageInit(uint8_t storageSize)
 * Function used for storage module initialization. It must be called before
 * first storage operation, but after kernel startup. Recommended place for
 * calling this function is iziKernelStartupHook.
 * \param storageSize
 * Capacity of storage. Defines number of parameters that can be kept in storage.
*/
void iziStorageInit(uint8_t storageSize);

/** \fn iziStorageFetch(void* data,size_t dataSize)
 * \param data
 * \param dataSize
 * \return
 * Function return eIziTrue if fetch operation succeeds. When data counterpart was
 * not found in storage, and there is no more place to add next persistent parameter
 * eIziFalse is returned.
 */
IziBool_t iziStorageFetch(void* data,size_t dataSize);

/** \fn iziStorageUpdate(const void* data,size_t dataSize)
 * \param data
 * \param dataSize
 * \return
 * Function returns eIziTrue if update operation succeeds, or eIziFalse if proper
 * parameter was not found in storage.
 */
IziBool_t iziStorageUpdate(const void* data,size_t dataSize);

/// @}

#ifdef __cplusplus
}
#endif

#endif //IZICONSOLE_H
