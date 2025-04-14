# Config
BOARD=arduino:avr:uno
BUILD_DIR=build
LIB_DIR=lib
CXX_STANDARD=17
ENABLE_COMPILER_LOGS=false

# Determine the log flag based on ENABLE_COMPILER_LOGS
ifeq ($(ENABLE_COMPILER_LOGS), true)
  LOG_FLAG=--log --log-level trace
else
  LOG_FLAG=
endif

# Targets
all: version receiver transmitter
# The 'all' target builds all components: version, receiver, and transmitter.

version:
	@echo "Updating version..."
	# The 'version' target updates the version number in the Version.h file.
	@if [ -f $(LIB_DIR)/UartIO/Version.h ]; then \
		VERSION=$$(grep -oP '(?<=VERSION ")[^"]+' $(LIB_DIR)/UartIO/Version.h); \
		MAJOR=$$(echo $$VERSION | cut -d. -f1); \
		MINOR=$$(echo $$VERSION | cut -d. -f2); \
		PATCH=$$(echo $$VERSION | cut -d. -f3); \
		PATCH=$$((PATCH + 1)); \
		NEW_VERSION="$$MAJOR.$$MINOR.$$PATCH"; \
		echo "#ifndef VERSION_H" > $(LIB_DIR)/UartIO/Version.h.tmp; \
		echo "#define VERSION_H" >> $(LIB_DIR)/UartIO/Version.h.tmp; \
		echo "" >> $(LIB_DIR)/UartIO/Version.h.tmp; \
		echo "#define VERSION \"$$NEW_VERSION\"" >> $(LIB_DIR)/UartIO/Version.h.tmp; \
		echo "" >> $(LIB_DIR)/UartIO/Version.h.tmp; \
		echo "#endif // VERSION_H" >> $(LIB_DIR)/UartIO/Version.h.tmp; \
		mv $(LIB_DIR)/UartIO/Version.h.tmp $(LIB_DIR)/UartIO/Version.h; \
		echo "Version updated to $$NEW_VERSION"; \
	else \
		echo "Version.h file not found!"; \
		exit 1; \
	fi

receiver:
	arduino-cli compile $(LOG_FLAG) \
		--fqbn $(BOARD) \
		--output-dir $(BUILD_DIR)/UartReceiver \
		--libraries $(LIB_DIR) \
		--build-property "compiler.cpp.extra_flags=-std=c++$(CXX_STANDARD)" \
		UartReceiver
	# The 'receiver' target compiles the UartReceiver project using arduino-cli.

transmitter:
	arduino-cli compile $(LOG_FLAG) \
		--fqbn $(BOARD) \
		--output-dir $(BUILD_DIR)/UartTransmitter \
		--libraries $(LIB_DIR) \
		--build-property "compiler.cpp.extra_flags=-std=c++$(CXX_STANDARD)" \
		UartTransmitter
	# The 'transmitter' target compiles the UartTransmitter project using arduino-cli.

format:
	find lib/UartIO -name '*.cpp' -o -name '*.h' | xargs clang-format -i
	# The 'format' target applies clang-format to all .cpp and .h files in the lib/UartIO directory.

clean:
	rm -rf $(BUILD_DIR)
	# The 'clean' target removes the build directory and all compiled files.
