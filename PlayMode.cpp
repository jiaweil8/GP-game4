#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>




PlayMode::PlayMode(): text_renderer("dist/Roboto-Light.ttf")
{
    test_text = text_renderer.make_text( "Good night, sleep tight, don't let the bed bugs bite.");
    std::cout<< "Created text texture: "<< test_text.texture<< " ("<< test_text.width<< " x "<< test_text.height<< ")"<< std::endl;
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

    if (evt.type == SDL_EVENT_KEY_DOWN) {
        if (evt.key.key == SDLK_ESCAPE) {
            return true;
        }
    }
    return false;
}

void PlayMode::update(float elapsed) {
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
    glViewport(0, 0, drawable_size.x, drawable_size.y);
	glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
	text_renderer.draw_text(
    test_text,
    glm::vec2(80.0f, 80.0f),
    1.0f,
    glm::vec3(0.0f, 1.0f, 0.0f),
    drawable_size
	);
    GL_ERRORS();
}


