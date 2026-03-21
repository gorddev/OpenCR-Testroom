
target_link_options(Example PRIVATE
    -Werror
    -Wall
    -DGL_SILENCE_DEPRECATION
    "--shell-file=${CMAKE_SOURCE_DIR}/external/emscripten/em_shell.html"
    "-O3"
    "-sUSE_WEBGL2=1"
    "-std=c++23"
    "--preload-file=assets"
    "--emrun"
    "-Wno-error=limited-postlink-optimizations"
    "-sMAXIMUM_MEMORY=512MB"
    "-sALLOW_MEMORY_GROWTH"
    "-sNO_DISABLE_EXCEPTION_CATCHING"
    #"-fwasm-exceptions"

    #"-fsanitize=address"
    #"-fno-omit-frame-pointer"
    #"-sEXPORTED_FUNCTIONS=['_main','_SDL_main']"
    "-sGL_ASSERTIONS=1"
    "-sGL_DEBUG=1"
    "-sASSERTIONS=1"
)