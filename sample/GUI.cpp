#include <iostream>
#include <array>
#include <algorithm>
#include <random>

#include <SDL2/SDL.h>

#include <GL/glew.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glk/util.h>
#include <glk/gl/util.h>
#include <glk/gl/Texture.h>
#include <Shader.h>
#include <Map.h>
#include <RayTracer.h>
#include <Mirror.h>

namespace gl = glk::gl;

namespace {
	constexpr int const WIDTH = 1024, HEIGHT = 768;
}

float const TILE_SZ = 48.0f;

glm::vec2 const g_screenScale{TILE_SZ / WIDTH, TILE_SZ / HEIGHT};

std::array<glm::vec2, 4> const unitSquare{{{0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}}};

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

int main(int, char**) {

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

	Shader rayProgram("data/ray.vert", "data/ray.frag");
	rayProgram.load();
	GLint rayPvmLoc = glGetUniformLocation(rayProgram.getProgramID(), "screenScale");
	GLint rayTilemapLoc = glGetUniformLocation(rayProgram.getProgramID(), "tilemap");
	CHECK_GL_ERROR();

	Map map(15, 15);

	GLuint rayVao, rayVbo;
	TRY_GL(glGenVertexArrays(1, &rayVao));
	TRY_GL(glGenBuffers(1, &rayVbo));


	TRY_GL(glBindVertexArray(rayVao));
	TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, rayVbo));

	TRY_GL(glEnableVertexAttribArray(0u));
	TRY_GL(glVertexAttribPointer(0u, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid *>(0)));

	TRY_GL(glBufferData(GL_ARRAY_BUFFER, 1100 * sizeof(glm::vec2), nullptr, GL_DYNAMIC_DRAW));

	GLuint spriteVao, spriteVbo;
	TRY_GL(glGenVertexArrays(1, &spriteVao));
	TRY_GL(glGenBuffers(1, &spriteVbo));


	TRY_GL(glBindVertexArray(spriteVao));
	TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, spriteVbo));

	TRY_GL(glEnableVertexAttribArray(0u));
	TRY_GL(glVertexAttribPointer(0u, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid *>(0)));

	TRY_GL(glBufferData(GL_ARRAY_BUFFER, unitSquare.size() * sizeof(glm::vec2), unitSquare.data(), GL_STATIC_DRAW));

	Shader spriteProgram("data/sprite.vert", "data/sprite.frag");
	spriteProgram.load();
	GLint spriteScaleLoc = glGetUniformLocation(spriteProgram.getProgramID(), "screenScale");
	GLint spriteTilemapLoc = glGetUniformLocation(spriteProgram.getProgramID(), "tilemap");
	GLint spriteModMatLoc = glGetUniformLocation(spriteProgram.getProgramID(), "modelMatrix");
	GLint spriteFrameLoc = glGetUniformLocation(spriteProgram.getProgramID(), "frame");
	GLint spriteSpriteSheetLoc = glGetUniformLocation(spriteProgram.getProgramID(), "spritesheet");
	CHECK_GL_ERROR();

	glk::gl::Texture objectSprites{"data/objects.png"};

	glm::vec2 rayOrig(5.5f, 4.5f);

	auto const *kbd = SDL_GetKeyboardState(nullptr);

	SDL_Event ev;
	bool loop = true;
	do {
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT
			    || (ev.type == SDL_KEYDOWN && ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
				loop = false;
		}

		float const SPD = 0.1f;

		if (kbd[SDL_SCANCODE_LEFT]) rayOrig.x -= SPD;
		if (kbd[SDL_SCANCODE_RIGHT]) rayOrig.x += SPD;
		if (kbd[SDL_SCANCODE_UP]) rayOrig.y -= SPD;
		if (kbd[SDL_SCANCODE_DOWN]) rayOrig.y += SPD;

		TRY_GL(glClear(GL_COLOR_BUFFER_BIT));

		int mx, my;
		SDL_GetMouseState(&mx, &my);
		glm::vec2 mouse = glm::vec2(mx, my) / TILE_SZ - rayOrig;


        map.addObject(std::make_unique<Mirror>(glm::vec2(3), glm::vec2(0.0f)));
        Ray ray = RayTracer::trace(map, rayOrig, normalize(mouse));

		std::vector<glm::vec2> &rayVerts = ray;

		TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, rayVbo));
		TRY_GL(glBufferSubData(GL_ARRAY_BUFFER, 0, rayVerts.size() * sizeof(glm::vec2), rayVerts.data()));

		map.display();

		TRY_GL(glUseProgram(rayProgram.getProgramID()));
		TRY_GL(glUniform2fv(rayPvmLoc, 1, glm::value_ptr(g_screenScale)));
		TRY_GL(glUniform1i(rayTilemapLoc, 0));

		map.tilemap().bind(GL_TEXTURE0);

		TRY_GL(glBindVertexArray(rayVao));
		TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, rayVbo));
		TRY_GL(glDrawArrays(GL_LINE_STRIP, 0, rayVerts.size()));

		glm::mat3 spritePvm = makeMatrix(10.0f, 3.5f, 60.0f);

		objectSprites.bind(GL_TEXTURE1);

		TRY_GL(glUseProgram(spriteProgram.getProgramID()));
		TRY_GL(glUniform2fv(spriteScaleLoc, 1, glm::value_ptr(g_screenScale)));
		TRY_GL(glUniformMatrix3fv(spriteModMatLoc, 1, GL_FALSE, glm::value_ptr(spritePvm)));
		TRY_GL(glUniform1i(spriteTilemapLoc, 0));
		TRY_GL(glUniform1i(spriteSpriteSheetLoc, 1));

		TRY_GL(glBindVertexArray(spriteVao));
		TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, spriteVbo));

		TRY_GL(glDrawArrays(GL_TRIANGLE_STRIP, 0, unitSquare.size()));

		SDL_GL_SwapWindow(sdlWindow);
		SDL_Delay(16u);
	} while (loop);

	SDL_Quit();

	return 0;
}
