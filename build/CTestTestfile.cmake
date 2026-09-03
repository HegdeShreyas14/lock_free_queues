# CMake generated Testfile for 
# Source directory: /home/hegde/Desktop/lock_free_queues
# Build directory: /home/hegde/Desktop/lock_free_queues/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[test_spsc]=] "/home/hegde/Desktop/lock_free_queues/build/test_spsc")
set_tests_properties([=[test_spsc]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/hegde/Desktop/lock_free_queues/CMakeLists.txt;25;add_test;/home/hegde/Desktop/lock_free_queues/CMakeLists.txt;0;")
add_test([=[test_spsc_tsan]=] "/home/hegde/Desktop/lock_free_queues/build/test_spsc_tsan")
set_tests_properties([=[test_spsc_tsan]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/hegde/Desktop/lock_free_queues/CMakeLists.txt;35;add_test;/home/hegde/Desktop/lock_free_queues/CMakeLists.txt;0;")
