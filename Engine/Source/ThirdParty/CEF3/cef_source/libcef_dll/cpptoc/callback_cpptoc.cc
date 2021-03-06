// Copyright (c) 2015 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//

#include "libcef_dll/cpptoc/callback_cpptoc.h"


// MEMBER FUNCTIONS - Body may be edited by hand.

void CEF_CALLBACK callback_cont(struct _cef_callback_t* self) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return;

  // Execute
  CefCallbackCppToC::Get(self)->Continue();
}

void CEF_CALLBACK callback_cancel(struct _cef_callback_t* self) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return;

  // Execute
  CefCallbackCppToC::Get(self)->Cancel();
}


// CONSTRUCTOR - Do not edit by hand.

CefCallbackCppToC::CefCallbackCppToC(CefCallback* cls)
    : CefCppToC<CefCallbackCppToC, CefCallback, cef_callback_t>(cls) {
  struct_.struct_.cont = callback_cont;
  struct_.struct_.cancel = callback_cancel;
}

#ifndef NDEBUG
template<> base::AtomicRefCount CefCppToC<CefCallbackCppToC, CefCallback,
    cef_callback_t>::DebugObjCt = 0;
#endif

