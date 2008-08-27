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

qore_classid_t CID_ASN1OCTETSTRING;
QoreClass *QC_ASN1OCTETSTRING;

static void ASN1OCTETSTRING_constructor(QoreObject *self, const QoreListNode *params, ExceptionSink *xsink)
{
   const AbstractQoreNode *p = get_param(params, 0);

   const QoreStringNode *str = p && p->getType() == NT_STRING ? reinterpret_cast<const QoreStringNode *>(p) : 0;
   const BinaryNode *b = p && p->getType() == NT_BINARY ? reinterpret_cast<const BinaryNode *>(p) : 0;

   if (!str && !b) {
      xsink->raiseException("ASN1OCTETSTRING-CONSTUCTOR-ERROR", "expecting string or binary argument to ASN1OctetString::constructor()");
      return;
   }

   SimpleRefHolder<QoreAsn1OctetString> ostr;

   if (str)
      ostr = new QoreAsn1OctetString(str->getBuffer(), str->strlen());
   else
      ostr = new QoreAsn1OctetString(b->getPtr(), b->size());

   if (!ostr) {
      xsink->raiseException("ASN1OCTETSTRING-CONSTUCTOR-ERROR", "failure creating ASN1OctetString object");
      return;
   }

   self->setPrivate(CID_ASN1OCTETSTRING, ostr.release());
}

static void ASN1OCTETSTRING_copy(QoreObject *self, QoreObject *old, QoreAsn1OctetString *str, ExceptionSink *xsink)
{
   QoreAsn1OctetString *nstr = (QoreAsn1OctetString *)str->copy();
   if (!nstr) {
      xsink->raiseException("ASN1OCTETSTRING-COPY-ERROR", "failure copying ASN1OctetString object");
      return;
   }

   self->setPrivate(CID_ASN1OCTETSTRING, nstr);   
}

QoreClass *initASN1OctetStringClass(QoreClass *parent)
{
   QC_ASN1OCTETSTRING = new QoreClass("ASN1OctetString");
   CID_ASN1OCTETSTRING = QC_ASN1OCTETSTRING->getID();

   QC_ASN1OCTETSTRING->addBuiltinVirtualBaseClass(parent);

   QC_ASN1OCTETSTRING->setConstructor(ASN1OCTETSTRING_constructor);
   QC_ASN1OCTETSTRING->setCopy((q_copy_t)ASN1OCTETSTRING_copy);

   return QC_ASN1OCTETSTRING;
}

