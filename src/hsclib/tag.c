/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
 * Copyright (C) 2001-2003  Matthias Bethke
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
/*
 * hsclib/tag.c
 *
 * hsc-tag funcs for hsc
 */

#define NOEXTERN_HSCLIB_TAG_H

#include "hsclib/inc_base.h"

#include "hsclib/defattr.h"
#include "hsclib/tag.h"

/*
 *-------------------------------------
 * constructor/destructor
 *-------------------------------------
 */

/*
 * del_hsctag
 */
void del_hsctag(HSCTAG *tag)
{
    /* release mem */
    ufreestr(tag->name);
    ufreestr(tag->name);
    del_estr(tag->op_text);
    del_estr(tag->cl_text);
    ufreestr(tag->mbi);
    ufreestr(tag->naw);
    del_dllist(tag->attr);
    del_infilepos(tag->start_fpos);
    del_infilepos(tag->end_fpos);

    /* clear values */
    tag->option = 0;
    tag->op_text = NULL;
    tag->cl_text = NULL;
    tag->attr = NULL;
    tag->uri_size = NULL;
    tag->uri_stripext = NULL;

    ufree(tag);
}

/*
 * new_hsctag
 *
 * alloc & init a new hsctag
 */
HSCTAG *new_hsctag(STRPTR newid)
{

    HSCTAG *newtag = (HSCTAG*) umalloc(sizeof(HSCTAG));

    if (newtag)
    {
        /* init new tag item */
        newtag->name = upstr(strclone(newid));  /* set id */
        newtag->option = 0;
        newtag->o_handle = NULL;        /* no handle functions */
        newtag->c_handle = NULL;
        newtag->occured = FALSE;
        newtag->op_text = NULL;
        newtag->cl_text = NULL;
        newtag->attr = init_dllist(del_hscattr);
        newtag->mbi = NULL;
        newtag->naw = NULL;
        newtag->uri_size = NULL;
        newtag->uri_stripext = NULL;
        newtag->start_fpos = NULL;
        newtag->end_fpos = NULL;

        if (!(newtag->name && newtag->attr))
        {
            del_hsctag(newtag);
            newtag = NULL;
        }
    }

    return (newtag);
}

/*
 * cpy_hsctag
 *
 * copy an already existing hsctag
 *
 * NOTE: this is not a 100% clone:
 *  - tag-callbacks are disabled and have to be assigned again.
 *  - the occured-flag is disabled
 */
HSCTAG *cpy_hsctag(HSCTAG * oldtag)
{
    HSCTAG *newtag = new_hsctag(oldtag->name);

    if (newtag)
    {
        DLNODE *nd = NULL;

        /* init new tag item */
        newtag->option = oldtag->option;
        newtag->o_handle = NULL;        /* no handle functions */
        newtag->c_handle = NULL;
        newtag->occured = FALSE;
        newtag->op_text = NULL;
        newtag->cl_text = NULL;
        newtag->attr = init_dllist(del_hscattr);
        newtag->mbi = strclone(oldtag->mbi);
        newtag->naw = strclone(oldtag->naw);
        newtag->uri_size = NULL;
        newtag->uri_stripext = NULL;

        /* copy macro text */
        if (oldtag->op_text)
        {
            newtag->op_text = init_estr(0);
            estrcpy(newtag->op_text, oldtag->op_text);
        }
        if (oldtag->cl_text)
        {
            newtag->cl_text = init_estr(0);
            estrcpy(newtag->cl_text, oldtag->cl_text);
        }

        /* copy attribute list */
        nd = oldtag->attr->first;
        while (nd)
        {
            HSCATTR *attr = (HSCATTR *) nd->data;       /* old attribute */

            /* create copy of old attribute */
            HSCATTR *nattr = cpy_hscattr(attr);

            /* append this copy to new attr-list */
            app_dlnode(newtag->attr, nattr);

            /* check for special uri-attributes */
            if (!upstrcmp(nattr->name, oldtag->uri_size->name))
                newtag->uri_size = nattr;
            if (!upstrcmp(nattr->name, oldtag->uri_stripext->name))
                newtag->uri_size = nattr;
        }
    }

    return (newtag);
}

/*
 *---------------------------
 * find tag string
 *---------------------------
 */

/*
 * cmp_strtag
 *
 * compares a tag-string with the name
 * of a HSCTAG-entry
 */
int cmp_strtag(APTR cmpstr, APTR tagdata)
{
    STRPTR tagstr = NULL;

    if (tagdata)
        tagstr = ((HSCTAG *) tagdata)->name;

    if (tagstr) {
        if (!upstrcmp(cmpstr, tagstr))
            return -1;
        else
            return 0;
    } else return 0;
}

/*
 * find_strtag
 */
HSCTAG *find_strtag(DLLIST * taglist, STRPTR name)
{
   DLNODE *nd = find_dlnode(taglist->first, (APTR) name, cmp_strtag);
   return nd ? (HSCTAG *)nd->data : NULL;
}

/*
 * find_named_tag
 */
HSCTAG *find_named_tag(hsctree *tags, STRPTR name)
{
    ubi_trNode *node = ubi_trFind(tags, name);
    return node ? *HSCTREENODEDP(node,HSCTAG*) : NULL;
}

/*
 *-------------------------------------
 * append tag functions
 *-------------------------------------
 */

/*
 * app_tag
 *
 * create a new tag and add it to tag tree
 *
 * params: tagid..name of the new tag (eg "IMG")
 * result: ptr to the new tag or NULL if no mem
 */
HSCTAG *app_tag(hsctree *tags, STRPTR tagid)
{
   HSCTAG *newtag = new_hsctag(tagid);
   if(newtag) {
      ubi_trNode *oldnd, *node =  new_hsctreenode(newtag);
      if(node) {
         if(ubi_sptInsert(&tags->r, node, newtag->name, &oldnd)) {
            if(oldnd) {
               tags->delfunc(oldnd);
            }
            return newtag;
         }
         ufree(node);
      }
      del_hsctag(newtag);
   }
   return NULL;
}

/* free a tag linked into a tree */
void free_tag_node(ubi_btNode *node)
{
   del_hsctag(*HSCTREENODEDP(node,HSCTAG*));
   /* ufree(node); */
}

/* ubiqx-tree comparison function for finding tags given a name */
int cmp_tag_node(ubi_btItemPtr item, ubi_btNodePtr node)
{
   return upstrcmp((STRPTR)item, (*HSCTREENODEDP(node,HSCTAG*))->name);
}

/* decides if a tag is a hsc-tag */
BOOL is_hsc_tag(HSCTAG * tag)
{
    return (BOOL)((!upstrncmp(tag->name, HSC_TAGID, strlen(HSC_TAGID)))
          || (!upstrcmp(tag->name, HSC_COMMENT_STR))
          || (!upstrcmp(tag->name, HSC_VERBATIM_STR))
          || (!upstrcmp(tag->name, HSC_INSEXPR_STR)));
}

/* decides if a tag is a macro-tag */
BOOL is_macro_tag( HSCTAG *tag )
{
    return((BOOL)(((tag->option) & HT_MACRO)>0));
}

/* $Id: tag.c,v 1.8 2006/03/15 22:05:06 mb Exp mb $ */
/* vi: set ts=4: */

