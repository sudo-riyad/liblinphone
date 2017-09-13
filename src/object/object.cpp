/*
 * object.cpp
 * Copyright (C) 2017  Belledonne Communications SARL
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "object-p.h"

#include "object.h"

// =============================================================================

using namespace std;

LINPHONE_BEGIN_NAMESPACE

Object::~Object () {
	delete mPrivate;
}

Object::Object (ObjectPrivate &p) : mPrivate(&p) {
	mPrivate->mPublic = this;
}

Variant Object::getProperty (const string &name) const {
	L_D(const Object);
	auto it = d->properties.find(name);
	return it == d->properties.cend() ? Variant() : it->second;
}

void Object::setProperty (const string &name, const Variant &value) {
	L_D(Object);
	d->properties[name] = value;
}

void Object::setProperty (const string &name, Variant &&value) {
	L_D(Object);
	d->properties[name] = move(value);
}

LINPHONE_END_NAMESPACE
