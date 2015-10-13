#
# This file defines Makefile for Gateway
#

# define some paths variable

TOP_DIR := $(shell pwd)
SOURCE_DIR := $(TOP_DIR)/source
REF_DIR := $(TOP_DIR)/3rd

# define build macros
CC = g++

CXXFLAGS += -I$(SOURCE_DIR)
CXXFLAGS += -I$(REF_DIR)/ace/include 

LDFLAGS += -L$(REF_DIR)/ace/lib

LIBS += -lACE 
LIBS += -lrt
LIBS += -lpthread
LIBS += -lstdc++ -lz -lm

APP := gateway

SOURCE_FILES := $(SOURCE_DIR)/main.cpp
SOURCE_FILES += $(SOURCE_DIR)/Gateway.cpp
SOURCE_FILES += $(SOURCE_DIR)/CfgService.cpp
SOURCE_FILES += $(SOURCE_DIR)/NET_Service.cpp
SOURCE_FILES += $(SOURCE_DIR)/NET_Mcast_Service.cpp

#deinfe targetes
.PHONY: all

all: $(APP)

$(APP): $(SOURCE_FILES)
	$(CC) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS) 




