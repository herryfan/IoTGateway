#
# This file defines Makefile for Gateway
#

# define some paths variable

TOP_DIR := $(shell pwd)
SOURCE_DIR := $(TOP_DIR)/source
REF_DIR := $(TOP_DIR)/3rd

# define build macros
CC = g++

CFLAGS += -g 
CFLAGS += -O2 
CFLAGS += -DWITH_POSIX
CFLAGS += -I$(SOURCE_DIR)
CFLAGS += -I$(REF_DIR)/ace/include 
CFLAGS += -I$(REF_DIR)/libcoap/include  

LDFLAGS += -L$(REF_DIR)/ace/lib
LDFLAGS += -L$(REF_DIR)/libcoap/lib

LIBS += -lACE 
LIBS += -lcoap
LIBS += -lrt
LIBS += -lpthread

APP := gateway

SOURCE_FILES := $(SOURCE_DIR)/main.cpp
SOURCE_FILES += $(SOURCE_DIR)/Gateway.cpp
SOURCE_FILES += $(SOURCE_DIR)/CfgService.cpp
SOURCE_FILES += $(SOURCE_DIR)/NET_Service.cpp
SOURCE_FILES += $(SOURCE_DIR)/NET_Mcast_Service.cpp
SOURCE_FILES += $(SOURCE_DIR)/CoAP_Service.cpp
SOURCE_FILES += $(SOURCE_DIR)/CoAP_Event_Handler.cpp
SOURCE_FILES += $(SOURCE_DIR)/RD_Service.cpp

#deinfe targetes
.PHONY: all

all: $(APP)

$(APP): $(SOURCE_FILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS) 




