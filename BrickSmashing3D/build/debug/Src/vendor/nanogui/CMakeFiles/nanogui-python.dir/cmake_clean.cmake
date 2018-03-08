file(REMOVE_RECURSE
  "Debug/python/nanogui.pdb"
  "Debug/python/nanogui.cp36-win32.pyd"
  "Debug/python/libnanogui.dll.a"
  "Debug/python/nanogui.cp36-win32.pyd.manifest"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/nanogui-python.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
