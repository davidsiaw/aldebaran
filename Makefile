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
	
# Runs inside the ndk-env container (needs ndk-build and ant on PATH).
android:
	ndk-build
	ant debug

# Run from the host: builds in the ndk-env container (NDK r9, API level 10).
ANDROID_IMAGE    = davidsiaw/ndk-env:original-api-level-10
ANDROID_PACKAGE  = net.astrobunny.aldebaran
ANDROID_ACTIVITY = .Aldebaran
# Persistent debug signing key, so `adb install -r` keeps working between builds.
ANDROID_KEYDIR   = $(HOME)/.ndk-env-android
DOCKER_TTY      := $(shell [ -t 0 ] && echo -ti)

# Builds as root (the 2014 SDK tools expect it), then hands everything the
# container wrote back to the calling user, even if the build failed.
build-android:
	mkdir -p $(ANDROID_KEYDIR)
	docker run --rm $(DOCKER_TTY) \
		-e JAVA_HOME=/usr/lib/jvm/java-7-openjdk-amd64 \
		-v $(ANDROID_KEYDIR):/root/.android \
		-v $(CURDIR):/src \
		$(ANDROID_IMAGE) \
		bash -c 'status=0; /build.sh || status=$$?; chown -R $(shell id -u):$(shell id -g) /src /root/.android; exit $$status'

run-android: build-android
	adb install -r $$(ls -t bin/*-debug.apk | head -n1)
	adb logcat -c
	adb shell am start -n $(ANDROID_PACKAGE)/$(ANDROID_ACTIVITY)
	adb logcat | grep --line-buffered -iE "SDL|aldebaran|AndroidRuntime|DEBUG"

.PHONY: run android build-android run-android
