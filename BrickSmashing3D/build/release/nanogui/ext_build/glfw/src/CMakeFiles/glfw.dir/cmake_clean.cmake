file(REMOVE_RECURSE
  "glfw3.pdb"
  "glfw3.dll"
  "libglfw3dll.a"
  "glfw3.dll.manifest"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/glfw.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
