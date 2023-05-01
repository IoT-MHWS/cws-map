function(custom_protobuf_generate)

  set(_options)
  set(_singleargs LANGUAGE PROTOC_OUT_DIR PLUGIN PROTO_ROOT)
  set(_multiargs IMPORT_DIRS GENERATE_EXTENSIONS)
  if(COMMAND target_sources)
    list(APPEND _singleargs TARGET)
  endif()

  cmake_parse_arguments(protobuf_generate "${_options}" "${_singleargs}" "${_multiargs}"  "${ARGN}")

  # Set includes
  set(_include_path)
  foreach(include ${protobuf_generate_IMPORT_DIRS})
    list(APPEND _include_path -I ${include})
  endforeach()

  # plugin
  set(_plugin --plugin=${protobuf_generate_PLUGIN})

  # generate protobufs
  set(_generated_srcs_all)

  get_target_property(_source_list ${protobuf_generate_TARGET} SOURCES)

  # don't include proto files
  set(_proto_list)
  foreach(_file ${_source_list})
    if(_file MATCHES "proto$")
      list(APPEND _proto_list ${_file})
    endif()
  endforeach()

  foreach(_source ${_proto_list})
    file(RELATIVE_PATH _source_rel ${protobuf_generate_PROTO_ROOT} ${_source})

    get_filename_component(_source_rel_dir ${_source_rel} DIRECTORY)
    get_filename_component(_source_name_we ${_source_rel} NAME_WE)

    set(_generated_srcs_dir ${protobuf_generate_PROTOC_OUT_DIR}/${_source_rel_dir})
    file(MAKE_DIRECTORY ${_generated_srcs_dir})

    set(_generated_srcs)
    foreach(_ext ${protobuf_generate_GENERATE_EXTENSIONS})
      list(APPEND _generated_srcs ${_generated_srcs_dir}/${_source_name_we}${_ext})
    endforeach()

    add_custom_command(
      OUTPUT ${_generated_srcs}
      COMMAND protobuf::protoc
      ARGS --${protobuf_generate_LANGUAGE}_out ${protobuf_generate_PROTOC_OUT_DIR} ${_plugin} ${_include_path} ${_source}
      DEPENDS ${_source} ${protobuf_PROTOC_EXE}
      COMMENT ${_comment}
      VERBATIM
    )
    list(APPEND _generated_srcs_all ${_generated_srcs})
  endforeach()

  set_source_files_properties(${_generated_srcs_all} PROPERTIES GENERATED TRUE)

  target_sources(${protobuf_generate_TARGET} PRIVATE ${_generated_srcs_all})

endfunction()
