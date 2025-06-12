# CMake generated Testfile for 
# Source directory: D:/Rean/IT/Main Project-final/test/Final_destination/build/_deps/xlnt-src/tests
# Build directory: D:/Rean/IT/Main Project-final/test/Final_destination/build/_deps/xlnt-build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(xlnt_test "D:/Rean/IT/Main Project-final/test/Final_destination/build/_deps/xlnt-build/tests/Debug/xlnt.test.exe")
  set_tests_properties(xlnt_test PROPERTIES  _BACKTRACE_TRIPLES "D:/Rean/IT/Main Project-final/test/Final_destination/build/_deps/xlnt-src/tests/CMakeLists.txt;85;add_test;D:/Rean/IT/Main Project-final/test/Final_destination/build/_deps/xlnt-src/tests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(xlnt_test "D:/Rean/IT/Main Project-final/test/Final_destination/build/_deps/xlnt-build/tests/Release/xlnt.test.exe")
  set_tests_properties(xlnt_test PROPERTIES  _BACKTRACE_TRIPLES "D:/Rean/IT/Main Project-final/test/Final_destination/build/_deps/xlnt-src/tests/CMakeLists.txt;85;add_test;D:/Rean/IT/Main Project-final/test/Final_destination/build/_deps/xlnt-src/tests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(xlnt_test "D:/Rean/IT/Main Project-final/test/Final_destination/build/_deps/xlnt-build/tests/MinSizeRel/xlnt.test.exe")
  set_tests_properties(xlnt_test PROPERTIES  _BACKTRACE_TRIPLES "D:/Rean/IT/Main Project-final/test/Final_destination/build/_deps/xlnt-src/tests/CMakeLists.txt;85;add_test;D:/Rean/IT/Main Project-final/test/Final_destination/build/_deps/xlnt-src/tests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(xlnt_test "D:/Rean/IT/Main Project-final/test/Final_destination/build/_deps/xlnt-build/tests/RelWithDebInfo/xlnt.test.exe")
  set_tests_properties(xlnt_test PROPERTIES  _BACKTRACE_TRIPLES "D:/Rean/IT/Main Project-final/test/Final_destination/build/_deps/xlnt-src/tests/CMakeLists.txt;85;add_test;D:/Rean/IT/Main Project-final/test/Final_destination/build/_deps/xlnt-src/tests/CMakeLists.txt;0;")
else()
  add_test(xlnt_test NOT_AVAILABLE)
endif()
