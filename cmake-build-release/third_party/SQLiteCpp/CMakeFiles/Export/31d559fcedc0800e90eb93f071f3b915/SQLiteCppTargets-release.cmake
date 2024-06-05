#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "sqlite3" for configuration "Release"
set_property(TARGET sqlite3 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(sqlite3 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libsqlite3.a"
  )

list(APPEND _cmake_import_check_targets sqlite3 )
list(APPEND _cmake_import_check_files_for_sqlite3 "${_IMPORT_PREFIX}/lib/libsqlite3.a" )

# Import target "SQLiteCpp" for configuration "Release"
set_property(TARGET SQLiteCpp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SQLiteCpp PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSQLiteCpp.a"
  )

list(APPEND _cmake_import_check_targets SQLiteCpp )
list(APPEND _cmake_import_check_files_for_SQLiteCpp "${_IMPORT_PREFIX}/lib/libSQLiteCpp.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
