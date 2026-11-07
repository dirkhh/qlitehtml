add_executable(qlitehtml_adapter_test main.cpp)
target_link_libraries(qlitehtml_adapter_test PRIVATE qlitehtml ${Qt}::Widgets)
add_test(NAME qlitehtml_adapter COMMAND qlitehtml_adapter_test)
set_tests_properties(qlitehtml_adapter PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
