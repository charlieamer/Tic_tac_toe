//
// MutationEvent.cpp
//
// $Id: //poco/1.4/XML/src/MutationEvent.cpp#1 $
//
// Library: XML
// Package: DOM
// Module:  DOMEvents
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/DOM/MutationEvent.h"


namespace Poco {
namespace XML {


const XMLString MutationEvent::DOMSubtreeModified          = toXMLString("DOMSubtreeModified");
const XMLString MutationEvent::DOMNodeInserted             = toXMLString("DOMNodeInserted");
const XMLString MutationEvent::DOMNodeRemoved              = toXMLString("DOMNodeRemoved");
const XMLString MutationEvent::DOMNodeRemovedFromDocument  = toXMLString("DOMNodeRemovedFromDocument");
const XMLString MutationEvent::DOMNodeInsertedIntoDocument = toXMLString("DOMNodeInsertedIntoDocument");
const XMLString MutationEvent::DOMAttrModified             = toXMLString("DOMAttrModified");
const XMLString MutationEvent::DOMCharacterDataModified    = toXMLString("DOMCharacterDataModified");


MutationEvent::MutationEvent(Document* pOwnerDocument, const XMLString& type): 
	Event(pOwnerDocument, type, 0, true, false),
	_change(MODIFICATION),
	_pRelatedNode(0)
{
}


MutationEvent::MutationEvent(Document* pOwnerDocument, const XMLString& type, EventTarget* pTarget, bool canBubble, bool cancelable, Node* relatedNode):
	Event(pOwnerDocument, type, pTarget, canBubble, cancelable),
	_change(MODIFICATION),
	_pRelatedNode(relatedNode)
{
}


MutationEvent::MutationEvent(Document* pOwnerDocument, const XMLString& type, EventTarget* pTarget, bool canBubble, bool cancelable, Node* relatedNode, 
	                         const XMLString& prevValue, const XMLString& newValue, const XMLString& attrName, AttrChangeType change):
	Event(pOwnerDocument, type, pTarget, canBubble, cancelable),
	_prevValue(prevValue),
	_newValue(newValue),
	_attrName(attrName),
	_change(change),
	_pRelatedNode(relatedNode)
{
}


MutationEvent::~MutationEvent()
{
}


void MutationEvent::initMutationEvent(const XMLString& type, bool canBubble, bool cancelable, Node* relatedNode, 
	                                  const XMLString& prevValue, const XMLString& newValue, const XMLString& attrName, AttrChangeType change)
{
	initEvent(type, canBubble, cancelable);
	_pRelatedNode = relatedNode;
	_prevValue    = prevValue;
	_newValue     = newValue;
	_attrName     = attrName;
	_change       = change;
}

	 
} } // namespace Poco::XML
