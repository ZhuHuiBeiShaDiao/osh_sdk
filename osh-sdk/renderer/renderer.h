/* This file is part of oshgui_sdk by alpine971, licensed under the MIT license:
*
* MIT License
*
* Copyright (c) namazso 2018
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#pragma once

namespace renderer {
	enum : uint32_t {
		FONT_VERDANA_7PX = 0,
		FONT_MAX
	};

	enum : uint32_t {
		NO_FLAGS = 0,
		CENTERED_X = 1,
		CENTERED_Y = 2,
		OUTLINED = 4,
		DROPSHADOW = 8
	};

	class c_renderer {
	private:
		std::unique_ptr< OSHGui::Drawing::Direct3D9Renderer > m_renderer;
		ulong_t m_old_color_write_enable;
		std::vector< OSHGui::Drawing::FontPtr > m_fonts;

	public:
		OSHGui::Application *m_instance;
		OSHGui::Drawing::GeometryBufferPtr m_geometry;
		OSHGui::Drawing::RenderTargetPtr m_render_target;

		c_renderer();
		void init( IDirect3DDevice9 *device );

		void start_drawing( IDirect3DDevice9 *device );
		void end_drawing( IDirect3DDevice9 *device ) const;

		// drawing.
		void rect( const OSHGui::Drawing::Color &color, int x, int y, int width, int height ) const;
		void ansi_text(const OSHGui::Drawing::FontPtr & font, const OSHGui::Drawing::Color & color, const OSHGui::Drawing::Color & shadow_color, float x, float y, int flags, const std::string str, ...) const;
		OSHGui::Drawing::FontPtr get_font(int index);
		OSHGui::Application * get_instance() const;
		OSHGui::Drawing::Renderer & get_renderer() const;
	};
}
