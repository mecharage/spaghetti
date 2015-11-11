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
#include <RayTracer.h>

namespace gl = glk::gl;

namespace {
	constexpr int const WIDTH = 1024, HEIGHT = 768;
}

float const TILE_SZ = 48.0f;

glm::vec2 g_camSize{WIDTH / TILE_SZ, HEIGHT / TILE_SZ};

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

glm::vec2 const g_screenScale{TILE_SZ / WIDTH, TILE_SZ / HEIGHT};

int main() {

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
	GLint rayPvmLoc = glGetUniformLocation(rayProgram.getProgramID(), "pvmMatrix");
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

		if(kbd[SDL_SCANCODE_LEFT])  rayOrig.x -= SPD;
		if(kbd[SDL_SCANCODE_RIGHT]) rayOrig.x += SPD;
		if(kbd[SDL_SCANCODE_UP])    rayOrig.y -= SPD;
		if(kbd[SDL_SCANCODE_DOWN])  rayOrig.y += SPD;

		TRY_GL(glClear(GL_COLOR_BUFFER_BIT));

		int mx, my;
		SDL_GetMouseState(&mx, &my);
		glm::vec2 mouse(mx, my);

		mouse /= TILE_SZ;
		mouse -= rayOrig;

		Ray ray(rayOrig, normalize(mouse));
		RayTracer::trace(ray, map);

		std::vector<glm::vec2> &rayVerts = ray;

		TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, rayVbo));
		TRY_GL(glBufferSubData(GL_ARRAY_BUFFER, 0, rayVerts.size() * sizeof(glm::vec2), rayVerts.data()));

		map.display();

		TRY_GL(glUseProgram(rayProgram.getProgramID()));
		TRY_GL(glUniformMatrix3fv(rayPvmLoc, 1, GL_FALSE, glm::value_ptr(g_pvMat)));
		TRY_GL(glUniform1i(rayTilemapLoc, 0));

		map.tilemap().bind(GL_TEXTURE0);

		TRY_GL(glBindVertexArray(rayVao));
		TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, rayVbo));
		TRY_GL(glDrawArrays(GL_LINE_STRIP, 0, rayVerts.size()));

		SDL_GL_SwapWindow(sdlWindow);
		SDL_Delay(16u);
	} while (loop);

	SDL_Quit();

	return 0;
}
