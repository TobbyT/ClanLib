/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Mark Page
*/

#include "precomp.h"
#include "path.h"

// The start of the Application
int PathApp::start(const std::vector<std::string> &args)
{
	quit = false;

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib PathApp Example");
	desc.set_size(clan::Size(1024, 680), true);
	desc.set_allow_resize(true);

	clan::Canvas canvas(desc);

	// Connect the Window close event
	clan::Slot slot_quit = canvas.get_window().sig_window_close().connect(this, &PathApp::on_window_close);

	// Connect a keyboard handler to on_key_up()
	clan::Slot slot_input_up = canvas.get_window().get_ic().get_keyboard().sig_key_up().connect(this, &PathApp::on_input_up);

	clan::Shape2D shape;

	float circle_x = 100.0f;
	float circle_y = 400.0f;
	float circle_radius = 128.0f;

	shape.add_circle(circle_x, circle_y, circle_radius, false);
	shape.add_circle(circle_x, circle_y, 64.0f, false);
	shape.add_circle(circle_x, circle_y, 32.0f, false);
	shape.add_circle(circle_x, circle_y, 16.0f, true);

	std::vector<clan::Vec2f> primitives_array_circle;
	std::vector< std::vector<clan::Vec2f> > primitives_array_outline_circle;
	shape.get_triangles(primitives_array_circle);
	shape.get_outline(primitives_array_outline_circle);

	clan::Texture2D texture(canvas, "../../../Examples/Game/DiceWar/Resources/lobby_background2.png");

	std::vector<clan::Vec2f> texture_array_circle;
	texture_array_circle.resize(primitives_array_circle.size());
	for (unsigned int cnt=0; cnt< primitives_array_circle.size(); cnt++)
	{
		clan::Vec2f pos = primitives_array_circle[cnt];
		pos.x -= circle_x;
		pos.y -= circle_y;
		pos.x /= circle_radius*2.0f;
		pos.y /= circle_radius*2.0f;
		pos.x += 0.5f;
		pos.y += 0.5f;
		texture_array_circle[cnt] = pos;
	}

	// Run until someone presses escape
	while (!quit)
	{
		// Clear the display in a dark blue nuance
		canvas.clear(clan::Colorf(0.0f,0.0f,0.2f));

		std::vector< std::vector<clan::Vec2f> >::const_iterator it;

		if (!primitives_array_circle.empty())
			canvas.fill_triangles(&(primitives_array_circle[0]), primitives_array_circle.size());

		canvas.push_translate(300, 0);
		for (it = primitives_array_outline_circle.begin(); it != primitives_array_outline_circle.end(); ++it)
		{
			canvas.draw_line_strip(&((*it)[0]), it->size());
		}
		canvas.pop_modelview();

		canvas.push_translate(600, 0);
		if (!primitives_array_circle.empty())
			canvas.fill_triangles(&primitives_array_circle[0], &texture_array_circle[0], primitives_array_circle.size(), texture);
		canvas.pop_modelview();


		canvas.flip(1);

		// This call processes user input and other events
		clan::KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void PathApp::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void PathApp::on_window_close()
{
	quit = true;
}


