/*
 * c-tools.h
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

#ifndef _C_TOOLS_H_
#define _C_TOOLS_H_

#include <list>
#include <memory>

// From coreapi.
#include "private.h"

// =============================================================================

LINPHONE_BEGIN_NAMESPACE

class Wrapper {
private:
	template<typename T>
	struct WrappedObject {
		belle_sip_object_t base;
		std::shared_ptr<T> cppPtr;
	};

	template<typename T>
	struct WrappedClonableObject {
		belle_sip_object_t base;
		T *cppPtr;
	};

public:
	template<typename T>
	static inline decltype (std::declval<T>().getPrivate()) getPrivate (T *object) {
		if (!object)
		return nullptr;
		return object->getPrivate();
	}

	// ---------------------------------------------------------------------------
	// Get c/cpp ptr helpers.
	// ---------------------------------------------------------------------------

	template<
		typename CppType,
		typename CType,
		typename std::enable_if<std::is_base_of<Object, CppType>::value>::type = 0
	>
	static inline std::shared_ptr<CppType> getCppPtrFromC (CType *object) {
		L_ASSERT(object);
		return reinterpret_cast<WrappedObject<CppType> *>(object)->cppPtr;
	}

	template<
		typename CppType,
		typename CType,
		typename std::enable_if<std::is_base_of<Object, CppType>::value, CppType>::type = 0
	>
	static inline std::shared_ptr<const CppType> getCppPtrFromC (const CType *object) {
		L_ASSERT(object);
		return reinterpret_cast<const WrappedObject<CppType> *>(object)->cppPtr;
	}

	template<
		typename CppType,
		typename CType,
		typename = typename std::enable_if<std::is_base_of<ClonableObject, CppType>::value, CppType>::type
	>
	static inline CppType *getCppPtrFromC (CType *object) {
		L_ASSERT(object);
		return reinterpret_cast<WrappedClonableObject<CppType> *>(object)->cppPtr;
	}

	template<
		typename CppType,
		typename CType,
		typename = typename std::enable_if<std::is_base_of<ClonableObject, CppType>::value, CppType>::type
	>
	static inline const CppType *getCppPtrFromC (const CType *object) {
		L_ASSERT(object);
		return reinterpret_cast<const WrappedClonableObject<CppType> *>(object)->cppPtr;
	}

	template<typename T>
	static inline void setCppPtrFromC (void *object, std::shared_ptr<T> &cppPtr) {
		L_ASSERT(object);
		static_cast<WrappedObject<T> *>(object)->cppPtr = cppPtr;
	}

	template<typename T>
	static T *getCppPtr (const std::shared_ptr<T> &cppPtr) {
		return cppPtr.get();
	}

	template<typename T>
	static T *getCppPtr (T *cppPtr) {
		return cppPtr;
	}

	// ---------------------------------------------------------------------------

	template<typename T>
	static inline bctbx_list_t * getCListFromCppList (std::list<T> cppList) {
		bctbx_list_t *result = nullptr;
		for (auto it = cppList.begin(); it != cppList.end(); it++)
			result = bctbx_list_append(result, *it);
		return result;
	}

	template<typename T>
	static inline std::list<T> getCppListFromCList (bctbx_list_t *cList) {
		std::list<T> result;
		for (auto it = cList; it; it = bctbx_list_next(it))
			result.push_back(static_cast<T>(bctbx_list_get_data(it)));
		return result;
	}

private:
	Wrapper ();

	L_DISABLE_COPY(Wrapper);
};

LINPHONE_END_NAMESPACE

// -----------------------------------------------------------------------------

#define L_DECLARE_C_STRUCT_IMPL(STRUCT, C_NAME) \
	struct _Linphone ## STRUCT { \
		belle_sip_object_t base; \
		std::shared_ptr<LINPHONE_NAMESPACE::STRUCT> cppPtr; \
	}; \
	BELLE_SIP_DECLARE_VPTR_NO_EXPORT(Linphone ## STRUCT); \
	static Linphone ## STRUCT *_linphone_ ## C_NAME ## _init() { \
		Linphone ## STRUCT * object = belle_sip_object_new(Linphone ## STRUCT); \
		new(&object->cppPtr) std::shared_ptr<LINPHONE_NAMESPACE::STRUCT>(); \
		return object; \
	} \
	static void _linphone_ ## C_NAME ## _uninit(Linphone ## STRUCT * object) { \
		object->cppPtr.~shared_ptr (); \
	} \
	static void _linphone_ ## C_NAME ## _clone(Linphone ## STRUCT * dest, const Linphone ## STRUCT * src) { \
		new(&dest->cppPtr) std::shared_ptr<LINPHONE_NAMESPACE::STRUCT>(); \
		dest->cppPtr = std::make_shared<LINPHONE_NAMESPACE::STRUCT>(*src->cppPtr.get()); \
	} \
	BELLE_SIP_DECLARE_NO_IMPLEMENTED_INTERFACES(Linphone ## STRUCT); \
	BELLE_SIP_INSTANCIATE_VPTR(Linphone ## STRUCT, belle_sip_object_t, \
	_linphone_ ## C_NAME ## _uninit, \
	_linphone_ ## C_NAME ## _clone, \
	NULL, \
	FALSE \
	);

#define L_DECLARE_C_CLONABLE_STRUCT_IMPL(STRUCT, C_NAME) \
	struct _Linphone ## STRUCT { \
		belle_sip_object_t base; \
		LINPHONE_NAMESPACE::STRUCT *cppPtr; \
	}; \
	BELLE_SIP_DECLARE_VPTR_NO_EXPORT(Linphone ## STRUCT); \
	static Linphone ## STRUCT *_linphone_ ## C_NAME ## _init() { \
		return belle_sip_object_new(Linphone ## STRUCT); \
	} \
	static void _linphone_ ## C_NAME ## _uninit(Linphone ## STRUCT * object) { \
		delete object->cppPtr; \
	} \
	static void _linphone_ ## C_NAME ## _clone(Linphone ## STRUCT * dest, const Linphone ## STRUCT * src) { \
		L_ASSERT(src->cppPtr); \
		dest->cppPtr = new LINPHONE_NAMESPACE::STRUCT(*src->cppPtr); \
	} \
	BELLE_SIP_DECLARE_NO_IMPLEMENTED_INTERFACES(Linphone ## STRUCT); \
	BELLE_SIP_INSTANCIATE_VPTR(Linphone ## STRUCT, belle_sip_object_t, \
	_linphone_ ## C_NAME ## _uninit, \
	_linphone_ ## C_NAME ## _clone, \
	NULL, \
	FALSE \
	);

#define L_DECLARE_C_STRUCT_NEW_DEFAULT(STRUCT, C_NAME) \
	Linphone ## STRUCT * linphone_ ## C_NAME ## _new() { \
		Linphone ## STRUCT * object = _linphone_ ## C_NAME ## _init(); \
		object->cppPtr = std::make_shared<LINPHONE_NAMESPACE::STRUCT>(); \
		return object; \
	}

#define L_STRING_TO_C(STR) ((STR).empty() ? NULL : (STR).c_str())
#define L_C_TO_STRING(STR) ((STR) == NULL ? std::string() : (STR))

#define L_GET_CPP_PTR_FROM_C_STRUCT(OBJECT, TYPE) \
	LINPHONE_NAMESPACE::Wrapper::getCppPtrFromC<LINPHONE_NAMESPACE::TYPE, Linphone ## TYPE>(OBJECT)

#define L_SET_CPP_PTR_FROM_C_STRUCT(OBJECT, CPP_PTR) \
	LINPHONE_NAMESPACE::Wrapper::setCppPtrFromC(OBJECT, CPP_PTR)

#define L_GET_PRIVATE(OBJECT) \
	LINPHONE_NAMESPACE::Wrapper::getPrivate(OBJECT)

#define L_GET_PRIVATE_FROM_C_STRUCT(OBJECT, TYPE) \
	L_GET_PRIVATE(LINPHONE_NAMESPACE::Wrapper::getCppPtr( \
		L_GET_CPP_PTR_FROM_C_STRUCT(OBJECT, TYPE) \
	))

#define L_GET_C_LIST_FROM_CPP_LIST(LIST, TYPE) \
	LINPHONE_NAMESPACE::Wrapper::getCListFromCppList<TYPE *>(LIST)
#define L_GET_CPP_LIST_FROM_C_LIST(LIST, TYPE) \
	LINPHONE_NAMESPACE::Wrapper::getCppListFromCList<TYPE *>(LIST)

#endif // ifndef _C_TOOLS_H_
