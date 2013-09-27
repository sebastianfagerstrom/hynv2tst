//-------------------- RA_RESULT.CPP -------------------- 
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

#include "ra_result.h"

ra_result::ra_result()
// Constructor for ra_result
{
  nValuesMax_ = 5;
  nValues_ = 0;
  valueList_ = new ra_value[nValuesMax_];
}

void ra_result::setPointer(int resultNo, const char *pointer)
// Set pointer to result
{
  pwr_sAttrRef    attrref;
  pwr_tDlid       refid;
  pwr_tStatus     sts;
  
  resultNo_ = resultNo;
  
  sts = gdh_NameToAttrref( pwr_cNObjid, pointer, &attrref);
  if ( EVEN(sts)) throw co_error(sts);

  sts = gdh_DLRefObjectInfoAttrref( &attrref, (void**)&pointer_, &refid);
  if ( EVEN(sts)) throw co_error(sts);

  dlid_ = &refid;
}

void ra_result::addValue(const char *pointer)
// Add value to result calculation
{
  if(nValues_ == nValuesMax_)
    listGrow();
  
  valueList_[nValues_++].setPointer(nValues_, pointer);
}

pwr_tFloat32 ra_result::getResult(int resultNo)
// Get value from values_ on array position valueNo
{
  pwr_tFloat32 result = pointer_->Value[resultNo];
  
  return result;
}

void ra_result::calculateResult()
// Calculate result for all values in valueList_
{
  pwr_tFloat32 tmpResult = 0;
  
  for(int i = 0; i < nValues_ ; i++) {
    for(int j = 0; j < 60 ; j++) {
      tmpResult = tmpResult + valueList_[i].getValue(j);
    }
    pointer_->Value[i] = tmpResult / 60;
    tmpResult = 0;
    valueList_[i].clearValues();
  }    
}

void ra_result::collectValues()
// Collect data for values in valueList_
{
  for(int i = 0; i < nValues_ ; i++) {
    valueList_[i].collectValue();
  }
}

void ra_result::clearResult()
// Delete all results in values_ and reset counter_
{
  
  for(int i = 0; i < nValues_ ; i++) {
    valueList_[i].clearValues();
    pointer_->Value[i] = 0;
  }
}

void ra_result::listGrow()
// grow the list of values
{
  nValuesMax_ = nValues_ + 5;
  ra_value* newList = new ra_value[nValuesMax_];
  
  for(int i=0; i < nValues_; i++)
    newList[i] = valueList_[i];
  
  delete [] valueList_;
  valueList_ = newList;
}

void ra_result::close()
{
  for (int i = 0;i < nValues_; i++)
    valueList_[i].close();
  
  gdh_UnrefObjectInfo(*dlid_);
  
  delete [] valueList_;
}