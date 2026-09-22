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
	win_text = text_renderer.make_text("YOU ESCAPED.");
	win_help_text = text_renderer.make_text("PRESS ENTER TO RESTART");
	glitch_text_1 = text_renderer.make_text("DO YOU WANT TO LEAVE?");
	glitch_text_2 = text_renderer.make_text("D# Y0U W@NT T# L$AVE?");
	glitch_text_3 = text_renderer.make_text("YOU CANNOT LEAVE.");
	glitch_stay = text_renderer.make_text("> STAY.");

	cursor_text =text_renderer.make_text(">");
    story = {
		//0
		{
			"You wake up in a dark room.",
			{
				{"Open the door", 1},
				{"Check the terminal", 2},
				{"Wait", 3}
			}
		},
		//1
		{
			"The door is locked.",
			{
				{"Go back", 0}
			}
		},
		//2
		{
			"The terminal flickers to life.",
			{
				{"Read the message", 4},
				{"Go back", 0}
			}
		},
		//3
		{
			"You wait. Nothing happens.",
			{
				{"Go back", 0}
			}
		},
		//4
		{
			"WELCOME, USER.",
			{
				{"Continue", 5}
			}
		},
		//5
		{
			"Something feels wrong.",
			{
				{"Continue", 6}
			}
		},
		//6 - GLITCH NODE
		{
			"The exit is right in front of you.",
			{
				{"LEAVE", 7},
				{"STAY", 0}
			}
		},
		//7
		{
			"YOU CANNOT LEAVE.",
			{
				{"LEAVE ANYWAY", 8},
				{"STAY*", 9}
			}
		},
		//8
		{
			"CONNECTION TERMINATED.",
			{
				{"RESTART", 0}
			}
		},
		//9
		{
			"YOU ESCAPED ?",
			{
			}
		}
	};
	enter_node(0);
    //std::cout<< "Created text texture: "<< test_text.texture<< " ("<< test_text.width<< " x "<< test_text.height<< ")"<< std::endl;
}

PlayMode::~PlayMode() {
    text_renderer.destroy_text(node_text);
    for (TextTexture &texture : choice_textures) {
        text_renderer.destroy_text(texture);
    }
    text_renderer.destroy_text(cursor_text);
    // text_renderer.destroy_text(title_text);
    // text_renderer.destroy_text(help_text);
    text_renderer.destroy_text(glitch_text_1);
    text_renderer.destroy_text(glitch_text_2);
    text_renderer.destroy_text(glitch_text_3);
    text_renderer.destroy_text(glitch_stay);
	text_renderer.destroy_text(win_text);
	text_renderer.destroy_text(win_help_text);
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {

	if (evt.type == SDL_EVENT_KEY_DOWN) {
        if (evt.key.key == SDLK_ESCAPE) {
            return true;
        }
    }

	if (win) {
		// if (evt.type == SDL_EVENT_KEY_DOWN && evt.key.key == SDLK_RETURN) {
		// 	win = false;
		// 	enter_node(0);
		// }
		return true;
	}

	if (glitch_mode) {
		return true;
	}

	if (evt.type == SDL_EVENT_KEY_DOWN) {
		if (evt.key.key == SDLK_DOWN) {
			StoryNode const &node = story[current_node];
			if (!node.choices.empty()) {
				selected_choice += 1;
				if (selected_choice >= static_cast<int>(node.choices.size())) {
					selected_choice = 0;
				}
			}
			return true;
		}
	}
	
	if (evt.type == SDL_EVENT_KEY_DOWN) {
		if (evt.key.key == SDLK_UP) {
			StoryNode const &node =story[current_node];
			if (!node.choices.empty()) {
				selected_choice -= 1;
				if (selected_choice < 0) {
					selected_choice =static_cast<int>(node.choices.size()) - 1;
				}
			}
			return true;
		}
	}

	if (evt.type == SDL_EVENT_KEY_DOWN) {
		if (evt.key.key == SDLK_RETURN) {
			StoryNode const &node =story[current_node];
			if (!node.choices.empty()) {
				Choice const &choice =node.choices[selected_choice];
				// Check for glitch mode trigger
				if ((current_node == 6 && selected_choice == 0) || (current_node == 7 && selected_choice == 0)) {
					glitch_mode = true;
					glitch_timer = 0.0f;
					return true;
				}
				if (choice.next_node == 8) {
					win = true;
					return true;
				}
				if (choice.next_node >= 0) {
					enter_node(choice.next_node);
				}
			}
			return true;
		}
	}

    return false;
}

void PlayMode::update(float elapsed) {
    if (glitch_mode) {
        glitch_timer += elapsed;
        if (glitch_timer >= 4.0f) {
            glitch_mode = false;
            glitch_timer = 0.0f;
			if( current_node == 7) {
				enter_node(8); 
				return;
			}
            enter_node(7);
        }
    }
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {

	glm::vec3 green(0.0f, 1.0f, 0.0f);
    glViewport(0, 0, drawable_size.x, drawable_size.y);
	glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

	if (win) {
		text_renderer.draw_text(
			win_text,
			glm::vec2(80.0f, 180.0f),
			1.3f,
			green,
			drawable_size
		);

		text_renderer.draw_text(
			win_help_text,
			glm::vec2(80.0f, 300.0f),
			0.7f,
			green,
			drawable_size
		);
		return;
	}

	//glitch mechanic drawing:
	if (glitch_mode) {
		TextTexture const *current_glitch = nullptr;
		if (glitch_timer < 1.0f) {
			current_glitch = &glitch_text_1;
			text_renderer.draw_text(
				node_text,
				glm::vec2(80.0f, 100.0f),
				1.0f,
				green,
				drawable_size
			);
			text_renderer.draw_text(
				glitch_stay,
				glm::vec2(80.0f, 220.0f),
				0.8f,
				green,
				drawable_size
			);
			return;
		} 
		else if (glitch_timer < 2.0f) {
			current_glitch = &glitch_text_1;
		} 
		else if(glitch_timer < 3.0f) {
			current_glitch = &glitch_text_2;
		}
		else{
			current_glitch = &glitch_text_3;
		}
		text_renderer.draw_text(
			*current_glitch,
			glm::vec2(80.0f, 100.0f),
			1.0f,
			glm::vec3(0.0f, 1.0f,0.0f),
			drawable_size
		);
		text_renderer.draw_text(
			glitch_stay,
			glm::vec2(80.0f, 220.0f),
			0.8f,
			green,
			drawable_size
		);
		return;
	}

	//draw story text:
	text_renderer.draw_text(
    node_text,
    glm::vec2(80.0f, 100.0f),
    1.0f,
    green,
    drawable_size
	);

	//draw choices:
	float choice_y = 220.0f;
	for (size_t i = 0; i < choice_textures.size();++i) {
		text_renderer.draw_text(
			choice_textures[i],
			glm::vec2(120.0f, choice_y),
			0.8f,
			green,
			drawable_size
		);
		choice_y += 60.0f;
	}

	//draw cursor:
	float cursor_y = 220.0f + selected_choice * 60.0f;
	text_renderer.draw_text(
		cursor_text,
		glm::vec2(80.0f, cursor_y),
		0.8f,
		green,
		drawable_size
	);

    GL_ERRORS();
}

void PlayMode::enter_node(int node_index) {

	//destroy old node texture:
    text_renderer.destroy_text(node_text);
    //destroy old choice textures:
    for (TextTexture &texture : choice_textures) {
        text_renderer.destroy_text(texture);
    }
    choice_textures.clear();

    current_node = node_index;
    selected_choice = 0;
    StoryNode const &node = story[current_node];
    node_text = text_renderer.make_text(node.text);
	for (Choice const &choice : node.choices) {
		choice_textures.push_back(text_renderer.make_text(choice.text));
	}
}