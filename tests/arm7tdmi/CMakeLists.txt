if(arm7tdmi IN_LIST ARES_COMPONENTS_LIST)
  add_executable(arm7tdmi arm7tdmi.cpp
          ps1_stub.cpp
          ps1_stub.h)

  target_include_directories(arm7tdmi PRIVATE ${CMAKE_SOURCE_DIR})

  set(ARM7TDMI_SOURCE_DIR "${CMAKE_SOURCE_DIR}/ares/ps1/cpu")

  target_sources(
    arm7tdmi
    PRIVATE
        ${ARM7TDMI_SOURCE_DIR}/cpu.cpp
          ps1_stub.cpp
  )

  set_target_properties(arm7tdmi PROPERTIES FOLDER tests PREFIX "")
  #get_target_property(arm7tdmi_SOURCES arm7tdmi SOURCES)
  #list(REMOVE_ITEM arm7tdmi_SOURCES arm7tdmi.cpp)
  #set_source_files_properties(${arm7tdmi_SOURCES} PROPERTIES HEADER_FILE_ONLY TRUE)

  target_enable_subproject(arm7tdmi "arm7tdmi processor test harness")

  target_link_libraries(arm7tdmi PRIVATE ares::ares ares::nall)
  set(CONSOLE TRUE)
  ares_configure_executable(arm7tdmi)

  source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES arm7tdmi.cpp)
endif()
