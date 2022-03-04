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

class Renderer
{
public:
	Renderer();

	void StartRender();
	void StartRenderThread();

	color ray_color(ray& r, hittable_list& list, int depth);

	hittable_list random_scene();

	void SetWidthAndHeight(int width, int height);

	void SetTargetBuffer(uint8_t* const buffer) { m_renderBuffer = buffer; }

	void handleRendering(int startheight, int endheight, int threadNo);

	int getNumThreads();

	std::thread RenderThread;

	
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

	// Args: Camera(double vertical_fov, double aspect_ratio, vec3 vup, 
// point3 lookfrom, point3 lookat)
	Camera c;

	uint8_t *m_renderBuffer;
};

