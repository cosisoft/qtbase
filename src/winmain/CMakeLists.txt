# special case begin
# special case skip regeneration
if (NOT WIN32)
    return()
endif()

qt_internal_add_module(WinMain
  STATIC
  NO_SYNC_QT
  NO_MODULE_HEADERS
  DEFINES
      QT_NO_FOREACH
  INCLUDE_DIRECTORIES
      $<TARGET_PROPERTY:Qt::Core,INTERFACE_INCLUDE_DIRECTORIES>
)

if (MSVC)
    # Store debug information inside the static lib
    string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
endif()

set_property(TARGET WinMain PROPERTY OUTPUT_NAME qtmain)

qt_internal_extend_target(WinMain CONDITION
    SOURCES qtmain_win.cpp
    LIBRARIES shell32
)

qt_internal_extend_target(WinMain CONDITION MINGW
    DEFINES QT_NEEDS_QMAIN
)
# special case end
