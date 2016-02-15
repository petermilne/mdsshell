/*------------------------------------------------------------------------------

		Name:   TreeDeletePulseFile

		Type:   C function

		Author:	Thomas W. Fredian

		Date:   14-SEP-1989

		Purpose: Create pulse file from Model

------------------------------------------------------------------------------

	Call sequence:

int TreeDeletePulseFile(int shotid,int numnids, int *nids)

------------------------------------------------------------------------------
   Copyright (c) 1989
   Property of Massachusetts Institute of Technology, Cambridge MA 02139.
   This program cannot be copied or distributed in any form for non-MIT
   use without specific written approval of MIT Plasma Fusion Center
   Management.
---------------------------------------------------------------------------

	Description:


------------------------------------------------------------------------------*/
#include <STATICdef.h>
#include <stdlib.h>
#include <string.h>
#include <mdsdescrip.h>
#include <mdsshr.h>
#include <treeshr.h>
#include "treeshrp.h"
#include <ncidef.h>
#include <sys/stat.h>

extern char *TranslateLogical(char *);
extern void TranslateLogicalFree(char *);
extern char *MaskReplace();

STATIC_CONSTANT char *cvsrev = "@(#)$RCSfile: TreeDeletePulseFile.c,v $ $Revision: 1.14 $ $Date: 2004/01/05 15:53:31 $";

#define __tolower(c) (((c) >= 'A' && (c) <= 'Z') ? (c) | 0x20 : (c))

extern void *DBID;
STATIC_ROUTINE int  TreeDeleteTreeFiles(char *tree, int shot);

#if defined(_WIN32)
#include <windows.h>
#else
STATIC_ROUTINE int DeleteFile(char *src);
#endif

int       TreeDeletePulseFile(int shotid, int allfiles)
{
  return _TreeDeletePulseFile(DBID, shotid, allfiles);
}

int       _TreeDeletePulseFile(void *dbid, int shotid, int allfiles)
{
  PINO_DATABASE *dblist = (PINO_DATABASE *)dbid;
  int       status = 1;
  int       retstatus = 1;
  int       num;
  int       nids[256];
  int       j;
  int       shot;
  void      *dbid_tmp = 0;

/* Make sure tree is open */

  if ((status = _TreeIsOpen(dblist)) != TreeOPEN)
    return status;

  status = _TreeOpen(&dbid_tmp, dblist->experiment, shotid, 0);
  if (status & 1)
  {
    void       *ctx = 0;
    shot = ((PINO_DATABASE *)dbid_tmp)->shotid;
    for (num = 0; num < 256 && _TreeFindTagWild(dbid_tmp, "TOP", &nids[num], &ctx); num++)
    {
      char name[13];
      if (nids[num])
      {
        NCI_ITM itmlst[] = {{sizeof(name)-1, NciNODE_NAME, 0, 0}, {0, NciEND_OF_LIST, 0, 0}};
        itmlst[0].pointer = name;
        status = _TreeGetNci(dbid_tmp, nids[num], itmlst);
        name[12] = 0;
        for (j=11;j>0;j--) if (name[j] == 0x20) name[j] = '\0';
      }
      else
      {
        strcpy(name,dblist->experiment);
      }
      if (status & 1)
        status = TreeDeleteTreeFiles(name, shot);
      if (!(status & 1))
        retstatus = status;
    }
    _TreeClose(&dbid_tmp, dblist->experiment, shotid);
    free(dbid_tmp);
    TreeFindTagEnd(&ctx);
  }
  return retstatus;
}

STATIC_ROUTINE int  TreeDeleteTreeFiles(char *tree, int shot)
{
  int len = strlen(tree);
  char tree_lower[13];
  char pathname[32];
  char *path;
  char *pathin;
  int pathlen;
  char name[32];
  int i;
  int status = 1;
  int retstatus = 1;
  int itype;
  char *types[] = {".tree",".characteristics",".datafile"};
  for (i=0;i<len && i < 12;i++)
    tree_lower[i] = __tolower(tree[i]);
  tree_lower[i]=0;
  strcpy(pathname,tree_lower);
  strcat(pathname,TREE_PATH_SUFFIX);
#if defined(__VMS)
  pathin = strcpy(malloc(strlen(pathname)+1,pathname);
#else
  pathin = TranslateLogical(pathname);
#endif
  if (pathin)
  {
    pathlen = strlen(pathin);
    path = malloc(pathlen+1);
    for (itype=0;itype<3 && (status & 1);itype++)
    {
      struct stat stat_info;
      char *sfile = 0;
      char *dfile = 0;
      char *type = types[itype];
      char *part;
      strcpy(path,pathin);
      if (shot < 0)
        sprintf(name,"%s_model",tree_lower);
      else if (shot < 1000)
        sprintf(name,"%s_%03d",tree_lower,shot);
      else
        sprintf(name,"%s_%d",tree_lower,shot);
      for (i=0,part=path;i<pathlen+1;i++)
      {
        if (*part == ' ') 
          part++;
        else if ((path[i] == ';' || path[i] == 0) && strlen(part))
        {
	  path[i] = 0;
          sfile = strcpy(malloc(strlen(part)+strlen(name)+strlen(type)+2),part);
	  if (strcmp(sfile+strlen(sfile)-1,TREE_PATH_DELIM))
	  strcat(sfile,TREE_PATH_DELIM);
	  strcat(sfile,name);
	  strcat(sfile,type);
	  dfile = MaskReplace(sfile,tree_lower,shot);
          free(sfile);
          if (stat(dfile,&stat_info) == 0)
            break;
          else
	  {
            free(dfile);
            dfile = 0;
	    part = &path[i+1];
          }
        }
      }
      if (dfile)
      {
        retstatus = DeleteFile(dfile);
        free(dfile);
      }
    }
    free(path);
	TranslateLogicalFree(pathin);
  }
  return retstatus ? status : 0;
}

#if !defined(_WIN32)
STATIC_ROUTINE int DeleteFile(char *src)
{
  int status = remove(src);
  return status == 0;
}
#endif

