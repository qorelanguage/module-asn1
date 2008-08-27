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

qore_classid_t CID_ASN1UTF8STRING;
QoreClass *QC_ASN1UTF8STRING;

static void ASN1UTF8STRING_constructor(QoreObject *self, const QoreListNode *params, ExceptionSink *xsink)
{
   const QoreStringNode *str = test_string_param(params, 0);
   if (!str) {
      xsink->raiseException("ASN1UTF8STRING-CONSTUCTOR-ERROR", "expecting string argument to ASN1UTF8String::constructor()");
      return;
   }

   TempEncodingHelper t(str, QCS_UTF8, xsink);
   if (!t)
      return;

   QoreAsn1UTF8String *ostr = new QoreAsn1UTF8String(*t);

   if (!ostr) {
      xsink->raiseException("ASN1UTF8STRING-CONSTUCTOR-ERROR", "failure creating ASN1UTF8String object");
      return;
   }

   self->setPrivate(CID_ASN1UTF8STRING, ostr);
}

static void ASN1UTF8STRING_copy(QoreObject *self, QoreObject *old, QoreAsn1UTF8String *str, ExceptionSink *xsink)
{
   QoreAsn1UTF8String *nstr = (QoreAsn1UTF8String *)str->copy();
   if (!nstr) {
      xsink->raiseException("ASN1UTF8STRING-COPY-ERROR", "failure copying ASN1UTF8String object");
      return;
   }

   self->setPrivate(CID_ASN1UTF8STRING, nstr);
}

QoreClass *initASN1UTF8StringClass(QoreClass *parent)
{
   QC_ASN1UTF8STRING = new QoreClass("ASN1UTF8String");
   CID_ASN1UTF8STRING = QC_ASN1UTF8STRING->getID();

   QC_ASN1UTF8STRING->addBuiltinVirtualBaseClass(parent);

   QC_ASN1UTF8STRING->setConstructor(ASN1UTF8STRING_constructor);
   QC_ASN1UTF8STRING->setCopy((q_copy_t)ASN1UTF8STRING_copy);

   return QC_ASN1UTF8STRING;
}
