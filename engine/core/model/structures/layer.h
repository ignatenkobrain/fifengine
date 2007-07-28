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

#ifndef FIFE_LAYER_H
#define FIFE_LAYER_H

// Standard C++ library includes
#include <algorithm>
#include <string>
#include <vector>

// 3rd party library includes

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder
#include "util/point.h"
#include "util/attributedclass.h"

namespace FIFE { namespace model {

	class Elevation;
	class Object;

	class Selection;

	class GeometryType;
	class Geometry;

	/** A basic layer on a map elevation
	 *
	 *  This class represents a layer on the Map.
	 *  This can be for example a Tile layer 
	 *  as the roofs and floors of a fo2 map
	 *  but can also just contain "objects"
	 *  on Layer coords.
	 * 
	 *  The tiles are *not* allways created only on
	 *  a first "setTileGID".
 	 *  
	 *  The most important features of this class are
	 *  "geometry", "shift" and "size":
	 *
	 *  The geometry is used to position objects on this
	 *  Layer and the Tiles too.
	 *
	 *  The shift is added to all screen coords and
	 *  will create the illusion of a height-difference :-)
	 *
	 *  The size simply is the maximum allowd size in Layer
	 *  coords this Layer covers.
	 *  
	 *  @bug The parameter code is untested, be warned.
	 *  @bug setTileGID and setParam behave differently on invalid positions.
	 *
	 *  Attributes: 
	 *  
	 *  Future:
	 *  	Connections between Layers to walk through (Elevators...)
	 *	Grouping of Layers (These Layers are roofs ... etc)
	 */
	class Layer : public AttributedClass {
		public:

			/** Constructs a Layer instance
			 */
			Layer(const Point& size, GeometryType* geometry);

			/** Destructs a Layer instance
			 */
			~Layer();

			const std::string& getName();
			void setName(const std::string&);

			/** Get the elevation this layer is contained in
			 */
			Elevation* getElevation();

			/** Get the Geometry as set in the constructor
			 *  @return a valid Geometry
			 */
			Geometry* getGeometry();

			/** Get the size as set in the constructor
			 *  @return the size in Layer coords
			 */
			const Point& getSize() const;

			/** Set the screen coord shift
			 *  @param shift The new shift in screen coords
			 */
			void setShift(const Point& shift);

			/** Get the screen coord shift
			 *  @return The shift in screen coords
			 */
			const Point& getShift() const;

			/** Check existance of objects on this layer
			 *  @return True, if objects exist.
			 */
			bool hasObjects() const;

			/** Add an object at a specific position
			 *  Does the equivalent to @c addObject( object, object->getPosition() )
			 */
			void addObject(Object* object);

			/** Remove an object from the layer
			 */
			void removeObject(Object* object);

			/** Return a constant reference to the objects on this layer.
			 */
			const std::vector<Object*>& getAllObjects() const;

			/** Apply a visitor to each object on this layer
			 */
			template<typename T>
			void forEachObject(T visitor) {
				std::for_each(m_objects.begin(),m_objects.end(),visitor);
			}

			/** Set object visibility
			 */
			void setObjectsVisible(bool vis);

			/** Toggle object visibility
			 *  @see setObjectsVisible
			 */
			void toggleObjectsVisible();

			/** Check object visibility
			 *  @see setObjectsVisible
			 */
			bool areObjectsVisible() const;

			/** Check whether the grid lines shall be drawn
			 *  Grid lines are drawn by setting an overlay
			 *  image, which is drawn over each tiles position
			 */
			bool isGridOverlayEnabled() const;
			void setGridOverlayEnabled(bool e);
			void toggleGridOverlayEnabled();

			const Selection& getSelection();

			/** Check whether a position is on the Layer
			 * @param positon A Layer position to check
			 * @return True, only if the position is on the Layer
			 */ 
			bool isValidPosition(const Point& pos) const;
 
			/** Check whether a position is on the Layer
			 * @see isValidPosition
			 */
			bool isValidPosition(int32_t x,int32_t y) const;

		protected:

			std::string m_name;

			Elevation* m_elevation;

			Point m_size;

			bool m_objects_visibility;
			bool m_grid_overlay;

			// all the objects on this layer
			std::vector<Object*> m_objects;

			Point m_shift;
			Geometry* m_geometry;

			mutable Selection* m_selection;

			friend class Elevation;
	};

	inline
	bool Layer::isValidPosition(const Point& p) const {
		return p.x >= 0 && p.x < m_size.x && p.y >= 0 && p.y < m_size.y;
	}

	inline
	bool Layer::isValidPosition(int32_t x, int32_t y) const {
		return x >= 0 && x < m_size.x && y >= 0 && y < m_size.y;
	}

	inline
	const std::string& Layer::getName() {
		return m_name;
	}

	inline
	void Layer::setName(const std::string& name) {
		m_name = name;
	}

} } // FIFE::model

#endif
