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
#include "model/metamodel/modelcoords.h"
#include "util/attributedclass.h"
#include "model/metamodel/object.h"

#include "instance.h"

namespace FIFE {

	class Elevation;
	class Selection;
	class CellGrid;
	class Object;


	/** A basic layer on a map elevation
	 *
	 * @bug These comments are very outdated!
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
	 *  "cellgrid", "shift" and "size":
	 *
	 *  The cellgrid is used to position objects on this
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
			/** Constructor
			 * Elevations are created by calling addLayer from elevation, thus
			 * this method should really be called only by elevation or test code
			 */
			Layer(const std::string& identifier, Elevation* elevation, CellGrid* grid);

			/** Destructs a Layer instance
			 */
			~Layer();

			/** Get the elevation this layer is contained in
			 */
			Elevation* getElevation() const { return m_elevation; }

			/** Get the Cellgrid as set in the constructor
			 *  @return a valid cellgrid
			 */
			CellGrid* getCellGrid() const { return m_grid; }

			/** Check existance of objects on this layer
			 *  @return True, if objects exist.
			 */
			bool hasInstances() const;

			/** Add an instance of an object at a specific position
			 */
			Instance* addInstance(Object* object, const ModelCoordinate& p);

			/** Remove an instance from the layer
			 */
			void removeInstance(Instance* object);

			/** Get the list of instances on this layer
			 */
			const std::vector<Instance*>& getInstances();

			/** Get a list of instances on this layer with a value.
			 * @param field the (string) field to search on
			 * @param value the value to be found in the field
			 */
			template<typename T>
			std::vector<Instance*> getInstances(const std::string& field, const T& value) {
				std::vector<Instance*> matches;

				std::vector<Instance*>::iterator it = m_instances.begin();
				for(; it != m_instances.end(); ++it) {
					if((*it)->get<T>(field) == value)
						matches.push_back(*it);	
				}

				return matches;
			}

			/** Set object visibility
			 */
			void setInstancesVisible(bool vis);

			/** Toggle object visibility
			 *  @see setObjectsVisible
			 */
			void toggleInstancesVisible();

			/** Check object visibility
			 *  @see setObjectsVisible
			 */
			bool areInstancesVisible() const { return m_instances_visibility; }

			/** Called periodically to update events on layer
			 */
			void update();

		protected:
			Elevation* m_elevation;

			bool m_instances_visibility;

			// all the instances on this layer
			std::vector<Instance*> m_instances;

			CellGrid* m_grid;
	};

} // FIFE

#endif