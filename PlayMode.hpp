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



	//text renderer:
	TextRenderer text_renderer;
	TextTexture test_text;
};
