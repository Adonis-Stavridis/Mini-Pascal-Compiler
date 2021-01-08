#ifndef __TYPES_H__
#define __TYPES_H__

/**
 * @brief ScalpaType type
 * 
 */
typedef enum ScalpaType
{
	UNIT_TYPE = 0, /*!< unit type */
	INT_TYPE = 1,	 /*!< int type */
	BOOL_TYPE = 2, /*!< bool type */
	STR_TYPE = 3,	 /*!< string type */
	ARR_TYPE = 4	 /*!< array type */
} ScalpaType;

#endif /* __TYPES_H__ */
