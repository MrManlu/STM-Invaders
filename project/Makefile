# Possible Targets:	all clean Debug cleanDebug Release cleanRelease

##############################################################################################
# Settings
#

# General settings
	# See $(GFXLIB)/tools/gmake_scripts/readme.txt for the list of variables
	OPT_OS					= raw32
	OPT_THUMB				= yes
	OPT_LINK_OPTIMIZE		= no
	OPT_CPU					= stm32m7

# uGFX settings
	# See $(GFXLIB)/tools/gmake_scripts/library_ugfx.mk for the list of variables
	GFXLIB					= ../ugfx_2.6
	GFXBOARD				= STM32F746-Discovery
	GFXDEMO					=
	GFXDRIVERS				=
	GFXSINGLEMAKE			= no

# Special - Required for the drivers for this discovery board.
STMHAL						= ../STM32F7xx_HAL_Driver

# Special - Required for Raw32
CMSIS						= ../CMSIS

##############################################################################################
# Set these for your project
#

ARCH     = arm-none-eabi-
SRCFLAGS = -ggdb -O1
CFLAGS   = -DINVADERS_SHOW_SPLASH
CXXFLAGS = -fno-rtti
ASFLAGS  =
LDFLAGS  = #-specs=nano.specs -specs=nosys.specs

SRC      = ./src/invaders.c ./src/main.c ./src/menu.c ./src/game.c

OBJS     =
DEFS     =
DEFS     =
LIBS     =
INCPATH  = ./inc

LIBPATH  =
LDSCRIPT =

##############################################################################################
# These should be at the end
#

include $(GFXLIB)/tools/gmake_scripts/library_ugfx.mk
include $(GFXLIB)/tools/gmake_scripts/os_$(OPT_OS).mk
include $(GFXLIB)/tools/gmake_scripts/compiler_gcc.mk

#######################################
# flash it!
#######################################
flash: .build/project.bin
		st-flash write .build/project.bin 0x8000000
# *** EOF ***
