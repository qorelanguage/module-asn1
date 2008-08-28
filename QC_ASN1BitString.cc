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

qore_classid_t CID_ASN1BITSTRING;
QoreClass *QC_ASN1BITSTRING;

static void ASN1BITSTRING_constructor(QoreObject *self, const QoreListNode *params, ExceptionSink *xsink)
{
   QoreAsn1BitString *ostr = 0;

   const AbstractQoreNode *p = get_param(params, 0);
   qore_type_t t = p ? p->getType() : NT_NOTHING;

   if (t == NT_BINARY) {
      const BinaryNode *b = reinterpret_cast<const BinaryNode *>(p);
      ostr = new QoreAsn1BitString(b->getPtr(), b->size());
   }
   else if (t == NT_LIST) {
      const QoreListNode *l = reinterpret_cast<const QoreListNode *>(p);
      ostr = new QoreAsn1BitString(l);
   }

   if (!ostr) {
      xsink->raiseException("ASN1BITSTRING-CONSTUCTOR-ERROR", "expecting binary or list argument to ASN1BitString::constructor()");
      return;
   }

   self->setPrivate(CID_ASN1BITSTRING, ostr);
}

static void ASN1BITSTRING_copy(QoreObject *self, QoreObject *old, QoreAsn1BitString *str, ExceptionSink *xsink)
{
   QoreAsn1BitString *nstr = (QoreAsn1BitString *)str->copy();
   if (!nstr) {
      xsink->raiseException("ASN1BITSTRING-COPY-ERROR", "failure copying ASN1BitString object");
      return;
   }

   self->setPrivate(CID_ASN1BITSTRING, nstr);   
}

static AbstractQoreNode *ASN1BITSTRING_getBinary(QoreObject *self, QoreAsn1BitString *str, const QoreListNode *params, ExceptionSink *xsink)
{
   return str->getBinary();
}

static AbstractQoreNode *ASN1BITSTRING_getBitString(QoreObject *self, QoreAsn1BitString *str, const QoreListNode *params, ExceptionSink *xsink)
{
   return str->getBitString();
}

QoreClass *initASN1BitStringClass(QoreClass *parent)
{
   QC_ASN1BITSTRING = new QoreClass("ASN1BitString");
   CID_ASN1BITSTRING = QC_ASN1BITSTRING->getID();

   QC_ASN1BITSTRING->addBuiltinVirtualBaseClass(parent);

   QC_ASN1BITSTRING->setConstructor(ASN1BITSTRING_constructor);
   QC_ASN1BITSTRING->setCopy((q_copy_t)ASN1BITSTRING_copy);

   QC_ASN1BITSTRING->addMethod("getBinary",    (q_method_t)ASN1BITSTRING_getBinary);
   QC_ASN1BITSTRING->addMethod("getBitString", (q_method_t)ASN1BITSTRING_getBitString);

   return QC_ASN1BITSTRING;
}

