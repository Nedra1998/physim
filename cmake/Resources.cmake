function(create_resources dir output)
  file(GLOB bins ${dir}/*)
  foreach(bin ${bins})
    get_filename_component(sym "${bin}" NAME_WLE)
    string(MAKE_C_IDENTIFIER "${sym}" csym)
    add_custom_command(
      OUTPUT ${output}/${sym}.h
      COMMAND binary_to_compressed_c -base85 "${bin}" "${csym}" >
              ${output}/${sym}.h
      DEPENDS "${bin}")
  endforeach()
endfunction()
