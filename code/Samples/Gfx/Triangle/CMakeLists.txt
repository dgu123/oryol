fips_begin_app(Triangle windowed)
    fips_files(Triangle.cc)
    fips_generate(TYPE Shader FROM shaders.shd)
    fips_deps(Assets)
    oryol_add_web_sample(Triangle "The canonical triangle demo" "emscripten,android,pnacl" Triangle.jpg "Gfx/Triangle/Triangle.cc")
fips_end_app()
