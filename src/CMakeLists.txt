add_executable(Chip8Emulator 
    main.cpp
)

set_target_properties(Chip8Emulator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)