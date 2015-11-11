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
#include <glk/gl/Instancer.h>
#include <Shader.h>

namespace gl = glk::gl;

// Billboard for one sprite
std::array<glm::vec2, 4> const mapVerts{{
	                                        {0.0f, 0.0f},
	                                        {10.0f, 0.0f},
	                                        {0.0f, 10.0f},
	                                        {10.0f, 10.0f}
                                        }};

int main() {
	constexpr int const WIDTH = 1024, HEIGHT = 768;

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

	Shader mapProgram("data/tilemap.vert", "data/tilemap.frag");
	CHECK_GL_ERROR();
	TRY_GL(mapProgram.load());

	GLint pvmLoc = glGetUniformLocation(mapProgram.getProgramID(), "pvmMatrix");
	GLint tilemapLoc = glGetUniformLocation(mapProgram.getProgramID(), "tilemap");
	CHECK_GL_ERROR();

	glk::gl::Texture mapTex;

	constexpr int const mapWidth = 10, mapHeight = 10;

	mapTex.bind();
	TRY_GL(glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, mapWidth, mapHeight));
	mapTex.unbind();

	std::array<std::uint32_t, mapWidth * mapHeight> mapData;

	std::mt19937 rng;
	rng.seed(1447054199);

	std::generate(begin(mapData), end(mapData), [&rng] {
		return std::uniform_int_distribution<>{0, 1}(rng);
	});

	// Update
	mapTex.bind();
	TRY_GL(
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mapWidth, mapHeight, GL_RED_INTEGER, GL_UNSIGNED_INT, mapData.data()));
	mapTex.unbind();

	glk::gl::Vbo<glm::vec2> mapVbo{begin(mapVerts), end(mapVerts)};
	glk::gl::VaoName mapVao;

	glm::vec2 camSize{WIDTH / 32, HEIGHT / 32};

	glm::mat3 worldToView{
		1.0f / camSize.x, 0.0f, 0.0f,
		0.0f, 1.0f / camSize.y, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	glm::mat3 viewToScreen{
		2.0f, 0.0f, 0.0f,
		0.0f, -2.0f, 0.0f,
		-1.0f, 1.0f, 1.0f
	};

	glm::mat3 pvMat = viewToScreen * worldToView;

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

		TRY_GL(glUseProgram(mapProgram.getProgramID()));
		TRY_GL(glUniformMatrix3fv(pvmLoc, 1, GL_FALSE, glm::value_ptr(pvMat)));
		TRY_GL(glUniform1i(tilemapLoc, 0));

		mapTex.bind(GL_TEXTURE0);

		TRY_GL(glBindVertexArray(mapVao));
		TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, mapVbo.name()));
		TRY_GL(glEnableVertexAttribArray(0u));
		glk::gl::setAttribPointers<glm::vec2>(0);
		TRY_GL(glDrawArrays(GL_TRIANGLE_STRIP, 0, mapVbo.size()));

		SDL_GL_SwapWindow(sdlWindow);
		++ticker;
		SDL_Delay(16u);
	} while (loop);

	SDL_Quit();
	return 0;
}
