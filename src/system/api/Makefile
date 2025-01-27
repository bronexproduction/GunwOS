.PHONY: all clean

all: gunwapi_shared \
	 gunwapi_shared_kernel \
	 gunwapi_driver \
	 gunwapi_driver_kernel \
	 gunwapi_app \
	 gunwapi_app_kernel \
	 clean

gunwapi_shared:
ifeq ("$(wildcard $(LIB_BUILD_DIR)/lib$@.a)","")
	make -C $(SHARED_API_SRC_DIR) shared
	mv $(SHARED_API_SRC_DIR)/$@.o $(SHARED_API_SRC_DIR)/lib$@.a $(LIB_BUILD_DIR)/
endif

gunwapi_shared_kernel:
	make -C $(SHARED_API_SRC_DIR) kernel
	mv $(SHARED_API_SRC_DIR)/$@.o $(LIB_BUILD_DIR)/

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
	find $(API_DIR) -type f -name '*.o' -delete
