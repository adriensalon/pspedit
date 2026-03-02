if(NOT DEFINED INPUT OR NOT DEFINED OUT_CPP OR NOT DEFINED OUT_H OR NOT DEFINED SYM)
  message(FATAL_ERROR "Missing INPUT/OUT_CPP/OUT_H/SYM")
endif()

file(READ "${INPUT}" _hex HEX)
string(LENGTH "${_hex}" _hex_len)
math(EXPR _nbytes "${_hex_len} / 2")

file(WRITE "${OUT_H}"
"#pragma once\n#include <cstddef>\n#include <cstdint>\n"
"extern const std::uint8_t ${SYM}[];\n"
"extern const std::size_t ${SYM}_size;\n"
)

file(WRITE "${OUT_CPP}" "#include \"embedded_sources.h\"\n")
file(APPEND "${OUT_CPP}" "const std::uint8_t ${SYM}[] = {\n")

set(_i 0)
while(_i LESS _nbytes)
  math(EXPR _pos "${_i} * 2")
  string(SUBSTRING "${_hex}" ${_pos} 2 _b)

  math(EXPR _mod "${_i} % 12")
  if(_mod EQUAL 0)
    file(APPEND "${OUT_CPP}" "  ")
  endif()

  file(APPEND "${OUT_CPP}" "0x${_b}, ")

  if(_mod EQUAL 11)
    file(APPEND "${OUT_CPP}" "\n")
  endif()

  math(EXPR _i "${_i} + 1")
endwhile()

file(APPEND "${OUT_CPP}" "\n};\n")
file(APPEND "${OUT_CPP}" "const std::size_t ${SYM}_size = sizeof(${SYM});\n")