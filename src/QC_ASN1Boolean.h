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

#ifndef _QORE_ASN1_QC_ASN1BOOLEAN_H
#define _QORE_ASN1_QC_ASN1BOOLEAN_H

DLLLOCAL extern qore_classid_t CID_ASN1BOOLEAN;
DLLLOCAL extern QoreClass *QC_ASN1BOOLEAN;

DLLLOCAL QoreClass *initASN1BooleanClass(QoreClass *parent);

class QoreAsn1Boolean : public AbstractQoreAsn1Object
{
   protected:
      ASN1_BOOLEAN b;

   public:
      DLLLOCAL QoreAsn1Boolean(bool v) : b(v ? -1 : 0) {}

      DLLLOCAL virtual AbstractQoreAsn1Object *copy() const {
	 return new QoreAsn1Boolean(b);
      }
      
      DLLLOCAL BinaryNode *getDerData() const
      {
	 int size = i2d_ASN1_BOOLEAN(b, 0);
	 
	 unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char) * size);
	 unsigned char *tmp = data;
	 i2d_ASN1_BOOLEAN(b, &tmp);
	 
	 return new BinaryNode(data, size);
      }

      DLLLOCAL bool value() const { return (bool)b; }

      DLLLOCAL virtual AbstractQoreNode *getQoreData() const {
	 return get_bool_node(b);
      }

      DLLLOCAL virtual const QoreClass *getQoreClass() const {
	 return QC_ASN1BOOLEAN;
      }
};

#endif
