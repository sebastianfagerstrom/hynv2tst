//--------------------  RA_MEAN.CPP -------------------- 
//
//	Definition of class ra_result for a mean
//	value calculation
//
//--------------------    SE1650    --------------------

#include "ra_mean.h"
 
void ra_mean::alarm( const char *description, const char *text)
// Send alarm to proview
{
    mh_sApplMessage	msg;
    pwr_tUInt32		msgid;
    pwr_tStatus		sts;
    
    memset(&msg, 0, sizeof(msg));
    msg.EventFlags = mh_mEventFlags(mh_mEventFlags_Returned |
		      mh_mEventFlags_NoObject |
		      mh_mEventFlags_Bell);
    clock_gettime( CLOCK_REALTIME, (timespec *)&msg.EventTime);
    strcpy( msg.EventName, description);
    sprintf( msg.EventText, text);
    msg.EventType = mh_eEvent_Alarm;
    msg.EventPrio = mh_eEventPrio_B;
    sts = mh_ApplMessage( &msgid, &msg);
      if (EVEN (sts)) throw co_error(sts);
}

void ra_mean::open()
// ra_mean::open calls every program start
{
  pwr_tStatus	sts;
  pwr_tUInt32	num;
  pwr_tObjid	aoid;
  string	line, tmpSystem, tmpPointer, tmpTime;
  pwr_tBoolean	newResult = TRUE;
  pwr_tInt32	nValues = 0;
  
  isActive_ = 0;
  nResultsMax_ = 5;
  nResults_ = 0;
  resultsList_ = new ra_result[nResultsMax_]; // Allocate memory for resultsList_
  
  ifstream setup_file("ra_mean_setup.txt"); // Setup file
  
  if(setup_file.is_open() ) { // Open setup file
    
    while ( getline (setup_file, line) ) { // Get lines i setup file
      
      if(line.substr(0,1) != "#") { // Check if line don't start with #, then read info
	tmpSystem = line.substr(0, line.find(' ') );
	tmpPointer = line.substr((line.find(' ') + 1), line.length() - line.find(' ') );
	
	if(tmpPointer.find(' ') != string::npos)
	  tmpPointer = tmpPointer.substr(0, tmpPointer.find(' '));
	
	if(newResult) { // If it's the first row in description, create a new result
	  addResult( tmpSystem.c_str(), tmpPointer.c_str() );
	  newResult = FALSE;
	}
	else { // Else we create a new value to the current result 
	  resultsList_[nResults_].addValue(tmpPointer.c_str());
	  nValues++;
	}
      }
      else { // If we encounter a #, then we shall prepare for a new result entry
	nValues = 0;
	
	if(newResult == FALSE) {
	  newResult = TRUE;
	  nResults_++;
	}
      }
    }
    setup_file.close(); // When there are no more lines, close the file
  }
  else 
    alarm("ra_mean", "Unable to open file");
    
  sts = mh_ApplConnect( aoid, mh_mApplFlags(0), "", mh_eEvent_Info, mh_eEventPrio_A, mh_mEventFlags_Bell, "", &num);
    if (EVEN (sts)) throw co_error(sts);
}

void ra_mean::scan()
// Scan function is the class main function
{
  if(!isActive_) { // If it's the first program cycle, activate logging
    
    for(int i = 0; i < nResults_; i++)
      resultsList_[i].clearResult();
    
    time(&oldTime_);
    isActive_ = TRUE;
    nSeconds_ = 0;
    collectValues();
  }
  if(difftime(oldTime_, time(NULL)) != 0) { // Run every second
    time(&oldTime_);
    
    if(nSeconds_ == 60) { // Run every minute
      calculateResults();
      collectValues();
      nSeconds_ = 0;
    }
    else {
      collectValues();
      nSeconds_++;
    }
  }
}

void ra_mean::collectValues()
// Collect values for mean calculation
{
  for(int i = 0; i < nResults_; i++)
    resultsList_[i].collectValues(); 
}

void ra_mean::calculateResults()
// Calculate mean
{
  for(int i = 0; i < nResults_; i++)
    resultsList_[i].calculateResult();
}

void ra_mean::addResult(const char *name, const char *pointer)
// Add new result to resultsList_
{
  if(nResults_ == nResultsMax_)
    ra_mean::listGrow();
  
  resultsList_[nResults_].setPointer(nResults_, pointer);
}

void ra_mean::listGrow()
// Grow the list of results
{
  nResultsMax_ = nResults_ + 5;
  ra_result* newList = new ra_result[nResultsMax_];
  
  for(int i=0; i < nResults_; i++)
    newList[i] = resultsList_[i];
  
  delete [] resultsList_;
  resultsList_ = newList;
}

void ra_mean::close()
// Close the function
{
  for (int i = 0;i < nResults_; i++) {
    resultsList_[i].clearResult();
    resultsList_[i].close();
  }
  
  delete [] resultsList_;
  
  printf("Alla referenser borttagna");
}

int main()
// Main function for ra_mean
{
  ra_mean * mean = new ra_mean; // Allocate memory by creating pointer
  
  mean->init();
  mean->register_appl( "Noder-Hynv2tst-ra_mean" );
  
  mean->mainloop();
}
  





