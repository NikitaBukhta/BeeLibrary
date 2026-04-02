find_package(Qt6 REQUIRED COMPONENTS Test)

enable_testing()

function(add_unit_test name)
    qt_add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE BeeLibraryCore Qt6::Test)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_unit_test(tst_SqlQueryBuilder)
add_unit_test(tst_BookDTO)
add_unit_test(tst_ContextModel)
add_unit_test(tst_BookTable)
add_unit_test(tst_BookFormController)
