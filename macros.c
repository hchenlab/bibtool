/******************************************************************************
** $Id: macros.c,v 1.1 2007-02-07 21:27:11 gene Exp $
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
******************************************************************************/

#include <bibtool/general.h>
#include <bibtool/symbols.h>
#include <bibtool/error.h>
#include <bibtool/macros.h>
#include <bibtool/sbuffer.h>
#include "config.h"

/*****************************************************************************/
/* Internal Programs                                                         */
/*===========================================================================*/

/*****************************************************************************/
/* External Programs                                                         */
/*===========================================================================*/

/*---------------------------------------------------------------------------*/

 static Macro macros = MacroNULL;		   /*                        */

/*-----------------------------------------------------------------------------
** Function:	new_macro()
** Purpose:	Allocate a new macro structure and fill it with initial values.
**		Upon failure an error is raised and |exit()| is called.
** Arguments:
**	name	Name of the macro. This must be a symbol.
**	val	The value of the macro. This must be a symbol.
**	next	The next pointer of the |Macro| structure.
**	count	The initial reference count.
** Returns:	The new |Macro|.
**___________________________________________________			     */
Macro new_macro(name,val,next,count)		   /*                        */
  register char  *name;				   /*                        */
  register char  *val;				   /*                        */
  register int   count;				   /*                        */
  Macro          next;				   /*                        */
{ register Macro new;				   /*                        */
 						   /*                        */
  if ( (new=(Macro)malloc(sizeof(SMacro))) == MacroNULL )/*                  */
  { OUT_OF_MEMORY("Macro"); }      		   /*                        */
 						   /*                        */
  MacroName(new)  = name;			   /*                        */
  MacroValue(new) = val ;			   /*                        */
  MacroCount(new) = count;			   /*                        */
  NextMacro(new)  = next;			   /*                        */
  return(new);					   /*                        */
}						   /*------------------------*/

/*-----------------------------------------------------------------------------
** Function:	free_macro()
** Purpose:	Free a list of macros. The memory allocated for the
**		|Macro| given as argument and all struictures
**		reachable via the |NextMacro| pointer are released.
** Arguments:
**	mac	First Macro to release.
** Returns:	nothing
**___________________________________________________			     */
void free_macro(mac)				   /*                        */
  Macro mac;					   /*                        */
{ Macro next;					   /*                        */
 						   /*                        */
  while (mac)					   /*                        */
  { next = NextMacro(mac);			   /*                        */
    free(mac);					   /*                        */
    mac = next;					   /*                        */
  }						   /*                        */
}						   /*------------------------*/

/*-----------------------------------------------------------------------------
** Function:	def_macro()
** Purpose:	Define or undefine a macro.
** Arguments:
**	name	name of the macro.
**	val	NULL or the value of the new macro
**	count	initial count for the macro.
** Returns:	nothing
**___________________________________________________			     */
int def_macro(name,val,count)			   /*                        */
  char           *name;				   /*                        */
  char           *val;				   /*                        */
  int		 count;				   /*                        */
{ register Macro *mp;				   /*                        */
 						   /*                        */
  name = symbol(name);				   /*                        */
  if ( val ) val  = symbol(val);		   /*                        */
 						   /*                        */
  for ( mp   = &macros;				   /*                        */
        *mp != MacroNULL;			   /*                        */
	mp   = &NextMacro(*mp)			   /*                        */
      )						   /*                        */
  { if ( MacroName(*mp) == name )		   /*                        */
    {						   /*                        */
      if ( val )				   /*                        */
      { MacroValue(*mp) = val; }		   /*                        */
      else
      { Macro mac = *mp;
        *mp = NextMacro(*mp);
	free(mac);
      }
      return 1;					   /*                        */
    }						   /*                        */
  }						   /*                        */
 						   /*                        */
  if ( val )					   /*                        */
  { macros = new_macro(name,val,macros,count); }   /*                        */
  return 0;					   /*                        */
}						   /*------------------------*/

/*-----------------------------------------------------------------------------
** Function:	look_macro()
** Purpose:	Return the value of a macro if it is defined. This
**		value is a symbol. 
**		If the macro is undefined then |NULL| is returned.  In
**		this case the value of |add| determines whether or not
**		the macro shpould be defined. If it is less than 0
**		then no new macros is defined. Otherwise a new macro
**		is defined. The value is the empty string and the
**		initial reference count is |add|.
** Arguments:
**	name	The name of the macros to find. This needs not to be a symbol.
**	add	Initial reference count or indicator that no new macro
**		is required.
** Returns:	The value or |NULL|.
**___________________________________________________			     */
char * look_macro(name,add)			   /*                        */
  char           *name;				   /*                        */
  int		 add;				   /*                        */
{ register Macro *mp;				   /*                        */
						   /*                        */
  name = symbol(name);				   /*                        */
						   /*                        */
  for ( mp   = &macros;				   /*                        */
        *mp != MacroNULL;			   /*                        */
	mp   = &NextMacro(*mp)			   /*                        */
      )						   /*                        */
  { if ( MacroName(*mp) == name )		   /*                        */
    { if ( MacroCount(*mp) >= 0 )		   /*                        */
	MacroCount(*mp) += add;			   /*                        */
      return(MacroValue(*mp));			   /*                        */
    }						   /*                        */
  }						   /*                        */
  if ( add < 0 ) return NULL;			   /*                        */
  def_macro(name,sym_empty,add);		   /*                        */
  WARNING3("Macro '",name,"' is undefined.");	   /*                        */
  return NULL;					   /*                        */
}						   /*------------------------*/

/*-----------------------------------------------------------------------------
** Function:	foreach_macro()
** Purpose:	Apply a function to each macro in turn. The function
**		is called with the name and the value of the macro. If
**		it returns |FALSE| then the processing of further
**		macros is suppressed.
**
**		The function given as argument is called with two
**		string arguments. The first is the name of the macro
**		and the second is its value. Both are symbols and must
**		not be modified in any way.
**
**		The order of the enumeration of the macros is
**		determined by the implementation. No specific
**		assumptions should be made about this order.
** Arguments:
**	fct	Function to apply to each macro.
** Returns:	nothing
**___________________________________________________			     */
void foreach_macro(fct)				   /*                        */
  int (*fct) _ARG((char *,char *));		   /*                        */
{ Macro mac;					   /*                        */
  for ( mac=macros; 				   /*                        */
	mac != MacroNULL; 			   /*                        */
	mac = NextMacro(mac) )			   /*                        */
  {						   /*                        */
    if ( MacroValue(mac) &&			   /*                        */
	 ! (*fct)(MacroName(mac),MacroValue(mac)) )/*                        */
      return;					   /*                        */
  }						   /*                        */
}						   /*------------------------*/

/*-----------------------------------------------------------------------------
** Function:	dump_mac()
** Purpose:	Write macros to a file.
** Arguments:
**	fname	File name of the target file.
**	allp	if == 0 only the used macros are written.
** Returns:	nothing
**___________________________________________________			     */
void dump_mac(fname,allp)			   /*                        */
  char           *fname;			   /*                        */
  int            allp;				   /*                        */
{ FILE           *file;				   /*                        */
  register Macro mac;				   /*                        */
 						   /*                        */
  if ( *fname == '-' && *(fname+1) == '\0' ) 	   /*                        */
  { file = stdout; }				   /*                        */
  else if ( (file=fopen(fname,"w")) == NULL )      /*                        */
  { ERROR2("File could not be opened: ",fname);	   /*                        */
    return;					   /*                        */
  }					   	   /*                        */
 						   /*                        */
  for ( mac=macros; 				   /*                        */
	mac != MacroNULL; 			   /*                        */
	mac = NextMacro(mac) )			   /*                        */
  { if ( MacroCount(mac) > 0  ||		   /*                        */
         ( MacroCount(mac) >= 0 && allp) )	   /*                        */
    { if ( MacroName(mac) == MacroValue(mac) )	   /*                        */
      { (void)fprintf(file,			   /*                        */
		      "_string{ %s = %s } used: %d\n",/*                     */
		      MacroName(mac),		   /*                        */
		      MacroValue(mac),		   /*                        */
		      MacroCount(mac));		   /*                        */
      }						   /*                        */
      else					   /*                        */
      { (void)fprintf(file,			   /*                        */
		      "@STRING{ %s = %s } used: %d\n",/*                     */
		      MacroName(mac),		   /*                        */
		      MacroValue(mac),		   /*                        */
		      MacroCount(mac));		   /*                        */
      }						   /*                        */
    }						   /*                        */
  }						   /*                        */
 						   /*                        */
  if ( file != stdout ) (void)fclose(file);	   /*                        */
}						   /*------------------------*/

#define DeclareMacro(M) name = symbol(M); def_macro(name,name,-1)

/*-----------------------------------------------------------------------------
** Function:	init_macros()
** Purpose:	Initialize some macros from a table defined in the
**		configuration file or given as define to the C
**		compiler. This function has to be caled to initialize
**		the global macros.
**
**		Note that this function is for internal purposes
**		only. The normal user should call |init_bibtool()|
**		instead.
** Arguments:	none
** Returns:	nothing
**___________________________________________________			     */
void init_macros()				   /*                        */
{ 						   /*                        */
#ifdef INITIALIZE_MACROS	
  register char *name;				   /*                        */
  register char**wp;				   /*			     */
  static char *word_list[] =			   /*                        */
  { INITIALIZE_MACROS, NULL };			   /*                        */
 						   /*                        */
  for ( wp=word_list; *wp!=NULL; ++wp )		   /*                        */
  { DeclareMacro(*wp); }			   /*			     */
#endif
}						   /*------------------------*/


/*****************************************************************************/
/***									   ***/
/*****************************************************************************/

 static Macro items = MacroNULL;		

/*-----------------------------------------------------------------------------
** Function:	def_item()
** Purpose:	
**		
** Arguments:
**	name
**	value
** Returns:	nothing
**___________________________________________________			     */
static void def_item(name,value)		   /*                        */
  register char * name;				   /*                        */
  register char * value;			   /*                        */
{						   /*                        */
  name  = symbol(name);				   /*                        */
  value = symbol(value);			   /*                        */
  items = new_macro(name,value,items,0);	   /*                        */
}						   /*------------------------*/

/*-----------------------------------------------------------------------------
** Function:	def_field_type()
** Purpose:	This function adds a printing representation for a
**		field name to the used list. The argument is an
**		equation of the following form
**		
**			\textit{type = value}
**		
**		\textit{type} is translated to lower case and compared
**		against the internal representation. \textit{value} is
**		printed at the appropriate places instead.
** Arguments:
**	s	String containing an equation.	
** Returns:	nothing
**___________________________________________________			     */
void def_field_type(s)				   /*                        */
  char * s;					   /*                        */
{ char *name, *val, c;				   /*                        */
 						   /*                        */
  while ( *s && !is_allowed(*s) ) ++s;		   /*                        */
  if ( *s == '\0' ) return;			   /*                        */
  name = s;					   /*                        */
  while ( is_allowed(*s) ) ++s;			   /*                        */
  c = *s; *s = '\0'; val = new_string(name); *s = c;/*                       */
 						   /*                        */
  { char * cp;				   	   /*                        */
    for ( cp=val; *cp; ++cp ) *cp = ToLower(*cp);  /*                        */
  }						   /*                        */
 						   /*                        */
  name = symbol(val);				   /*                        */
  free(val);					   /*                        */
  						   /*                        */
  while ( *s && !is_allowed(*s) ) ++s;		   /*                        */
  if ( *s == '\0' ) return;			   /*                        */
  val = s;					   /*                        */
  while ( is_allowed(*s) ) ++s;			   /*                        */
  c = *s; *s = '\0'; val = symbol(val); *s = c;	   /*                        */
 						   /*                        */
  def_item(name,val);				   /*                        */
}						   /*------------------------*/


/*****************************************************************************/
/***									   ***/
/*****************************************************************************/

/*-----------------------------------------------------------------------------
** Function:	get_mapped_or_cased()
** Purpose:	
**		
**
** Arguments:
**	name
**	mac
**	type
** Returns:	
**___________________________________________________			     */
static char * get_mapped_or_cased(name,mac,type)   /*                        */
  register char * name;				   /*                        */
  int            type;				   /*                        */
  register Macro mac;				   /*                        */
{ static StringBuffer* sb = (StringBuffer*)NULL;   /*                        */
 						   /*                        */
  for ( ; mac != MacroNULL; mac=NextMacro(mac) )   /*                        */
  { if ( name == MacroName(mac) )		   /*                        */
      return MacroValue(mac);			   /*                        */
  }						   /*                        */
 						   /*                        */
  if ( sb == NULL &&				   /*                        */
       (sb=sbopen()) == NULL )			   /*                        */
  { OUT_OF_MEMORY("get_item()"); } 		   /*                        */
  sbrewind(sb);
  if ( type == SYMBOL_TYPE_LOWER ) 	   	   /*                        */
  { sbputs(name,sb);
  }
  else
  { 						   /*                        */
    						   /*                        */
    switch ( type )			   	   /*                        */
    { case SYMBOL_TYPE_CASED:			   /*                        */
	while ( *name )				   /*                        */
	{ if ( is_alpha(*name) )		   /*                        */
	  { (void)sbputc(ToUpper(*name),sb);	   /*                        */
	    for ( ++name; is_alpha(*name); ++name) /*                        */
	    { (void)sbputc(*name,sb); }		   /*                        */
	  }					   /*                        */
	  else					   /*                        */
	  { (void)sbputc(*name,sb);		   /*                        */
	    ++name;				   /*                        */
	  }					   /*                        */
	}					   /*                        */
	break;					   /*                        */
      case SYMBOL_TYPE_UPPER:			   /*                        */
	for ( ; *name; ++name )			   /*                        */
	{ (void)sbputc(ToUpper(*name),sb); }	   /*                        */
	break;					   /*                        */
    }						   /*                        */
  }						   /*                        */
  return sbflush(sb);				   /*                        */
}						   /*------------------------*/



/*****************************************************************************/
/***									   ***/
/*****************************************************************************/

/*-----------------------------------------------------------------------------
** Function:	get_item()
** Purpose:	Return the print representation of a \BibTeX{} string.
**		The appearance is determined by the |items| mapping.
**		If no approriate entry is found then |type| is used to
**		decide whether the item should be returned as
**		uppercase, lowercase or first upper only.
** Arguments:
**	name	Symbol to get the print representation for.
**	type	One of the values |SYMBOL_TYPE_UPPER|, |SYMBOL_TYPE_LOWER|, or
**		|SYMBOL_TYPE_CASED| as they are defined in |type.h|.
** Returns:	A pointer to a static string. This location  is reused
**		upon the next invocation of this function.
**___________________________________________________			     */
char * get_item(name,type)			   /*                        */
  register char * name;				   /*                        */
  int            type;				   /*                        */
{ return get_mapped_or_cased(name,items,type);	   /*                        */
}						   /*------------------------*/


/*****************************************************************************/
/***									   ***/
/*****************************************************************************/

 static Macro keys = MacroNULL;			   /*                        */

/*-----------------------------------------------------------------------------
** Function:	save_key()
** Purpose:	
**		
**
** Arguments:
**	s
**	key
** Returns:	nothing
**___________________________________________________			     */
void save_key(s,key)			   	   /*                        */
  char * s;				   	   /*                        */
  char * key;					   /*                        */
{						   /*                        */
  keys = new_macro(s,key,keys,1);		   /*                        */
}						   /*------------------------*/

/*-----------------------------------------------------------------------------
** Function:	get_key_name()
** Purpose:	
**		
**
** Arguments:
**	s
** Returns:	
**___________________________________________________			     */
char * get_key_name(s)		   	   	   /*                        */
  register char *s;			   	   /*                        */
{ return get_mapped_or_cased(s,keys,SYMBOL_TYPE_LOWER);/*                    */
}						   /*------------------------*/
