#################################################################
#   ShiVa Extension Plugin Makefile for Linux
#################################################################

TEMP_DIR       		= ../../Temp/Linux
SRCROOT             = ../../Sources
S3DXROOT            = ../../Sources/S3DX
BUILT_PRODUCTS_DIR  = ../../Contents/Linux
PRODUCT_NAME        = sqlite3_Linux.so


#################################################################
SOURCES				= Plugin.cpp sqlite3db.cpp sqlite3.c shell.c 

#################################################################
#  Compiler executables
#
CC      			= gcc
CPP     			= g++
LD					= $(CPP)

####################################################################"
#  Compiler flags
#
CFLAGS      		= -Os -Wno-unknown-pragmas -fsigned-char -fno-exceptions -DS3DX_DLL -D__LINUX__ -D_GNU_SOURCE -DHAVE_STDINT_H -I$(SRCROOT)/Linux -I$(S3DXROOT)
CPPFLAGS    		= 
LDFLAGS    			= 

####################################################################"
#  Objects
#
OBJECTS  			= \
	$(patsubst %.c,%.o,$(filter %.c,$(SOURCES))) \
	$(patsubst %.cc,%.o,$(filter %.cc,$(SOURCES))) \
	$(patsubst %.cpp,%.o,$(filter %.cpp,$(SOURCES)))

OBJECTS_ABS			= $(addprefix $(TEMP_DIR)/,$(OBJECTS))
PRODUCT_ABS         = $(BUILT_PRODUCTS_DIR)/$(PRODUCT_NAME)

####################################################################"
#  Rules
#

$(TEMP_DIR)/%.o: $(SRCROOT)/%.c
	mkdir -p `dirname $@`
	$(CC) $(CFLAGS) -c $< -o $@

$(TEMP_DIR)/%.o: $(SRCROOT)/%.cpp
	mkdir -p `dirname $@`
	$(CPP) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

####################################################################"
#  Targets
#
all: $(PRODUCT_ABS) 

$(PRODUCT_ABS): $(OBJECTS_ABS)
	mkdir -p $(BUILT_PRODUCTS_DIR)
	$(LD) -shared -o $(PRODUCT_ABS) $(OBJECTS_ABS) $(LDFLAGS)

clean:
	rm -f $(PRODUCT_ABS) $(OBJECTS_ABS)

####################################################################"
