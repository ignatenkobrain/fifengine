/***************************************************************************
 *   Copyright (C) 2005-2007 by the FIFE Team                              *
 *   fife-public@lists.sourceforge.net                                     *
 *   This file is part of FIFE.                                            *
 *                                                                         *
 *   FIFE is free software; you can redistribute it and/or modify          *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA              *
 ***************************************************************************/

#ifndef FIFE_GUI_FONTS_SUBIMAGEFONT_H
#define FIFE_GUI_FONTS_SUBIMAGEFONT_H

// Standard C++ library includes
#include <string>

// Platform specific includes
#include "util/fife_stdint.h"

// 3rd party library includes

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder
#include "imagefontbase.h"

namespace FIFE {
	class ImagePool;

	/** Imagefont that is able to read glyphs from single image sheet, see e.g.
	 *  guichan imagefontcollection (google for it)
	 */
	class SubImageFont: public ImageFontBase {
		public:
			/**
			 * Constructor.
			 *
			 * @param filename the filename of the Image Font.
			 * @param glyphs the glyphs for Guichan format.
			 */
			SubImageFont(const std::string& filename, const std::string& glyphs, ImagePool& pool);
			~SubImageFont() {}
		private:
			ImagePool& m_pool;
	};

}

#endif