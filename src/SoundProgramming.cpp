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
		printf("FMOD error! (%d) %s\n", result);
		return false;
	}

	//Initalize with 512 channels
	result = m_FMSystem->init(512, FMOD_INIT_NORMAL, 0);

	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result);
		return false;
	}

	//Create Sounds
	FMOD::Sound* WonkaSound;
	FMOD::Sound* ZanarSound;

	result = m_FMSystem->createStream("data/audio/Willy Wonka - Pure Imagination (Trap Remix).mp3", FMOD_DEFAULT | FMOD_3D, 0, &WonkaSound);
	result = m_FMSystem->createStream("data/audio/To Zanarkand (ORCHESTRA).mp3", FMOD_DEFAULT | FMOD_3D, 0, &ZanarSound);
	
	//Cerate Channels and Group
	FMOD::Channel* WonkaChannel;
	FMOD::Channel* ZanerkandChannel;
	FMOD::ChannelGroup* MusicChannelGroup;

	FMOD_VECTOR WonkaPosition = { 0, 20, 0 };
	FMOD_VECTOR WonkaVelocity = { 0, 0, 0 };

	result = m_FMSystem->createChannelGroup("Music", &MusicChannelGroup);
	
	//Play Sounds
	result = m_FMSystem->playSound(WonkaSound, MusicChannelGroup, false, &WonkaChannel);
	result = m_FMSystem->playSound(ZanarSound, MusicChannelGroup, false, &ZanerkandChannel);

	result = WonkaChannel->set3DAttributes(&WonkaPosition, &WonkaVelocity, 0);
	

	FMOD_VECTOR position = {0,0,0};
	FMOD_VECTOR velocity = { 0,0,0 };
	FMOD_VECTOR forward = { 1,0,0 };
	FMOD_VECTOR up = { 0,1,0 };

	m_FMSystem->set3DListenerAttributes(0, &position, &velocity, &forward, &up);

	FMOD::DSP* pEchoDSP;
	result = m_FMSystem->createDSPByType(FMOD_DSP_TYPE_ECHO, &pEchoDSP);
	CheckResult(result);

	result = ZanerkandChannel->addDSP(0, pEchoDSP);
	CheckResult(result);

    return true;
}

void SoundProgramming::shutdown()
{
    Gizmos::destroy();

	m_FMSystem->close();
	m_FMSystem->release();

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

	//Update 
	FMOD_VECTOR NewPos = { m_camera.world[3][0], m_camera.world[3][1], m_camera.world[3][2] };
	FMOD_VECTOR velocity = { 0,0,0 };
	FMOD_VECTOR forward = { 1,0,0 };
	FMOD_VECTOR up = { 0,1,0 };

	m_FMSystem->set3DListenerAttributes(0, &NewPos, &velocity, &forward, &up);

	m_FMSystem->update();

    return true;
}

void SoundProgramming::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Gizmos::draw(m_camera.proj, m_camera.view);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

bool SoundProgramming::CheckResult(FMOD_RESULT result) {
	if (result != FMOD_OK) {
		printf("FMOD error! (%d) %s\n", result);
		return false;
	}

	else {
		printf("K");
		return true;
	}
}