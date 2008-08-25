/*
  asn1 Qore module

  Qore Programming Language

  Copyright (C) 2008 David Nichols, all rights reserved

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "qore-asn1.h"

qore_classid_t CID_ASN1STRING;
QoreClass *QC_ASN1STRING;

static void ASN1STRING_constructor(QoreObject *self, const QoreListNode *params, ExceptionSink *xsink)
{
   const AbstractQoreNode *p = get_param(params, 0);

   const QoreStringNode *str = p && p->getType() == NT_STRING ? reinterpret_cast<const QoreStringNode *>(p) : 0;
   const BinaryNode *b = p && p->getType() == NT_BINARY ? reinterpret_cast<const BinaryNode *>(p) : 0;

   if (!str && !b) {
      xsink->raiseException("ASN1STRING-CONSTUCTOR-ERROR", "expecting string or binary argument to ASN1String::constructor()");
      return;
   }

   p = get_param(params, 1);
   int type = p ? p->getAsInt() : V_ASN1_UTF8STRING;

   SimpleRefHolder<QoreAsn1String> ostr;

   if (str) {
      if (type == V_ASN1_UTF8STRING) {
	 TempEncodingHelper t(str, QCS_UTF8, xsink);
	 if (!t)
	    return;
	 
	 ostr = new QoreAsn1String(*t, type);
      }
      else
	 ostr = new QoreAsn1String(str, type);
   }
   else
      ostr = new QoreAsn1String(b, type);

   if (!ostr) {
      xsink->raiseException("ASN1STRING-CONSTUCTOR-ERROR", "failure creating ASN1String object");
      return;
   }

   self->setPrivate(CID_ASN1STRING, ostr.release());
}

static void ASN1STRING_copy(QoreObject *self, QoreObject *old, QoreAsn1String *b, ExceptionSink *xsink)
{
   xsink->raiseException("ASN1STRING-COPY-ERROR", "ASN1String objects cannot be copied");
}

/*
static AbstractQoreNode *ASN1STRING_value(QoreObject *self, QoreAsn1String *b, const QoreListNode *params, ExceptionSink *xsink)
{
   return get_bool_node(b->value());
}
*/

QoreClass *initASN1StringClass(QoreClass *parent)
{
   QC_ASN1STRING = new QoreClass("ASN1String");
   CID_ASN1STRING = QC_ASN1STRING->getID();

   QC_ASN1STRING->addBuiltinVirtualBaseClass(parent);

   QC_ASN1STRING->setConstructor(ASN1STRING_constructor);
   QC_ASN1STRING->setCopy((q_copy_t)ASN1STRING_copy);

   //QC_ASN1STRING->addMethod("value",      (q_method_t)ASN1STRING_value);

   return QC_ASN1STRING;
}
