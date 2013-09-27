//-------------------- RA_VALUE.CPP -------------------- 
//
//	Definition of class ra_value for a mean
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


#include "ra_value.h"

ra_value::ra_value()
// Constructor for ra_value
{
  counter_ = 0;
}

void ra_value::setPointer(int valueNo, const char *pointer)
// Set pointer to GDB database
{
  pwr_sAttrRef    attrref;
  pwr_tDlid       refid;
  pwr_tStatus     sts;
  
  valueNo_ = valueNo;

  sts = gdh_NameToAttrref( pwr_cNObjid, pointer, &attrref);
  if ( EVEN(sts)) throw co_error(sts);

  sts = gdh_DLRefObjectInfoAttrref( &attrref, (void**)&pointer_, &refid);
  if ( EVEN(sts)) throw co_error(sts);

  dlid_ = &refid;
}

pwr_tFloat32 ra_value::getValue(int valueNo)
// Get value from values_ on array position valueNo
{
  return values_[valueNo];
}

void ra_value::collectValue()
// Store value from pointer to values_
{
  values_[counter_] = *pointer_;
  counter_++;
}

void ra_value::clearValues()
// Delete all values in values_ and reset counter_
{
  int i;
  
  for( i = 0; i < 59 ; i++) {
    values_[i] = 0;
  }
  
  counter_ = 0;
}

void ra_value::close()
{  
  gdh_UnrefObjectInfo(*dlid_);
  clearValues();
}