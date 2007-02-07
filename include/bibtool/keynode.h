/******************************************************************************
** $Id: keynode.h,v 1.1 2007-02-07 21:31:59 gene Exp $
**=============================================================================
** 
** This file is part of BibTool.
** It is distributed under the GNU General Public License.
** See the file COPYING for details.
** 
** (c) 1996-1997 Gerd Neugebauer
** 
** Net: gerd@informatik.uni-koblenz.de
** 
**-----------------------------------------------------------------------------
** Description:
**	This header file provides the datatype of a keynode.  This is
**	an internal structure which is used to built parse trees from
**	format specifications. Usually this is done in |key.c| and
**	should not be visible outside.
**
******************************************************************************/

/*-----------------------------------------------------------------------------
** Typedef:	KeyNode
** Purpose:	
**		
**		
**___________________________________________________			     */
 typedef struct kEYnODE				   /*                        */
 { short int	  kn_type;
   short int      kn_pre;
   short int      kn_post;
   char           *kn_string;
   char           *kn_from;
   char		  *kn_to;
   struct kEYnODE *kn_next;
   struct kEYnODE *kn_then;
   struct kEYnODE *kn_else;
 } *KeyNode, SKeyNode;				   /*                        */

#define NodeType(X)   ((X)->kn_type)
#define NodePre(X)    ((X)->kn_pre)
#define NodePost(X)   ((X)->kn_post)
#define NodeSymbol(X) ((X)->kn_string)
#define NodeNext(X)   ((X)->kn_next)
#define NodeThen(X)   ((X)->kn_then)
#define NodeElse(X)   ((X)->kn_else)
#ifdef REGEX
#define NodeFrom(X)   ((X)->kn_from)
#define NodeTo(X)     ((X)->kn_to)
#endif

#define NodeCountMask 0x100
#define NodePlusMask  0x200
#define NodeMinusMask 0x400
#define NodeSTRING    0x800
#define NodeTEST      0x801
#define NodeTESTneg   0x802
#define NodeOR        0x803
#define NodeSPECIAL   0x804
