OUTPUT_DIR=build
REPORT_DIR=reports

ifeq ($(OS),Windows_NT)
  CONFIGURE_CMD=./configure.bat
  LAUNCH_PREFIX=start
  LAUNCH_SUFFIX=
  CONFIG=Debug
  BINARY_DIR=Debug/
  BINARY_SUFFIX=.exe
  MEMCHECK=drmemory.exe
else
  CONFIGURE_CMD=./configure.sh
  LAUNCH_PREFIX=
  LAUNCH_SUFFIX=&
  CONFIG=Debug
  BINARY_DIR=
  BINARY_SUFFIX=
  MEMCHECK=valgrind --leak-check=full --track-origins=yes --xml=yes --xml-file=$(REPORT_DIR)/memcheck.xml 
  MEMCHECK_GUI=valgrind --leak-check=full --track-origins=yes --xml=yes --xml-file=$(REPORT_DIR)/memcheck_gui.xml 
endif

CUCUMBER_FEATURES_PATH=tests/feature
CUCUMBER=cd $(CUCUMBER_FEATURES_PATH) && cucumber

CUCUMBER_GUI_FEATURES_PATH=gui/tests/feature
CUCUMBER_GUI=cd $(CUCUMBER_GUI_FEATURES_PATH) && cucumber

all: unittest features gui-unittest gui-features gui

.PHONY: configure
configure: 
	$(CONFIGURE_CMD) -DCMAKE_BUILD_TYPE=$(CONFIG)

$(OUTPUT_DIR)/CMakeFiles:
	@make configure

$(REPORT_DIR):
	@make configure

.PHONY: prepare
prepare: $(OUTPUT_DIR)/CMakeFiles
	@cmake --build $(OUTPUT_DIR) --target googlemock
	@cmake --build $(OUTPUT_DIR) --target boost
	@cmake --build $(OUTPUT_DIR) --target cucumber-cpp
	@$(CONFIGURE_CMD)

.PHONY: tournament_test
tournament_test: $(OUTPUT_DIR)/CMakeFiles
	@cmake --build $(OUTPUT_DIR) --target tournament_test

.PHONY: karateka_test
karateka_test: $(OUTPUT_DIR)/CMakeFiles
	@cmake --build $(OUTPUT_DIR) --target karateka_test

.PHONY: date_test
date_test: $(OUTPUT_DIR)/CMakeFiles
	@cmake --build $(OUTPUT_DIR) --target date_test

.PHONY: tournament_registrator_test
tournament_registrator_test: $(OUTPUT_DIR)/CMakeFiles
	@cmake --build $(OUTPUT_DIR) --target tournament_registrator_test

.PHONY: persistency_test
persistency_test: $(OUTPUT_DIR)/CMakeFiles
	@cmake --build $(OUTPUT_DIR) --target persistency_test

.PHONY: tournament_model_test
tournament_model_test: $(OUTPUT_DIR)/CMakeFiles
	@cmake --build $(OUTPUT_DIR) --target tournament_model_test

.PHONY: mainwindow_test
mainwindow_test: $(OUTPUT_DIR)/CMakeFiles
	@cmake --build $(OUTPUT_DIR) --target mainwindow_test

.PHONY: test
test: tournament_test karateka_test date_test tournament_registrator_test persistency_test tournament_model_test mainwindow_test
	@cmake --build $(OUTPUT_DIR) --target test

.PHONY: gui_translation
gui_translation: $(OUTPUT_DIR)/CMakeFiles
	@cmake --build $(OUTPUT_DIR) --target translations

.PHONY: gui
gui:
	@cmake --build $(OUTPUT_DIR) --target Kata_Tournament_Runner

.PHONY: unittest
unittest: $(OUTPUT_DIR)/CMakeFiles 
	@cmake --build $(OUTPUT_DIR) --target unittests
	$(OUTPUT_DIR)/tests/unit/$(BINARY_DIR)/unittests$(BINARY_SUFFIX) --gtest_shuffle

.PHONY: coverage-unittests
coverage-unittests: $(OUTPUT_DIR)/CMakeFiles
	@cmake --build $(OUTPUT_DIR) --target coverage_tests

.PHONY: gui-unittest
gui-unittest: $(OUTPUT_DIR)/CMakeFiles 
	@cmake --build $(OUTPUT_DIR) --target gui_unittests
	$(OUTPUT_DIR)/gui/tests/unit/$(BINARY_DIR)/gui_unittests$(BINARY_SUFFIX) --gtest_shuffle

.PHONY: coverage-gui-unittests
coverage-gui-unittests: $(OUTPUT_DIR)/CMakeFiles
	@cmake --build $(OUTPUT_DIR) --target coverage_gui_unittests

.PHONY: build-features
build-features: $(OUTPUT_DIR)/CMakeFiles
	@cmake --build $(OUTPUT_DIR) --target features

.PHONY: launch-wireserver
launch-wireserver: build-features
	$(LAUNCH_PREFIX) $(OUTPUT_DIR)/tests/feature/$(BINARY_DIR)/features$(BINARY_SUFFIX) $(LAUNCH_SUFFIX)

.PHONY: features
features: launch-wireserver
	$(CUCUMBER) 

.PHONY: coverage-features
coverage-features: $(OUTPUT_DIR)/CMakeFiles
	@cmake --build $(OUTPUT_DIR) --target coverage_features

.PHONY: wip-features
wip-features: launch-wireserver
	$(CUCUMBER) --profile wip 

.PHONY: features-doc
features-doc: launch-wireserver
	$(CUCUMBER) --profile html

.PHONY: build-gui-features
build-gui-features: $(OUTPUT_DIR)/CMakeFiles
	@cmake --build $(OUTPUT_DIR) --target gui_features

.PHONY: launch-gui-wireserver
launch-gui-wireserver: build-gui-features
	$(LAUNCH_PREFIX) $(OUTPUT_DIR)/gui/tests/feature/$(BINARY_DIR)/gui_features$(BINARY_SUFFIX) $(LAUNCH_SUFFIX)
		
.PHONY: gui-features
gui-features: launch-gui-wireserver
	sleep 5
	$(CUCUMBER_GUI) 
		
.PHONY: coverage-gui-features
coverage-gui-features: $(OUTPUT_DIR)/CMakeFiles
	@cmake --build $(OUTPUT_DIR) --target coverage_gui_features

.PHONY: wip-gui-features
wip-gui-features: launch-gui-wireserver
	sleep 5
	$(CUCUMBER_GUI) --profile wip 

.PHONY: gui-features-doc
gui-features-doc: launch-gui-wireserver
	sleep 5
	$(CUCUMBER_GUI) --profile html	

.PHONY: unittestcoverage
unittestcoverage: coverage-unittests coverage-gui-unittests
	./collect-coverage.sh $(REPORT_DIR)/unittest-coverage.xml -d

.PHONY: featurescoverage
featurescoverage:  coverage-features coverage-gui-features
	./collect-coverage.sh $(REPORT_DIR)/features-coverage.xml -d

.PHONY: overallcoverage
overallcoverage:  coverage-unittests coverage-gui-unittests coverage-features coverage-gui-features
	./collect-coverage.sh $(REPORT_DIR)/overall-coverage.xml

.PHONY: coverage
coverage: unittestcoverage featurescoverage overallcoverage

.PHONY: memcheck
memcheck: build-features
	$(LAUNCH_PREFIX) $(MEMCHECK) $(OUTPUT_DIR)/tests/feature/$(BINARY_DIR)/features$(BINARY_SUFFIX) $(LAUNCH_SUFFIX)
	sleep 10
	$(CUCUMBER)

.PHONY: memcheck-gui
memcheck-gui: build-gui-features
	$(LAUNCH_PREFIX) $(MEMCHECK_GUI) $(OUTPUT_DIR)/gui/tests/feature/$(BINARY_DIR)/gui_features$(BINARY_SUFFIX) $(LAUNCH_SUFFIX)
	sleep 20
	$(CUCUMBER_GUI)	
	

.PHONY: cppcheck
cppcheck: $(REPORT_DIR)
	cppcheck -iexternals --enable=style -f --std=c++11 -j2 --xml --suppress=*:externals/include/gtest/gtest.h --suppress=*:externals/include/gtest/internal/gtest-tuple.h --suppress=*:externals/include/gmock/gmock.h ./ 2> $(REPORT_DIR)/cppcheck.xml

.PHONY: rats
rats: $(REPORT_DIR)
	rats --quiet --xml $SOURCE_PATH/src $SOURCE_PATH/include $SOURCE_PATH/gui/src $SOURCE_PATH/gui/include > $(REPORT_DIR)/rats.xml

.PHONY: clean	
clean: $(OUTPUT_DIR)/CMakeFiles
	@cmake --build $(OUTPUT_DIR) --target clean
	rm -rf $(OUTPUT_DIR)
	
.PHONY: clean-full	
clean-full: clean 
	rm -rf externals/lib	
	rm -rf externals/include

.PHONY: clean-reports
clean-reports: 
	rm -rf $(REPORT_DIR)

.PHONY: sonar-runner
sonar-runner: clean-reports cppcheck rats coverage memcheck
	sonar-runner

.PHONY: doxygen
doxygen:  
	@cmake --build $(OUTPUT_DIR) --target doc

.PHONY: doc
doc: features-doc gui-features-doc doxygen

.PHONY: install
install: clean-full
	@make prepare
	@make gui 
	@cmake --build $(OUTPUT_DIR) --target install

.PHONY: package
package: clean-full
	@make prepare CONFIG='Release'
	@make gui CONFIG='Release'
	@make doxygen
	@cmake --build $(OUTPUT_DIR) --target package
