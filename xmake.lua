add_rules("mode.debug", "mode.release")


set_languages("cxx20")


add_requires("stb", "glm", "glfw", "glad")
add_packages("stb", "glm", "glfw", "glad")


set_targetdir("Bin")
set_rundir(".")


if is_mode("debug") then
    set_suffixname("_d")
end


target("HelloOpenGL")
    set_kind("binary")

    add_includedirs("HelloOpenGL")
    add_headerfiles("HelloOpenGL/**.h")
    add_files("HelloOpenGL/**.cpp")
