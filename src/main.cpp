#include <iostream>
#include <array>

#include <SDL2/SDL.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

#include <glk/util.h>
#include <glk/gl/Program.h>
#include <glk/gl/Texture.h>

#include <glk/gl/Instancer.h>

// Program for displaying sprites
struct {
	glk::gl::Program prog;
	glk::gl::Uniform<glm::mat3> pvMatrix;
	glk::gl::Uniform<glk::gl::sampler2D> spritesheet;
} spritesProgram;

namespace gl = glk::gl;

// Transform matrix factory
glm::mat3 makeMatrix(float x, float y, float rot) {
	glm::mat3 tm;

	float angle = glk::deg2rad(rot);
	float s = std::sin(angle);
	float c = std::cos(angle);

	tm[0][0] = c;
	tm[1][1] = c;
	tm[0][1] = -s;
	tm[1][0] = s;

	tm[2][0] = x - (tm[0][0] * 0.5f + tm[1][0] * 0.5f);
	tm[2][1] = y - (tm[0][1] * 0.5f + tm[1][1] * 0.5f);

	return tm;
}

//fmtoff
GLK_GL_ATTRIB_STRUCT(
SpriteVertex,
	(glm::vec2, pos)
	(glm::vec4, col)
);

// Attributes for one sprite
GLK_GL_ATTRIB_STRUCT(
	SpriteAttribs,
	(glm::mat3, transform)
	(glm::vec2, tlUv)
	(glm::vec2, brUv)
	(glm::vec4, color)
);
//fmton

// Billboard for one sprite
std::array<SpriteVertex, 4> spriteVertices{{
	                                           {{0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
	                                           {{1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
	                                           {{0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
	                                           {{1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}
                                           }};

int main() {
	constexpr int const WIDTH = 1024, HEIGHT = 768;

	// Initialize the OpenGL context --------------------------------------
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	auto sdlWindow = SDL_CreateWindow(
		"glk Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL
	);

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

	// Setup the program ----------------------------------------------
	auto spriteVert = gl::VertexShader::fromFile("data/sprite.vert");
	auto spriteFrag = gl::FragmentShader::fromFile("data/sprite.frag");
	spritesProgram.prog = gl::ProgramBuilder().attach(spriteVert).attach(spriteFrag).link();
	spritesProgram.pvMatrix = spritesProgram.prog.uniform<glm::mat3>("pvMatrix");
	spritesProgram.spritesheet = spritesProgram.prog.uniform<gl::usampler2D>("spritesheet");
	// ----------------------------------------------------------------

	// Set up DisplayQueue --------------------------------------------
	gl::Vbo <SpriteVertex> instancer{begin(spriteVertices), end(spriteVertices)};
	gl::InstanceQueue <SpriteVertex, SpriteAttribs> displayQueue{instancer};
	// ----------------------------------------------------------------140,

	gl::Texture tex{"data/tex.png"};

	glm::vec2 camSize{WIDTH / 64, HEIGHT / 64};

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

		displayQueue.clear();
		for (int i = 0; i < 8; ++i)
			displayQueue.enqueue(SpriteAttribs{
				makeMatrix(6 + i, 6, ticker + i * 15),
				glm::vec2(0.0f), glm::vec2(1.0f),
				glm::vec4(1.0f)
			});

		displayQueue.upload();

		tex.bind(GL_TEXTURE2);

		spritesProgram.prog.use();
		spritesProgram.pvMatrix = pvMat;
		spritesProgram.spritesheet = 2u;

		displayQueue.display();

		SDL_GL_SwapWindow(sdlWindow);
		++ticker;
		SDL_Delay(16u);
	} while (loop);

	SDL_Quit();
	return 0;
}
