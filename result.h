#ifndef RA_RESULT_H
#define	RA_RESULT_H

//--------------------  RA_RESULT.H  -------------------- 
//
//	Definition of class ra_result for a mean
//	value calculation
//
//--------------------    SE1650    --------------------

#include "ra_value.h"


class ra_result
{
  public:
    ra_result();
    void		setPointer(int resultNo, const char *pointer);
    pwr_tFloat32	getResult(int resultNo);
    void		addValue(const char *pointer);
    void		collectValues();
    void		calculateResult();
    void		clearResult();
    void		close();
    
  private:
    pwr_sClass_AArray100	*pointer_;	// Pointer to GDB
    pwr_tInt32			resultNo_;	// Number in list of results
    pwr_tRefId			*dlid_;		// dlid to pointer
    pwr_tInt32			nValues_,	// Number of values in valueList
				nValuesMax_;	// Maximum number for valueList_
    ra_value*			valueList_;	// Pointer to array of value classes
    void			listGrow();	// Function for expanding valueList_
    
};

#endif