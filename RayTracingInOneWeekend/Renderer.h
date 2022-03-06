#pragma once


#include <iostream>
#include <vector>
#include <thread>
#include "constants.h"
#include "hittable_list.h"
#include "sphere.h"
#include "color.h"
#include "camera.h"
#include "material.h"
#include <chrono>
#include <string>
#include "ThreadPool.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void StartRender();
	void StartRenderThread();
	void ToggleRender();
	bool currentPauseState;
	void setStoppedState();

	color ray_color(ray& r, hittable_list& list, int depth);

	hittable_list random_scene();

	void SetWidthAndHeight(int width, int height);

	void SetTargetBuffer(uint8_t* const buffer) { m_renderBuffer = buffer; }

	void handleRendering(int startheight, int endheight, int threadNo);

	int getNumThreads();

	// this thread is in charge of synchronizing all the threads that do the rendering
	std::thread RenderThread;
	Pool m_threadPool{};
	int numThreadsToUse;
	// std::vector<std::thread> running_threads;

	
	enum RenderState { NotStarted, Running, Stopped, Finished };
	RenderState currentState;
	[[nodiscard]] RenderState getState() { return currentState; }
	

	// configuration
	double aspect_ratio;
	int image_width;
	int image_height;
	// for anti aliasing
	int samples_per_pixel;
	// more 'accurate' the larger this number is, but will take longer
	int child_ray_depth;
	hittable_list hl;

	int numThreads = -1;

	point3 currentCameraPosition;

	// statistics
	int getNumPixelsRendered();
	int numPixelsRendered;
	int numPixelsRenderedLast;

	// Args: Camera(double vertical_fov, double aspect_ratio, vec3 vup, 
// point3 lookfrom, point3 lookat)
	Camera c;

	uint8_t *m_renderBuffer;
};

