ra_mean_top : ra_mean

include $(pwr_exe)/pwrp_rules.mk

ra_mean_modules_linux = \
	$(pwrp_obj)/ra_mean.o \
	$(pwrp_obj)/result.o \
	$(pwrp_obj)/value.o \
	$(pwrp_exe)/ra_mean

ra_mean_modules : $(ra_mean_modules_linux)

#
# Main rule
#

ra_mean : ra_mean_modules
	@echo "ra_mean built"


.PHONY: linux
.PHONY: all

linux: 	$(ra_mean_modules_linux)
all:	ra_mean_modules

#
# Modules
#

$(pwrp_obj)/ra_mean.o : $(pwrp_appl)/mean/ra_mean.cpp \
                        $(pwrp_inc)/ra_mean.h

$(pwrp_obj)/result.o : $(pwrp_appl)/mean/result.cpp \
                         $(pwrp_inc)/result.h

$(pwrp_obj)/value.o : $(pwrp_appl)/mean/value.cpp \
                         $(pwrp_inc)/value.h

$(pwrp_exe)/ra_mean : $(pwrp_obj)/ra_mean.o \
	$(pwrp_obj)/result.o \
	$(pwrp_obj)/value.o
	@echo "Link $(tname)"
	@$(ldxx) $(linkflags) -o $(target) $(source) $(pwrp_obj)/result.o \
	$(pwrp_obj)/value.o \
	-lpwr_rt -lpwr_co -lpwr_msg_dummy -lrpcsvc -lpthread -lm -lrt	