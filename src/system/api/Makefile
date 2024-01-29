.PHONY: all clean

all: gunwapi_driver gunwapi_driver_kernel gunwapi_app gunwapi_app_kernel

gunwapi_driver:
ifeq ("$(wildcard $(LIB_BUILD_DIR)/lib$@.a)","")
	make -C $(DRIVER_API_SRC_DIR) driver
	mv $(DRIVER_API_SRC_DIR)/$@.o $(DRIVER_API_SRC_DIR)/lib$@.a $(LIB_BUILD_DIR)/
endif

gunwapi_driver_kernel:
	make -C $(DRIVER_API_SRC_DIR) kernel
	mv $(DRIVER_API_SRC_DIR)/$@.o $(LIB_BUILD_DIR)/
	
gunwapi_app:
ifeq ("$(wildcard $(LIB_BUILD_DIR)/lib$@.a)","")
	make -C $(APP_API_SRC_DIR) app
	mv $(APP_API_SRC_DIR)/$@.o $(APP_API_SRC_DIR)/lib$@.a $(LIB_BUILD_DIR)/
endif

gunwapi_app_kernel:
	make -C $(APP_API_SRC_DIR) kernel
	mv $(APP_API_SRC_DIR)/$@.o $(LIB_BUILD_DIR)/

clean:
	find $(SRC_DIR)/ -type f -name '*.o' -delete
