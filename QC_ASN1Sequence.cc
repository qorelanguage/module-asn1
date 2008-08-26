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

qore_classid_t CID_ASN1SEQUENCE;
QoreClass *QC_ASN1SEQUENCE;

static void ASN1SEQUENCE_constructor(QoreObject *self, const QoreListNode *params, ExceptionSink *xsink)
{
   SimpleRefHolder<QoreAsn1Sequence> seq(new QoreAsn1Sequence);
   if (!seq) {
      xsink->raiseException("ASN1SEQUENCE-CONSTRUCTOR-ERROR", "error creating ASN1Sequence object");
      return;
   }

   self->setPrivate(CID_ASN1SEQUENCE, seq.release());
}

static void ASN1SEQUENCE_copy(QoreObject *self, QoreObject *old, QoreAsn1Sequence *seq, ExceptionSink *xsink)
{
   xsink->raiseException("ASN1SEQUENCE-COPY-ERROR", "ASN1Sequence objects cannot be copied");
}

static AbstractQoreNode *ASN1SEQUENCE_add(QoreObject *self, QoreAsn1Sequence *seq, const QoreListNode *params, ExceptionSink *xsink)
{
   QoreObject *o = test_object_param(params, 0);
   AbstractQoreAsn1Object *obj = o ? (AbstractQoreAsn1Object *)o->getReferencedPrivateData(CID_ASN1OBJECT, xsink) : 0;
   if (!obj) {
      if (!*xsink)
	 xsink->raiseException("ASN1SEQUENCE-ADD-ERROR", "expecting an object derived from ASN1Object as the sole argument to ASN1Sequence::add()");
      return 0;
   }
   // we pass the reference on to the sequence
   seq->add(obj);
   return 0;
}

QoreClass *initASN1SequenceClass(QoreClass *parent)
{
   QC_ASN1SEQUENCE = new QoreClass("ASN1Sequence");
   CID_ASN1SEQUENCE = QC_ASN1SEQUENCE->getID();

   QC_ASN1SEQUENCE->addBuiltinVirtualBaseClass(parent);

   QC_ASN1SEQUENCE->setConstructor(ASN1SEQUENCE_constructor);
   QC_ASN1SEQUENCE->setCopy((q_copy_t)ASN1SEQUENCE_copy);

   QC_ASN1SEQUENCE->addMethod("add",    (q_method_t)ASN1SEQUENCE_add);
   
   return QC_ASN1SEQUENCE;
}
