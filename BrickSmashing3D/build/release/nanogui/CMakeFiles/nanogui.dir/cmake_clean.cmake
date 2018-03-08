file(REMOVE_RECURSE
  "libnanogui.pdb"
  "libnanogui.dll"
  "libnanogui.dll.a"
  "libnanogui.dll.manifest"
)

# Per-language clean rules from dependency scanning.
foreach(lang C CXX)
  include(CMakeFiles/nanogui.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
