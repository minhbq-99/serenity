/*
 * Copyright (c) 2021, Valtteri Koskivuori <vkoskiv@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "MagnifierWidget.h"
#include <LibGUI/Painter.h>
#include <LibGUI/Window.h>
#include <LibGUI/WindowServerConnection.h>
#include <LibGfx/AffineTransform.h>
#include <math.h>

MagnifierWidget::MagnifierWidget()
{
}

MagnifierWidget::~MagnifierWidget()
{
}

void MagnifierWidget::track_cursor_globally()
{
    VERIFY(window());
    auto window_id = window()->window_id();
    VERIFY(window_id >= 0);

    set_global_cursor_tracking(true);
    GUI::WindowServerConnection::the().set_global_cursor_tracking(window_id, true);
}

void MagnifierWidget::set_scale_factor(int scale_factor)
{
    VERIFY(scale_factor == 2 || scale_factor == 4);
    m_scale_factor = scale_factor;
    update();
}

void MagnifierWidget::timer_event(Core::TimerEvent&)
{
    m_mouse_position = GUI::WindowServerConnection::the().get_global_cursor_position();
    update();
}

void MagnifierWidget::paint_event(GUI::PaintEvent&)
{
    GUI::Painter painter(*this);

    int grab_frame_size = 200;

    grab_frame_size /= m_scale_factor;

    // Paint our screenshot
    Gfx::Rect region { m_mouse_position.x() - (grab_frame_size / 2), m_mouse_position.y() - (grab_frame_size / 2), grab_frame_size, grab_frame_size };
    auto map = GUI::WindowServerConnection::the().get_screen_bitmap(region);
    painter.draw_scaled_bitmap(rect(), *map.bitmap(), map.bitmap()->rect());
}
