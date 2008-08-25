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

class AbstractQoreAsn1Object : public AbstractPrivateData
{
   public:
      DLLLOCAL virtual ~AbstractQoreAsn1Object() {}

      DLLLOCAL virtual int getDerSize() const = 0;
      DLLLOCAL virtual BinaryNode *getDerData() const = 0;

      static int getLen(int size)
      {
	 if (size <= 127)
	    return 1;

	 if (size <= 255)
	    return 2;

	 if (size <= 65535)
	    return 3;

	 if (size <= 16777215)
	    return 4;

	 return 5;
      }

      static BinaryNode *encodeDer(char type, qore_size_t size, const void *data = 0)
      {
	 int lsize = getLen(size);

	 int total_size = size + lsize + 1;
	 unsigned char *buf = (unsigned char *)malloc(sizeof(unsigned char) * total_size);
	 buf[0] = type;

	 // encode length
	 if (size <= 127) {
	    buf[1] = size;
	 }
	 else if (size <= 255) {
	    buf[1] = 128 + 1;
	    buf[2] = size;
	 }
	 else if (size <= 65535) {
	    buf[1] = 128 + 2;
	    buf[2] = (size >> 8);
	    buf[3] = size & 255;
	 }
	 else if (size <= 16777215) {
	    buf[1] = 128 + 3;
	    buf[2] = (size >> 16);
	    buf[3] = (size >> 8) & 255;
	    buf[4] = size & 255;
	 }
	 else {
	    buf[1] = 128 + 4;
	    buf[2] = (size >> 24);
	    buf[3] = (size >> 16) & 255;
	    buf[4] = (size >> 8) & 255;
	    buf[5] = size & 255;
	 }

	 if (data)
	    memcpy(buf + lsize + 1, data, size);

	 //printd(5, "encodeDer(%d, %d, %08p) returning total length=%d lsize=%d (bin object size=%d)\n", type, size, data, total_size, lsize, data ? total_size : lsize + 1);
	 return new BinaryNode(buf, data ? total_size : lsize + 1);
      }
};

typedef std::vector<AbstractQoreAsn1Object *> olist_t;

class QoreAsn1Sequence : public AbstractQoreAsn1Object
{
   protected:
      olist_t olist;

      DLLLOCAL virtual ~QoreAsn1Sequence()
      {
	 for (olist_t::iterator i = olist.begin(), e = olist.end(); i != e; ++i) {
	    (*i)->deref();
	 }
      }

      DLLLOCAL virtual int getChildDerSize() const
      {
	 int size = 0;
	 
	 for (olist_t::const_iterator i = olist.begin(), e = olist.end(); i != e; ++i) {
	    size += (*i)->getDerSize();
	 }

	 return size;
      }

   public:
      DLLLOCAL QoreAsn1Sequence() {}

      DLLLOCAL virtual int getDerSize() const
      {
	 int size = getChildDerSize();
	 return 1 + getLen(size) + size;
      }

      DLLLOCAL BinaryNode *getDerData() const
      {
	 int cds = getChildDerSize();
	 //printd(5, "Ans1Sequence::getDerData() this=%08p, childDerSize=%d\n", this, cds);
	 BinaryNode *b = encodeDer(V_ASN1_SEQUENCE, cds);

	 for (olist_t::const_iterator i = olist.begin(), e = olist.end(); i != e; ++i) {
	    SimpleRefHolder<BinaryNode> obj((*i)->getDerData());
	    //printd(5, "Ans1Sequence::getDerData() this=%08p got %08p size=%d type=%d\n", this, *obj, obj->size(), ((char *)(obj->getPtr()))[0]);
	    b->append(obj->getPtr(), obj->size());
	 }

	 return b;
      }

      // it is assumed that the object is already referenced for the assignment
      DLLLOCAL void add(AbstractQoreAsn1Object *o)
      {
	 olist.push_back(o);
      }
};

#endif
