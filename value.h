#ifndef RA_VALUE_H
#define	RA_VALUE_H

//-------------------- RA_VALUE.CPP -------------------- 
//
//	Definition of class ra_value for a mean
//	value calculation
//
//--------------------    SE1650    --------------------

class ra_value
{
  public:
    ra_value();
    void		setPointer(int valueNo, const char *pointer);
    pwr_tFloat32	getValue(int valueNo);
    void		collectValue();
    void		clearValues();
    void		close();
    
  private:
    pwr_tFloat32	*pointer_;
    pwr_tInt32		valueNo_;
    pwr_tInt32		counter_;
    pwr_tFloat32	values_[60];
    pwr_tRefId		*dlid_;
};
#endif
    
    
  