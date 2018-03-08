file(REMOVE_RECURSE
  "Release/python/nanogui.pdb"
  "Release/python/nanogui.cp36-win32.pyd"
  "Release/python/libnanogui.dll.a"
  "Release/python/nanogui.cp36-win32.pyd.manifest"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/nanogui-python.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
