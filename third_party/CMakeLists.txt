# don't export any symbols from the third party stuff
# set(CMAKE_C_VISIBILITY_PRESET hidden)

if(NOT AMALGAMATION_BUILD)
  add_subdirectory(libpg_query)
  add_subdirectory(utf8proc)
  add_subdirectory(fmt)
  add_subdirectory(fsst)
  add_subdirectory(miniz)
  add_subdirectory(re2)
endif()
