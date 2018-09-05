CPPFLAGS += -D_GLIBCXX_USE_C99

ULP_APP_NAME ?= ulp_main
ULP_S_SOURCES = $(COMPONENT_PATH)/main/ulp/pulse_cnt.S
ULP_EXP_DEP_OBJECTS := khronos.o ntp_server.o

include $(IDF_PATH)/components/ulp/component_ulp_common.mk