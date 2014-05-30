### Setting Erika's variables:
# ERIKA_FILES= ../../..
# APPBASE = .
# 
# EEOPT =


RTDRUID_CONFIGURATOR_NUMBER:=1277



############################################################################
#
# User options
#
############################################################################
EEOPT += DEBUG


############################################################################
#
# Automatic options
#
############################################################################
EEOPT += __RTD_CYGWIN__
EEOPT += __PIC30__
EEOPT += __33FJ256GP710__
EEOPT += __PIC33FJ256GP710__
EEOPT += __MICROCHIP_EXPLORER16__
EEOPT += __USE_LEDS__
EEOPT += __USE_BUTTONS__
EEOPT += __USE_LCD__
EEOPT += __USE_ANALOG__
EEOPT += __MICROCHIP_DSPIC30__
EEOPT += __PIC30_ICD2__
EEOPT += __PIC30_SPLIM__
EEOPT += __FP__
EEOPT += __MONO__
EEOPT += __ALARMS__
EEOPT += __ALLOW_NESTED_IRQ__
EEOPT += __DISABLE_EEOPT_DEFINES__


############################################################################
#
# Erika base directory
#
############################################################################
ifdef ERIKA_FILES
ifdef EEBASE
    $(warning EEBASE is set, but it has been overridden by ERIKA_FILES)
endif
EEBASE := $(shell cygpath `cygpath -ms '${ERIKA_FILES}'`)

else # ERIKA_FILES

ifndef EEBASE
        EEBASE := $(shell cygpath `cygpath -ms 'C:\Programmi\Evidence\eclipse\plugins\com.eu.evidence.ee_2.1.0.20130515_1228\ee_files'`)
else
        $(warning The usage of EEBASE is deprecated. Please use ERIKA_FILES)
endif
endif # ERIKA_FILES
# ERIKA_FILES has fulfilled its role. Make sure it's not used inside Erika makefiles
ERIKA_FILES :=
$(info Using erika files in $(EEBASE))


############################################################################
#
# PIC 30
#
############################################################################
APPBASE := ..
OUTBASE := Debug

PIC30_ASMDIR := $(shell $(EEBASE)/pkg/cfg/arch/cc_pic30_detect.sh ASM "c:\Programmi\Microchip\MPLAB ASM30 Suite")
PIC30_GCCDIR := $(shell $(EEBASE)/pkg/cfg/arch/cc_pic30_detect.sh GCC "c:\Programmi\Microchip\MPLAB C30")


############################################################################
#
# Init MCU for PIC30 (PIC33FJ256GP710)
#
############################################################################
PIC30_MODEL           := 33FJ256GP710
PIC30_LINKERSCRIPT    := p33FJ256GP710.gld
PIC30_DEV_LIB         := libp33FJ256GP710-elf.a
PIC30_INCLUDE_C       := p33FJ256GP710.h
PIC30_INCLUDE_S       := p33FJ256GP710.inc


############################################################################
#
# Flags
#
############################################################################
CFLAGS  += 
ASFLAGS += 
LDFLAGS += 
LDDEPS  += 
LIBS    += 



############################################################################
#
# Sources
#
############################################################################
APP_SRCS += $(OUTBASE)/eecfg.c code.c sensors.c


############################################################################
#
# end
#
############################################################################
include $(EEBASE)/pkg/cfg/rules.mk
