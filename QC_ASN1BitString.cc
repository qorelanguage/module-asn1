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
   const BinaryNode *b = test_binary_param(params, 0);
   if (!b) {
      xsink->raiseException("ASN1BITSTRING-CONSTUCTOR-ERROR", "expecting binary argument to ASN1BitString::constructor()");
      return;
   }

   QoreAsn1BitString *ostr = new QoreAsn1BitString(b->getPtr(), b->size());

   if (!ostr) {
      xsink->raiseException("ASN1BITSTRING-CONSTUCTOR-ERROR", "failure creating ASN1BitString object");
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

QoreClass *initASN1BitStringClass(QoreClass *parent)
{
   QC_ASN1BITSTRING = new QoreClass("ASN1BitString");
   CID_ASN1BITSTRING = QC_ASN1BITSTRING->getID();

   QC_ASN1BITSTRING->addBuiltinVirtualBaseClass(parent);

   QC_ASN1BITSTRING->setConstructor(ASN1BITSTRING_constructor);
   QC_ASN1BITSTRING->setCopy((q_copy_t)ASN1BITSTRING_copy);

   return QC_ASN1BITSTRING;
}

