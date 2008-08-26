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

#ifndef _QORE_ASN1_QC_ASN1SEQUENCE_H
#define _QORE_ASN1_QC_ASN1SEQUENCE_H

#include <vector>

DLLLOCAL extern qore_classid_t CID_ASN1SEQUENCE;
DLLLOCAL extern QoreClass *QC_ASN1SEQUENCE;

DLLLOCAL QoreClass *initASN1SequenceClass(QoreClass *parent);

typedef std::vector<AbstractQoreAsn1Object *> olist_t;

// we make sure there are no deadlocks and infinitely recursive trees by copying container objects
class QoreAsn1Sequence : public AbstractQoreAsn1Object
{
   protected:
      mutable QoreThreadLock m;
      olist_t olist;

      DLLLOCAL virtual ~QoreAsn1Sequence() {
	 for (olist_t::const_iterator i = olist.begin(), e = olist.end(); i != e; ++i) {
	    (*i)->deref();
	 }
      }

   public:
      DLLLOCAL QoreAsn1Sequence() {}

      DLLLOCAL AbstractQoreAsn1Object *copy() const
      {
	 QoreAsn1Sequence *rv = new QoreAsn1Sequence;
	 AutoLocker al(m);
	 for (olist_t::const_iterator i = olist.begin(), e = olist.end(); i != e; ++i)
	    rv->olist.push_back((*i)->copy());

	 return rv;
      }

      DLLLOCAL virtual bool isContainer() const { return true; }

      DLLLOCAL BinaryNode *getDerData() const {
	 BinaryNode *b = new BinaryNode();
	 {
	    AutoLocker al(m);
	    //printd(5, "Ans1Sequence::getDerData() this=%08p, childDerSize=%d\n", this, cds);

	    for (olist_t::const_iterator i = olist.begin(), e = olist.end(); i != e; ++i) {
	       SimpleRefHolder<BinaryNode> obj((*i)->getDerData());
	       //printd(5, "Ans1Sequence::getDerData() this=%08p got %08p size=%d type=%d\n", this, *obj, obj->size(), ((char *)(obj->getPtr()))[0]);
	       b->append(obj->getPtr(), obj->size());
	    }
	 }

	 SimpleRefHolder<BinaryNode> prefix(encodeDer(V_ASN1_SEQUENCE, b->size()));
	 b->prepend(prefix->getPtr(), prefix->size());
	 return b;
      }

      // referenced for addition
      DLLLOCAL void add(AbstractQoreAsn1Object *o) {
	 // copy and deref if the object is not unique and is a container
	 if (!o->is_unique() && o->isContainer()) {
	    AbstractQoreAsn1Object *obj = o->copy();
	    o->deref();
	    o = obj;
	 }
	 AutoLocker al(m);
	 olist.push_back(o);
      }
};

#endif
