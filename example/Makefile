#
# Makefile for example
#

LOCAL_DIR := $(shell pwd)
SOURCE_DIR := $(LOCAL_DIR)/../source/

SOURCE_FILES := $(LOCAL_DIR)/mcast.cpp
SOURCE_FILES += $(SOURCE_DIR)/DgramMulticast.cpp

INCLUDE_FILES := $(SOURCE_DIR)/DgramMulticast.h

CFLAGS += -I$(SOURCE_DIR)

App := mcast
CC := g++

.PONY: all

$(App): $(SOURCE_FILES)
	$(CC) $(CFLAGS) -o $@ $^

all:$(App)



