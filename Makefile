OS := $(firstword $(shell uname -s))

ASSETS_DIR=assets
TARGET_NAME=aldebaran
TARGET=$(ASSETS_DIR)/$(TARGET_NAME)

ifeq ($(OS),Darwin)
-include Makefile.macosx
else
-include Makefile.linux
endif

run: all
	cd $(ASSETS_DIR) && ./$(TARGET_NAME)
	
android:
	ndk-build
	ant debug
