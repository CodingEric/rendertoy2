#include <iostream>
#include <memory>

#include "rendertoy.h"
#include "logger.h"
#include "principled.h"

using namespace rendertoy;

int main()
{
    INFO << "Welcome to the executable program of RenderToy2!" << std::endl;

// TestRenderWork test_renderer(RenderConfig{.width = 1920, .height = 1080});
// test_renderer.Render();

// Image result = test_renderer.GetResult();
// result.Export("/Users/tyanyuy3125/Pictures/test.png");
#ifdef _WIN32
    auto ret = ImportMeshFromFile("E:/cornellbox.obj");
#else
    auto ret = ImportMeshFromFile("/Users/tyanyuy3125/cornellbox.obj");
// auto ret = ImportMeshFromFile("/Users/tyanyuy3125/monkey.obj");
#endif // _WIN32
    INFO << "Import done." << std::endl;

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    scene->objects().insert(scene->objects().end(), std::make_move_iterator(ret.begin()), std::make_move_iterator(ret.end()));
    // scene->objects().push_back(std::make_shared<UVSphere>(glm::vec3(0.0f), 1.0f));
    // scene->objects().push_back(std::make_shared<UVSphere>(glm::vec3(10.0f, 0.0f, 0.0f), 2.0f));
    // scene->objects().push_back(std::make_shared<UVSphere>(glm::vec3(-5.0f, 0.0f, 0.0f), 1.0f));
    // scene->objects().push_back(std::make_shared<UVSphere>(glm::vec3(-5.0f, -5.0f, 0.0f), 1.0f));
    // scene->objects().push_back(std::make_shared<UVSphere>(glm::vec3(-1.0f, 0.0f, 0.0f), 0.5f));

    std::shared_ptr<ISamplableColor> tex_white = std::make_shared<ColorTexture>(glm::vec4{1.0f});
    std::shared_ptr<ISamplableColor> tex_white2 = std::make_shared<ColorTexture>(glm::vec4{1.0f});
    std::shared_ptr<ISamplableColor> tex_black = std::make_shared<ColorTexture>(glm::vec4{0.4f, 0.0f, 0.0f, 1.0f});
    std::shared_ptr<ISamplableColor> tex_R = std::make_shared<ColorTexture>(glm::vec4{0.3f});
    std::shared_ptr<ISamplableColor> tex_red = std::make_shared<ColorTexture>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    std::shared_ptr<ISamplableColor> tex_green = std::make_shared<ColorTexture>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    std::shared_ptr<ISamplableNumerical> emissive_strength = std::make_shared<ConstantNumerical>(10.0f);
    std::shared_ptr<ISamplableNumerical> diffuse_roughness = std::make_shared<ConstantNumerical>(45.0f);
    std::shared_ptr<ISamplableNumerical> metal_roughness = std::make_shared<ConstantNumerical>(0.1f);
    std::shared_ptr<IMaterial> mat_white = std::make_shared<DiffuseBSDF>(tex_white, diffuse_roughness);
    std::shared_ptr<IMaterial> mat_red = std::make_shared<DiffuseBSDF>(tex_red, diffuse_roughness);
    std::shared_ptr<IMaterial> mat_green = std::make_shared<DiffuseBSDF>(tex_green, diffuse_roughness);
    std::shared_ptr<IMaterial> mat_emissive = std::make_shared<Emissive>(tex_white, emissive_strength);
    std::shared_ptr<IMaterial> mat_specular = std::make_shared<SpecularBSDF>(tex_white);
    std::shared_ptr<ISamplableColor> copper_eta = std::make_shared<ColorTexture>(glm::vec4(0.27105f, 0.67693f, 1.31640f, 1.0f));
    std::shared_ptr<ISamplableColor> copper_k = std::make_shared<ColorTexture>(glm::vec4(3.60920f, 2.62480f, 2.29210f, 1.0f));
    std::shared_ptr<ISamplableNumerical> water_eta = std::make_shared<ConstantNumerical>(1.1f);
    std::shared_ptr<IMaterial> mat_metal = std::make_shared<MetalBSDF>(tex_white, copper_eta, copper_k, metal_roughness, metal_roughness);
    std::shared_ptr<IMaterial> mat_glass = std::make_shared<RefractionBSDF>(tex_R, tex_white, water_eta);
    std::shared_ptr<ISamplableNumerical> glass_roughness = std::make_shared<ConstantNumerical>(0.1f);
    std::shared_ptr<IMaterial> mat_frosted_glass = std::make_shared<RefractionBSDF>(tex_black, tex_white2, water_eta, glass_roughness, glass_roughness);
    std::shared_ptr<ISamplableColor> test_tex = std::make_shared<ImageTexture>("./test.png");
    std::shared_ptr<ISamplableNumerical> test_tex_brightness = std::make_shared<Brightness>(test_tex);
    std::shared_ptr<IMaterial> mat_principled = std::make_shared<PrincipledBSDF>(
        tex_white,
        std::make_shared<ConstantNumerical>(0.75f),
        std::make_shared<ConstantNumerical>(1.333f),
        std::make_shared<ConstantNumerical>(1.0f),
        // test_tex_brightness,
        std::make_shared<ConstantNumerical>(1.0f),
        std::make_shared<ConstantNumerical>(0.0f),
        std::make_shared<ConstantNumerical>(1.0f),
        std::make_shared<ConstantNumerical>(0.5f),
        std::make_shared<ConstantNumerical>(0.0f),
        std::make_shared<ConstantNumerical>(0.0f),
        std::make_shared<ConstantNumerical>(0.0f),
        true,
        std::make_shared<ConstantNumerical>(0.0f),
        std::make_shared<ConstantNumerical>(0.2f));
    // std::shared_ptr<IMaterial> mat_metal;
    scene->objects()[0]->mat() = mat_white;
    scene->objects()[1]->mat() = mat_white;
    scene->objects()[2]->mat() = mat_white;
    dynamic_cast<TriangleMesh *>(scene->objects()[2].get())
        ->Animate(glm::quat(glm::mat3(1.0f)), glm::vec3(-0.5f, 0.0f, 0.0f), -0.25f, 0.25f);
    // dynamic_cast<TriangleMesh *>(scene->objects()[2].get())->_tran = glm::vec3(0.5f, 0.0f, 0.0f);
    scene->objects()[3]->mat() = mat_emissive;
    scene->objects()[4]->mat() = mat_red;
    scene->objects()[5]->mat() = mat_green;

    scene->Init();
    INFO << "Scene inited." << std::endl;

    std::shared_ptr<ISamplableColor> hdr_bg = std::make_shared<ColorTexture>(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    hdr_bg->SetSampleMethod(SampleMethod::BILINEAR);
    scene->hdr_background() = hdr_bg;

    std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{0.0f, 1.0f, 4.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::radians(45.0f), 16.0f / 9.0f);
    camera->lens_radius() = 0.0f;
    ImageTexture lens_mask("./lens_mask.png");
    camera->func_reject_lens_sampling = [&](const glm::vec2 &v) -> bool
    {
        if (lens_mask.Sample(v.x, v.y).x > 0.5f)
        {
            return true;
        }
        return false;
    };

    RenderConfig conf;
    conf.width = 1280;
    conf.height = 720;
    conf.camera = camera;
    conf.scene = scene;
    conf.x_sample = 4;
    conf.y_sample = 4;
    conf.spp = 4;
    conf.gamma = 2.4f;
    conf.time = 0.0f;
    NormalRenderWork renderwork(conf);
    renderwork.Render();
    Image result = renderwork.GetResult(true);
#ifdef _WIN32
    result.Export("E:/test.png");
#else
    result.Export("/Users/tyanyuy3125/Pictures/test.png");
#endif // _WIN32

    return 0;
}