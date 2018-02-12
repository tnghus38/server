#pragma once

namespace RenderObject
{
	enum RenderQueue
	{
		Background = 1000,
		Geometry = 2000,
		AlphaTest = 2450,
		Transparent = 3000,
		Overlay = 4000,
	};
}