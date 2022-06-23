-- Package Requirements
add_requires("fmt")

-------------------- Ray Tracer -------------------

-- Target RayTracer
target("light_chess")
-- Common Compiler Options
    set_languages("cxx20") -- -std=c++20
    -- FIXME: Not working
    set_warnings("allextra") -- -Wall -Wextra -Wfatal-errors (if error enabled)
    set_optimize("fastest") -- -O3
    set_targetdir("bin/")
    -- Compiler Options
    add_includedirs("include")
    -- Dependencies
    add_packages("fmt")
    -- Binary
    set_kind("binary")
    add_files("src/game.cpp")
    add_files("src/main.cpp")