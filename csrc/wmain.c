#include "util.h"

sg_buffer buffer;
sg_shader shader;
sg_pipeline pipeline;
sg_bindings resource_bindings;

sg_pass_action pass_action;

void init()
{
    sg_setup(&(sg_desc){
        .context = sapp_sgcontext()
    });

    const float vertices[] = {
        // positions            // colors
         0.0f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f
    };
    buffer = sg_make_buffer(&(sg_buffer_desc){
        .data = SG_RANGE(vertices)
    });

    shader = sg_make_shader(&(sg_shader_desc){
        .vs.source =
            "attribute vec4 position;\n"
            "attribute vec4 color0;\n"
            "out vec4 color;\n"
            "void main() {\n"
            "  gl_Position = position;\n"
            "  color = color0;\n"
            "}\n",
        .fs.source =
            "precision mediump float;\n"
            "in vec4 color;\n"
            "out vec4 frag_color;\n"
            "void main() {\n"
            "  frag_color = color;\n"
            "}\n"
    });

    pipeline = sg_make_pipeline(&(sg_pipeline_desc){
        .shader = shader,
        .layout = {
            .attrs = {
                [0].format=SG_VERTEXFORMAT_FLOAT3,
                [1].format=SG_VERTEXFORMAT_FLOAT4
            }
        }
    });

    resource_bindings = (sg_bindings) {
        .vertex_buffers[0] = buffer
    };


    pass_action = (sg_pass_action) {
        .colors[0] = { .action=SG_ACTION_CLEAR, .value={0.0f, 0.0f, 0.0f, 1.0f} }
    };
}

void frame()
{
    // float g = pass_action.colors[0].value.g + 0.01f;
    // pass_action.colors[0].value.g = (g > 1.0f) ? 0.0f : g;
    sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());
    sg_apply_pipeline(pipeline);
    sg_apply_bindings(&resource_bindings);
    sg_draw(0, 3, 1);
    sg_end_pass();
    sg_commit();
}

void cleanup()
{
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[]) {
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .width = 640,
        .height = 480,
        .window_title = "the ultimate anti-school weapon.",
    };
}