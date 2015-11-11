#include <iostream>
#include <array>
#include <algorithm>
#include <random>

#include <SDL2/SDL.h>

#include <GL/glew.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

#include <glk/util.h>
#include <glk/gl/util.h>
#include <glk/gl/Texture.h>
#include <Shader.h>
#include <Map.h>

namespace gl = glk::gl;

namespace {
	constexpr int const WIDTH = 1024, HEIGHT = 768;
}

glm::vec2 g_camSize{WIDTH / 32, HEIGHT / 32};

glm::mat3 g_worldToView{
	1.0f / g_camSize.x, 0.0f, 0.0f,
	0.0f, 1.0f / g_camSize.y, 0.0f,
	0.0f, 0.0f, 1.0f
};

glm::mat3 g_viewToScreen{
	2.0f, 0.0f, 0.0f,
	0.0f, -2.0f, 0.0f,
	-1.0f, 1.0f, 1.0f
};

glm::mat3 g_pvMat = g_viewToScreen * g_worldToView;

int main(int , char **) {

	// Initialize the OpenGL context --------------------------------------
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	auto sdlWindow = SDL_CreateWindow("glk Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
	                                  SDL_WINDOW_OPENGL);

	auto sdlGlContext = SDL_GL_CreateContext(sdlWindow);
	(void) sdlGlContext;

	SDL_ShowWindow(sdlWindow);

	glewInit();

	TRY_GL(glPixelStorei(GL_PACK_ALIGNMENT, 1));
	TRY_GL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

	TRY_GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	TRY_GL(glEnable(GL_BLEND));
	TRY_GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	// ----------------------------------------------------------------

	Map<> map(10, 10);

	SDL_Event ev;
	bool loop = true;
	int ticker = 0;
	do {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT
			    || (ev.type == SDL_KEYDOWN && ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
				loop = false;
		}

		TRY_GL(glClear(GL_COLOR_BUFFER_BIT));

		map.display();

		SDL_GL_SwapWindow(sdlWindow);
		++ticker;
		SDL_Delay(16u);
	} while (loop);

	SDL_Quit();

	return 0;
}
