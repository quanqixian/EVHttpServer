all: thirdParty unitTest

.PHONY:help
help:
	@echo ""
	@echo "make          : compile third party libs and unit test"
	@echo "make unitTest : run unit test"
	@echo "make report   : generate code coverage report and test result report"
	@echo ""

.PHONY:thirdParty
thirdParty:googletest libevent

.PHONY:googletest
googletest:
	@cd ./thirdparty/googletest/; ./build.sh build

.PHONY:libevent
libevent:
	@cd ./thirdparty/libevent/; ./build.sh build

unitTest:
	make -C ./test/

.PHONY:runTest
runTest:thirdParty
	make -C ./test/ $@

REPORT_DIR=./report
#生成测试通过率和代码覆盖率html报告
.PHONY:report
report:thirdParty
	make -C ./test $@
	cp -r ./test/report/  $(REPORT_DIR)
	@echo ""
	@echo  "\033[42;37m    Html report is generated in directory: $(REPORT_DIR)    \033[0m"

.PHONY:clean
clean:
	@cd ./thirdparty/googletest/; ./build.sh clean
	@cd ./thirdparty/libevent/; ./build.sh clean
	make -C ./test/ clean
	rm -rf $(REPORT_DIR)

