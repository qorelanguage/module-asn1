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

#ifndef _QORE_ASN1_QC_ASN1BITSTRING_H
#define _QORE_ASN1_QC_ASN1BITSTRING_H

DLLLOCAL extern qore_classid_t CID_ASN1BITSTRING;
DLLLOCAL extern QoreClass *QC_ASN1BITSTRING;

DLLLOCAL QoreClass *initASN1BitStringClass(QoreClass *parent);

class QoreAsn1BitString : public QoreAsn1String
{
   protected:

   public:
      DLLLOCAL QoreAsn1BitString(const void *ptr, int len) : QoreAsn1String(V_ASN1_BIT_STRING, ptr, len)
      {
      }

      DLLLOCAL QoreAsn1BitString(const QoreListNode *l) : QoreAsn1String(V_ASN1_BIT_STRING)
      {
	 ConstListIterator li(l);
	 while (li.next()) {
	    const AbstractQoreNode *n = li.getValue();
	    ASN1_BIT_STRING_set_bit(str, li.index(), n ? (int)n->getAsBool() : 0);
	 }
      }

      // takes over ownership of n_str
      DLLLOCAL QoreAsn1BitString(ASN1_STRING *n_str) : QoreAsn1String(n_str)
      {
	 assert(ASN1_STRING_type(str) == V_ASN1_BIT_STRING);
      }

      DLLLOCAL virtual AbstractQoreNode *getQoreData() const {
	 // return a list of boolean values
	 //printd(5, "ASN1BitString::getQoreData() length=%d\n", ASN1_STRING_length(str));

	 QoreListNode *l = new QoreListNode;

	 for (int i = 0, e = ASN1_STRING_length(str) * 8; i < e; ++i) {
	    l->push(get_bool_node(ASN1_BIT_STRING_get_bit(str, i)));
	 }

	 return l;
      }

      DLLLOCAL BinaryNode *getBinary() const {
	 SimpleRefHolder<BinaryNode> b(getDerData());
	 const unsigned char *p = ((const unsigned char *)b->getPtr()) + 1;
	 decodeLen(p);

	 // skip the first byte of the data for some reason
	 int hlen = (char *)p - (char *)b->getPtr() + 1;
	 ++p;

	 // copy data to new BinaryNode
	 BinaryNode *rv = new BinaryNode();
	 //printd(5, "ASN1BitString::getBinary() new binarynode len=%d (last=0x%x)\n", b->size() - hlen, ((char *)b->getPtr())[b->size() - hlen]);
	 rv->append(p, b->size() - hlen);

	 for (int i = 0, e = b->size() - hlen; i < e; ++i) {
	    char c = ((char *)rv->getPtr())[i];
	    //printd(5, "getBinary() %d/%d: %03d (%c)\n", i, e, c, c);
	 }

	 return rv;
      }

      DLLLOCAL virtual AbstractQoreNode *getBitString() const {
	 // return a list of boolean values
	 //printd(0, "ASN1BitString::getQoreData() length=%d\n", ASN1_STRING_length(str));

	 QoreStringNode *rv = new QoreStringNode;

	 for (int i = 0, e = ASN1_STRING_length(str) * 8; i < e; ++i) {
	    rv->concat(ASN1_BIT_STRING_get_bit(str, i) ? '1' : '0');
	 }

	 return rv;
      }

      DLLLOCAL virtual const QoreClass *getQoreClass() const {
	 return QC_ASN1BITSTRING;
      }
};

#endif
