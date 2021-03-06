//------------------------------------------------------------------------------
//  Instancing.cc
//------------------------------------------------------------------------------
#include "Pre.h"
#include "Core/App.h"
#include "Gfx/Gfx.h"
#include "Assets/Gfx/ShapeBuilder.h"
#include "Dbg/Dbg.h"
#include "Input/Input.h"
#include "Time/Clock.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/random.hpp"
#include "shaders.h"

using namespace Oryol;

class InstancingApp : public App {
public:
    AppState::Code OnInit();
    AppState::Code OnRunning();
    AppState::Code OnCleanup();
    
private:
    void updateCamera();
    void emitParticles();
    void updateParticles();

    Id instanceMesh;
    Id drawState;
    glm::mat4 view;
    glm::mat4 proj;
    glm::mat4 model;
    glm::mat4 modelViewProj;
    bool updateEnabled = true;
    int32 frameCount = 0;
    int32 curNumParticles = 0;
    TimePoint lastFrameTimePoint;
    static const int32 MaxNumParticles = 1024 * 1024;
    const int32 NumParticlesEmittedPerFrame = 100;
    glm::vec4 positions[MaxNumParticles];
    glm::vec4 vectors[MaxNumParticles];
};
OryolMain(InstancingApp);

//------------------------------------------------------------------------------
AppState::Code
InstancingApp::OnRunning() {
    
    Duration updTime, bufTime, drawTime;
    this->frameCount++;
    
    // update block
    this->updateCamera();
    if (this->updateEnabled) {
        TimePoint updStart = Clock::Now();
        this->emitParticles();
        this->updateParticles();
        updTime = Clock::Since(updStart);

        TimePoint bufStart = Clock::Now();
        Gfx::UpdateVertices(this->instanceMesh, this->curNumParticles * sizeof(glm::vec4), this->positions);
        bufTime = Clock::Since(bufStart);
    }
    
    // render block        
    TimePoint drawStart = Clock::Now();
    Gfx::ApplyDefaultRenderTarget();
    Gfx::Clear(PixelChannel::All, glm::vec4(0.0f));
    Gfx::ApplyDrawState(this->drawState);
    Gfx::ApplyVariable(Shaders::Main::ModelViewProjection, this->modelViewProj);
    Gfx::DrawInstanced(0, this->curNumParticles);
    drawTime = Clock::Since(drawStart);
    
    Dbg::DrawTextBuffer();
    Gfx::CommitFrame();
    
    // toggle particle update
    const Mouse& mouse = Input::Mouse();
    if (mouse.Attached && mouse.ButtonDown(Mouse::Button::LMB)) {
        this->updateEnabled = !this->updateEnabled;
    }
    
    Duration frameTime = Clock::LapTime(this->lastFrameTimePoint);
    Dbg::PrintF("\n %d instances\n\r upd=%.3fms\n\r bufUpd=%.3fms\n\r draw=%.3fms\n\r frame=%.3fms\n\r"
                " LMB/Tap: toggle particle updates",
                this->curNumParticles,
                updTime.AsMilliSeconds(),
                bufTime.AsMilliSeconds(),
                drawTime.AsMilliSeconds(),
                frameTime.AsMilliSeconds());
    
    return Gfx::QuitRequested() ? AppState::Cleanup : AppState::Running;
}

//------------------------------------------------------------------------------
void
InstancingApp::updateCamera() {
    float32 angle = this->frameCount * 0.01f;
    glm::vec3 pos(glm::sin(angle) * 10.0f, 2.5f, glm::cos(angle) * 10.0f);
    this->view = glm::lookAt(pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    this->modelViewProj = this->proj * this->view * this->model;
}

//------------------------------------------------------------------------------
void
InstancingApp::emitParticles() {
    for (int32 i = 0; i < NumParticlesEmittedPerFrame; i++) {
        if (this->curNumParticles < MaxNumParticles) {
            this->positions[this->curNumParticles] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            glm::vec3 rnd = glm::ballRand(0.5f);
            rnd.y += 2.0f;
            this->vectors[this->curNumParticles] = glm::vec4(rnd, 0.0f);
            this->curNumParticles++;
        }
    }
}

//------------------------------------------------------------------------------
void
InstancingApp::updateParticles() {
    const float32 frameTime = 1.0f / 60.0f;
    for (int32 i = 0; i < this->curNumParticles; i++) {
        auto& pos = this->positions[i];
        auto& vec = this->vectors[i];
        vec.y -= 1.0f * frameTime;
        pos += vec * frameTime;
        if (pos.y < -2.0f) {
            pos.y = -1.8f;
            vec.y = -vec.y;
            vec *= 0.8f;
        }
    }
}

//------------------------------------------------------------------------------
AppState::Code
InstancingApp::OnInit() {
    // setup rendering system
    Gfx::Setup(GfxSetup::Window(800, 500, "Oryol Instancing Sample"));
    Dbg::Setup();
    Input::Setup();
    
    // check instancing extension
    if (!Gfx::Supports(GfxFeature::Instancing)) {
        o_error("ERROR: instanced_arrays extension required!\n");
    }

    // create dynamic instance data mesh
    auto instanceMeshSetup = MeshSetup::Empty(MaxNumParticles, Usage::Stream);
    instanceMeshSetup.Layout.Add(VertexAttr::Instance0, VertexFormat::Float4);
    this->instanceMesh = Gfx::Resource().Create(instanceMeshSetup);
    
    // setup static draw state
    const glm::mat4 rot90 = glm::rotate(glm::mat4(), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    ShapeBuilder shapeBuilder;
    shapeBuilder.RandomColors = true;
    shapeBuilder.Layout
        .Add(VertexAttr::Position, VertexFormat::Float3)
        .Add(VertexAttr::Color0, VertexFormat::Float4);
    shapeBuilder.Transform(rot90).Sphere(0.05f, 3, 2).Build();
    auto shapeBuilderResult = shapeBuilder.Result();
    shapeBuilderResult.Setup.InstanceMesh = this->instanceMesh;
    Id mesh = Gfx::Resource().Create(shapeBuilderResult);
    Id prog = Gfx::Resource().Create(Shaders::Main::CreateSetup());
    auto dss = DrawStateSetup::FromMeshAndProg(mesh, prog);
    dss.RasterizerState.CullFaceEnabled = true;
    dss.DepthStencilState.DepthWriteEnabled = true;
    dss.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
    this->drawState = Gfx::Resource().Create(dss);
    
    // setup projection and view matrices
    const float32 fbWidth = (const float32) Gfx::DisplayAttrs().FramebufferWidth;
    const float32 fbHeight = (const float32) Gfx::DisplayAttrs().FramebufferHeight;
    this->proj = glm::perspectiveFov(glm::radians(45.0f), fbWidth, fbHeight, 0.01f, 100.0f);
    this->modelViewProj = this->proj * this->view * this->model;
    
    return App::OnInit();
}

//------------------------------------------------------------------------------
AppState::Code
InstancingApp::OnCleanup() {
    Input::Discard();
    Dbg::Discard();
    Gfx::Discard();
    return App::OnCleanup();
}
