cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(docs VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_custom_target(doxygenDoc ALL
	COMMAND doxygen ./Doxyfile
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "\nGenerate API documentation using doxygen begin..."
	)

#Give a notice after build
add_custom_command(TARGET doxygenDoc
	POST_BUILD
	COMMENT "Generate API documentation using doxygen finished..."
	VERBATIM
)
