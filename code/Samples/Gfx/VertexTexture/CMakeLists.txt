fips_begin_app(VertexTexture windowed)
    fips_files(VertexTexture.cc)
    fips_generate(TYPE Shader FROM shaders.shd)
    fips_deps(Assets Time Dbg)
    oryol_add_web_sample(VertexTexture "Vertex texture fetch from offscreen render target" "emscripten,android,pnacl" VertexTexture.jpg "Gfx/VertexTexture/VertexTexture.cc")
fips_end_app()
