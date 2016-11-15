#include "SoundProgramming.h"

#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"
#include <stdio.h>

bool SoundProgramming::startup()
{
    if (Application::startup() == false)
    {
        return false;
    }

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    Gizmos::create();

    m_camera = FlyCamera(1280.0f / 720.0f, 10.0f);
    m_camera.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
    m_camera.sensitivity = 3;

	FMOD_RESULT result;
	//Create Main system
	result = FMOD::System_Create(&m_FMSystem);

	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result);		return false;
	}

	//Initalize with 512 channels
	result = m_FMSystem->init(512, FMOD_INIT_NORMAL, 0);

	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result);		return false;
	}

	FMOD::Sound* pSound;
	result = m_FMSystem->createStream("data/audio/Willy Wonka - Pure Imagination (Trap Remix).mp3", FMOD_DEFAULT, 0, &pSound);
	FMOD::Channel* pChannel;
	result = m_FMSystem->playSound(pSound, 0, false, &pChannel);

	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result);
	}

	else {
		printf("K");
	}

    return true;
}

void SoundProgramming::shutdown()
{
    Gizmos::destroy();

    Application::shutdown();
}

bool SoundProgramming::update()
{
    if (Application::update() == false)
    {
        return false;
    }

    Gizmos::clear();

    float dt = (float)glfwGetTime();
    glfwSetTime(0.0);

    vec4 white(1);
    vec4 black(0, 0, 0, 1);
    for (int i = 0; i <= 20; ++i)
    {
        Gizmos::addLine(vec3(-10 + i, -0.01, -10), vec3(-10 + i, -0.01, 10),
            i == 10 ? white : black);
        Gizmos::addLine(vec3(-10, -0.01, -10 + i), vec3(10, -0.01, -10 + i),
            i == 10 ? white : black);
    }

    m_camera.update(dt);

    return true;
}

void SoundProgramming::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Gizmos::draw(m_camera.proj, m_camera.view);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}