FILE(REMOVE_RECURSE
  "CMakeFiles/example_c.dir/example.c.o"
  "../bin/example_c.pdb"
  "../bin/example_c"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C)
  INCLUDE(CMakeFiles/example_c.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
