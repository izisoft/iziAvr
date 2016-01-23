#ifndef IZIUTILS_H_
#define IZIUTILS_H_

#include <driver/iziPlatform.h>

#ifdef __cplusplus
extern "C" {
#endif

#define iziSafeAccessor(_type,_varName)		\
	_type get##_varName( void ) 			\
	{										\
		_type tmpVal;						\
		IZI_ENTER_CRITICAL();				\
		tmpVal = (_type)(_varName);			\
		IZI_EXIT_CRITICAL();				\
		return tmpVal;						\
	}

#define iziSafeAccessorDecl(_type,_varName)	\
	_tpye get##_varName( void );

#define iziSafeMutator(_type,_varName)		\
	void set##_varName(_type newVal)		\
	{										\
		IZI_ENTER_CRITICAL();				\
		_varName = (_type)(_newVal);		\
		IZI_EXIT_CRITICAL();				\
	}

#define iziSafeMutatorDecl(_type,_varName)	\
	void set##_varName( _type );

#ifdef __cplusplus
}
#endif

#endif /* IZIUTILS_H_ */
