/**
 *	@filename :
 * 		lib_so.h
 * 
 *  @note :
 * 		
 **/
#ifndef __LIB_SO_H__
#define __LIB_SO_H__

#define HAL_MODULE_INFO_SYM			HMI
#define HAL_MODULE_INFO_SYM_STR		"HMI"

typedef struct {
	const char *tagname;
	unsigned short ver_major;
	unsigned short ver_minor;
	
	int (*open)(void *ptr);
}hal_module_info_t;

#endif //__LIB_SO_H__
 
