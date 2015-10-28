#
# This file defines Makefile for Gateway
#

# define some paths variable

TOP_DIR := $(shell pwd)
SOURCE_DIR := $(TOP_DIR)/source
REF_DIR := $(TOP_DIR)/3rd

# define build macros
CC = $(CXX)

CFLAGS += -g 
CFLAGS += -O2 
CFLAGS += -DWITH_POSIX
CFLAGS += -I$(SOURCE_DIR)


# not cross compile, include ace and libcoap
ifneq ($(ARCH),arm)
    CFLAGS += -I$(REF_DIR)/ace/include 
    CFLAGS += -I$(REF_DIR)/libcoap/include  

    LDFLAGS += -L$(REF_DIR)/ace/lib
    LDFLAGS += -L$(REF_DIR)/libcoap/lib
endif

LIBS += -lACE 
LIBS += -lcoap
LIBS += -lrt
LIBS += -lpthread
LIBS += -lstdc++


APP := gateway

SOURCE_FILES := $(SOURCE_DIR)/main.cpp
SOURCE_FILES += $(SOURCE_DIR)/Gateway.cpp
SOURCE_FILES += $(SOURCE_DIR)/CfgService.cpp
SOURCE_FILES += $(SOURCE_DIR)/NET_Service.cpp
SOURCE_FILES += $(SOURCE_DIR)/NET_Mcast_Service.cpp
SOURCE_FILES += $(SOURCE_DIR)/RD_Service.cpp
SOURCE_FILES += $(SOURCE_DIR)/CoAP_Wrapper.cpp
SOURCE_FILES += $(SOURCE_DIR)/CoAP_Resource.cpp
SOURCE_FILES += $(SOURCE_DIR)/CoAP_RD_Resource.cpp
SOURCE_FILES += $(SOURCE_DIR)/FW_Proxy_Service.cpp

#deinfe targetes
.PHONY: all

all: $(APP)

$(APP): $(SOURCE_FILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS) 




