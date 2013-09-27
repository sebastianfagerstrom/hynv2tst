#ifndef RA_MEAN_H
#define	RA_MEAN_H

//--------------------  RA_MEAN.H   -------------------- 
//
//	Definition of class ra_result for a mean
//	value calculation
//
//--------------------    SE1650    --------------------

#include "pwr.h"
#include "rt_appl.h"
#include "pwr_ssaboxclasses.h"
#include "pwr_baseclasses.h"
#include "rt_gdh.h"
#include "rt_mh_appl.h"
#include "co_error.h"
#include "rt_aproc.h"
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>

#include "result.h"

class ra_mean : public rt_appl {
  public:
    ra_mean() : rt_appl( "ra_mean", errh_eAnix_appl1) {}
    void 	open();
    void 	close();
    void 	scan();
    void 	alarm( const char *description, const char *text);
    void	addResult(const char *name, const char *pointer);
    void 	collectValues();
    void 	calculateResults();
    
  private:
    pwr_tBoolean		isActive_;	// Specifies if the logging has started    
    pwr_tInt32			nResults_,	// Number of results in resultsList_
				nResultsMax_,	// Maxinum number of result in resultsList_
				nSeconds_;	// Seconds since last mean calculation of mean
    ra_result*			resultsList_;	// Pointer to array of result classes
    time_t			oldTime_;  	// Last time vaues collected
    void			listGrow();	// Expand resultList if needed
}; 
      

#endif