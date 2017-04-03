#include <iostream>

#include <glbinding/Binding.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/Version.h>

#include <SDL.h>
#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

int main(int argc, char ** argv)
{
	SDL_Window *        displayWindow;
	SDL_Renderer *      displayRenderer;
	SDL_RendererInfo    displayRendererInfo;
	SDL_GLContext       glContext;

	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	displayWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 1080, SDL_WINDOW_OPENGL);
	if (!displayWindow)
	{
		SDL_Quit();
		return -1;
	}

	glContext = SDL_GL_CreateContext(displayWindow);

//	SDL_GetRendererInfo(displayRenderer, &displayRendererInfo);
//
//	if ((displayRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
//		(displayRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
//		return -1;
//	}

	SDL_GL_MakeCurrent(displayWindow, glContext);
	
	// Init glbinding
	glbinding::Binding::initialize();

	//auto versionString = glbinding::ContextInfo::version().toString();
	//auto vendorString = glbinding::ContextInfo::vendor();
	auto rendererString = glbinding::ContextInfo::renderer();


	auto s = reinterpret_cast<const char *>(gl::glGetString(gl::GL_RENDERER));

	if (!s)
	{
		return -1;
	}

	std::cout << std::endl
	//	<< "OpenGL Version:  "/* << versionString <<*/<< std::endl;
	//	<< "OpenGL Vendor:   " << vendorString << std::endl
		<< "OpenGL Renderer: " << s << std::endl;

	gl::glClearColor(1.0f, 0.0f, 0.5f, 1.0f);
	gl::glClear(gl::GL_COLOR_BUFFER_BIT);

	while(true)
	{
		gl::glClear(gl::GL_COLOR_BUFFER_BIT);

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				return 0;

			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					return 0;
					break;
				case SDLK_r:
					// Cover with red and update
					gl::glClearColor(1.0, 0.0, 0.0, 1.0);
					break;
				case SDLK_g:
					// Cover with green and update
					gl::glClearColor(0.0, 1.0, 0.0, 1.0);
					break;
				case SDLK_b:
					// Cover with blue and update
					gl::glClearColor(0.0, 0.0, 1.0, 1.0);
					break;
				default:
					break;
				}
			}
		}

		SDL_GL_SwapWindow(displayWindow);
	}
}
