#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

#include "Application.h"
#include "Camera.h"
#include <fmod.hpp>
#include <fmod.h>

class SoundProgramming : public Application
{
public:
    virtual bool startup();
    virtual void shutdown();
    virtual bool update();
    virtual void draw();

	bool CheckResult(FMOD_RESULT);

    FlyCamera m_camera;
	FMOD::System* m_FMSystem;
};

#endif //CAM_PROJ_H_