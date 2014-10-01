// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
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

#include "libcef_dll/cpptoc/completion_handler_cpptoc.h"


// MEMBER FUNCTIONS - Body may be edited by hand.

void CEF_CALLBACK completion_handler_on_complete(
    struct _cef_completion_handler_t* self) {
  // AUTO-GENERATED CONTENT - DELETE THIS COMMENT BEFORE MODIFYING

  DCHECK(self);
  if (!self)
    return;

  // Execute
  CefCompletionHandlerCppToC::Get(self)->OnComplete();
}


// CONSTRUCTOR - Do not edit by hand.

CefCompletionHandlerCppToC::CefCompletionHandlerCppToC(
    CefCompletionHandler* cls)
    : CefCppToC<CefCompletionHandlerCppToC, CefCompletionHandler,
        cef_completion_handler_t>(cls) {
  struct_.struct_.on_complete = completion_handler_on_complete;
}

#ifndef NDEBUG
template<> long CefCppToC<CefCompletionHandlerCppToC, CefCompletionHandler,
    cef_completion_handler_t>::DebugObjCt = 0;
#endif

