add_executable(Chip8Emulator 
    main.cpp
	Chip8.h
	Chip8.cpp
	Cpu.h
	Cpu.cpp
)

set_target_properties(Chip8Emulator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
if (NOT MSVC)
	target_link_libraries(Chip8Emulator stdc++fs)
endif()