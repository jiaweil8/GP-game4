#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <deque>

#include "TextRenderer.hpp"

struct Choice {
    std::string text;
    int next_node = -1;
};

struct StoryNode {
    std::string text;
    std::vector<Choice> choices;
};



struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----
	bool win = false;
	TextTexture win_text;
	TextTexture win_help_text;

	//text renderer:
	TextRenderer text_renderer;
	TextTexture test_text;
	TextTexture node_text;
	TextTexture cursor_text;
	std::vector<TextTexture> choice_textures;

	//choices and story nodes:
	std::vector<StoryNode> story;
	int current_node = 0;
	int selected_choice = 0;
	void enter_node(int node_index);

	//glitch
	bool glitch_mode = false;
	float glitch_timer = 0.0f;
	TextTexture glitch_text_1;
	TextTexture glitch_text_2;
	TextTexture glitch_text_3;
	TextTexture glitch_stay;
};
