
set(FONT_SRC ${CMAKE_SOURCE_DIR}/resources/fonts/Roboto-Regular.ttf)
set(FONT_C ${CMAKE_BINARY_DIR}/resources/fontData.c)
set(FONT_BIN ${CMAKE_BINARY_DIR}/resources/fontData.o)

add_custom_command(
    OUTPUT ${FONT_C}
    COMMAND xxd -i -n robotoRegular ${FONT_SRC} > ${FONT_C}
    DEPENDS ${FONT_SRC}
    COMMENT "Create font"
)

add_custom_command(
    OUTPUT ${FONT_BIN}
    COMMAND ${CMAKE_CXX_COMPILER} -c ${FONT_C} -o ${FONT_BIN}
    DEPENDS ${FONT_C}
    COMMENT "Compile font"
)

add_custom_target(generateFonts DEPENDS ${FONT_BIN})