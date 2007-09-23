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

// Standard C++ library includes

// 3rd party library includes

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder
#include "util/attributedclass.h"

namespace FIFE {

	AttributedClass::AttributedClass(const std::string& identifier, const std::string& className) 
		: m_id(identifier),
		m_className(className) { 
	}

	AttributedClass::AttributedClass(const AttributedClass& ac)
		: m_className(ac.m_className) {
		updateAttributes(&ac,true);
	}

	AttributedClass::~AttributedClass() {

	};

	const std::string& AttributedClass::Id() const {
		return m_id;
	}

	void AttributedClass::updateAttributes(const AttributedClass* attrObject, bool override) {
		if( attrObject == 0 )
			return;
		std::map<std::string,value_type>::const_iterator i(attrObject->m_fields.begin());
		std::map<std::string,value_type>::const_iterator end(attrObject->m_fields.end());
		for(; i != end; ++i) {
			if( !override && m_fields.find(i->first) != m_fields.end() )
				continue;
			m_fields[i->first] = i->second;
		}
	}
}; //FIFE
