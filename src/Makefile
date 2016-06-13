# Makefile for the pvPortClient library

TOP = ..
include $(TOP)/configure/CONFIG

PVPORTCLIENT_SRC = $(TOP)/src


LIBRARY += pvPortClient

INC += pv/pvPortClient.h
INC += pv/scalarClient.h

include $(PVPORTCLIENT_SRC)/portClient/Makefile

pvPortClient_LIBS += pvAccess pvData Com 
pvPortClient_LIBS += $(EPICS_BASE_IOC_LIBS)

# shared library ABI version.
#SHRLIB_VERSION ?= 4.2


include $(TOP)/configure/RULES

