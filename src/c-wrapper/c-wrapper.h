/*
 * c-wrapper.h
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

#ifndef _C_WRAPPER_H_
#define _C_WRAPPER_H_

#include "linphone/api/c-types.h"

#include "internal/c-tools.h"

// =============================================================================

#define L_REGISTER_TYPE(C_TYPE) \
	extern Linphone ## C_TYPE *_linphone_ ## C_TYPE ## _init ();
#define L_INIT(C_TYPE) \
	_linphone_ ## C_TYPE ## _init ()

// =============================================================================

L_REGISTER_TYPE(Call);
L_REGISTER_TYPE(ChatMessage);
L_REGISTER_TYPE(ChatRoom);
L_REGISTER_TYPE(Participant);

#endif // ifndef _C_WRAPPER_H_