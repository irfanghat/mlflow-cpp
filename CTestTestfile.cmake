# CMake generated Testfile for 
# Source directory: /home/irfanghat/mlflow-cpp
# Build directory: /home/irfanghat/mlflow-cpp
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(CoreClientTests "/home/irfanghat/mlflow-cpp/mlflow_tests")
set_tests_properties(CoreClientTests PROPERTIES  _BACKTRACE_TRIPLES "/home/irfanghat/mlflow-cpp/CMakeLists.txt;52;add_test;/home/irfanghat/mlflow-cpp/CMakeLists.txt;0;")
subdirs("_deps/json-build")
subdirs("_deps/googletest-build")
