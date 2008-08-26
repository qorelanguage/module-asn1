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

#ifndef _QORE_ASN1_QC_ASN1OBJECTIDENTIFIER_H
#define _QORE_ASN1_QC_ASN1OBJECTIDENTIFIER_H

DLLLOCAL extern qore_classid_t CID_ASN1OBJECTIDENTIFIER;
DLLLOCAL extern QoreClass *QC_ASN1OBJECTIDENTIFIER;

DLLLOCAL QoreClass *initASN1ObjectIdentifierClass(QoreClass *parent);

#define MAX_OBJ_STR 180

class QoreAsn1ObjectIdentifier : public AbstractQoreAsn1Object
{
   protected:
      ASN1_OBJECT *obj;

      DLLLOCAL virtual ~QoreAsn1ObjectIdentifier() {
	 if (obj)
	    ASN1_OBJECT_free(obj);
      }

   public:
      DLLLOCAL QoreAsn1ObjectIdentifier(int n) {
	 obj = OBJ_nid2obj(n);
      }

      DLLLOCAL QoreAsn1ObjectIdentifier(const char *str, int n) {
	 obj = OBJ_txt2obj(str, n);
      }

      // takes over ownership of n_obj
      DLLLOCAL QoreAsn1ObjectIdentifier(ASN1_OBJECT *n_obj) : obj(n_obj) {}

      DLLLOCAL operator bool() const
      {
	 return (bool)obj;
      }

      DLLLOCAL virtual int getDerSize() const
      {
	 return i2d_ASN1_OBJECT(obj, 0);
      }

      DLLLOCAL BinaryNode *getDerData() const
      {
	 int size = getDerSize();
	 
	 unsigned char *data = (unsigned char *)malloc(sizeof(unsigned char) * size);
	 unsigned char *tmp = data;
	 i2d_ASN1_OBJECT(obj, &tmp);
	
	 //printd(5, "QoreAsn1ObjectIdentifier::getDerData() returning type %d length %d\n", data[0], data[1]);
 
	 return new BinaryNode(data, size);
      }

      DLLLOCAL QoreStringNode *getName() const
      {
	 char *buf = (char *)malloc(sizeof(char) * MAX_OBJ_STR);
	 OBJ_obj2txt(buf, MAX_OBJ_STR, obj, 0);
	 
	 return new QoreStringNode(buf, strlen(buf), MAX_OBJ_STR, QCS_DEFAULT);
      }

      DLLLOCAL QoreStringNode *getNumericalForm() const
      {
	 char *buf = (char *)malloc(sizeof(char) * MAX_OBJ_STR);
	 OBJ_obj2txt(buf, MAX_OBJ_STR, obj, 1);
	 
	 return new QoreStringNode(buf, strlen(buf), MAX_OBJ_STR, QCS_DEFAULT);
      }
};

#endif
