#include "stdafx.h"

#include "tests/CornellBoxScene.h"
#include "tests/TestScene1.h"
#include "tests/TestScene2.h"
#include "tests/ProceduralScene.h"
#include "tests/DragonScene.h"

#include "OpenGLWindowsRaytracerUI.h"
#include "ManualTests.h"
#include "Benchmarks.h"

#include <iostream>
#include <string>

#include <Windows.h>


// MAX_TRACE_DEPTH = 969 already causes stackoverflows
// MAX_TRACE_DEPTH > 31 for most scenes no significant visual change
#ifdef NDEBUG
const ASizeT FAST_PREVIEW_SIZE = 512;
const ASizeT MAX_TRACE_DEPTH = 31;
#else
const ASizeT FAST_PREVIEW_SIZE = 64;
const ASizeT MAX_TRACE_DEPTH = 0;
#endif

const Float PERSPECTIVE_FOV = 45.f;

const Float2 PERSPECTIVE_Z_PLANES = Float2(.01f, 2.f);

const Float2 PERSPECTIVE_Z_PLANE_EXTENDS = Float2(2.f);

const Float3 CAMERA_INIT_TRANSLATION = Float3(Zero<Float>(), Zero<Float>(), -3.2f);

// x => x * x * screen-pixels == image-pixels
const Float SAMPLING_FACTOR = 1.f;

// n => (n+1)x(n+1) samples/image-pixel
const ASizeT SUPER_SAMPLING_FACTOR = 0;

const Float MAX_DISTANCE = std::numeric_limits<Float>::max();

const Int CULLING_ORIENTATION = -1;

const Float VISIBILITY_CUTOFF = reciprocal(512.f);

// n => (n+1)x(n+1) image-pixels/packet
const ASizeT RAY_PACKET_SIZE = 15;


namespace raytracerui {

	const LRESULT runRaytracerUI() {
		Resources * resources = new Resources();
		Scene * scene = new Scene(
			new NaiveKDTreeTraverser<SceneIntersection>(),
			new FixedIterationsSAHKDTreeBalancer()
		);

		CornellBoxScene::setup(*scene, *resources);
		//TestScene1::setup(*scene, *resources);
		//TestScene2::setup(*scene, *resources);
		//DragonScene::setup(*scene, *resources);
		//ProceduralScene<3, 10>::setup(*scene, *resources);

		scene->buildSceneGraph();

		Camera * camera = new Camera();
		camera->setProjection(PERSPECTIVE_FOV, PERSPECTIVE_Z_PLANE_EXTENDS, PERSPECTIVE_Z_PLANES);
		camera->translate(CAMERA_INIT_TRANSLATION);

		RaytraceParameters * parameters = new RaytraceParameters();
		parameters->visibilityCutoff = VISIBILITY_CUTOFF;
		parameters->maxDistance = MAX_DISTANCE;
		parameters->maxTraceDepth = MAX_TRACE_DEPTH;
		parameters->sceneShader = scene;
		parameters->cullingOrientation = CULLING_ORIENTATION;
		parameters->perspectiveZPlanes = PERSPECTIVE_Z_PLANES;
		parameters->perspectiveFOV = PERSPECTIVE_FOV;
		parameters->samplingFactor = SAMPLING_FACTOR;
		parameters->supersamplingFactor = SUPER_SAMPLING_FACTOR;
		parameters->rayPacketSize = RAY_PACKET_SIZE;
		parameters->camera = camera;

		Raytracer * raytracer = new Raytracer();

		MessageLoopBasedUI * ui = new WindowsRaytracerUI(*raytracer, *parameters, FAST_PREVIEW_SIZE);
		LRESULT returnCode = static_cast<LRESULT>(-1);
		try {
			returnCode = ui->run();
		}
		catch (const std::exception & exception) {
			std::cout << exception.what() << std::endl;
		}

		delete ui;
		delete raytracer;
		delete parameters;
		delete camera;
		delete scene;
		delete resources;

		return returnCode;
	}

}


int __cdecl main()
{
	TCHAR currentPath[MAX_PATH + 1] = { 0 };
	GetCurrentDirectory(MAX_PATH, currentPath);

	std::wcout << "Working directory: " << std::wstring(currentPath) << std::endl;

	raytracerui::ManualTests()();
	//raytracerui::Benchmarks()();

	return static_cast<int>(raytracerui::runRaytracerUI());
}
