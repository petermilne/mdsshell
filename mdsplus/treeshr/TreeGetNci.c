#if defined(_WIN32)
#include <io.h>
#endif
#include <fcntl.h>
#include <treeshr.h>
#include "treeshrp.h"
#include <ncidef.h>
#include <mdsdescrip.h>
#include <mdsshr.h>
#include <usagedef.h>
#include <stdlib.h>
#include <string.h>
#include <mds_stdarg.h>
#ifndef O_BINARY
#define O_BINARY 0
#endif
#ifndef O_RANDOM
#define O_RANDOM 0
#endif

extern int StrFree1Dx();

static char *cvsrev = "@(#)$RCSfile: TreeGetNci.c,v $ $Revision: 1.49 $ $Date: 2002/11/05 15:34:05 $";

#ifndef HAVE_VXWORKS_H
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define read_nci \
 if (need_nci)\
 {\
    nid_to_tree_nidx(dblist, (&nid), info, node_number);\
    status = TreeCallHook(GetNci,info,nid_in);\
    if (status && !(status & 1)) break;\
    if (info->reopen) TreeCloseFiles(info);\
    status = TreeGetNciW(info, node_number, &nci);\
    need_nci = 0;\
    if (!(status & 1)) break;\
 }
#define break_on_no_node if (!node_exists) {status = TreeNNF; break; }
#define set_retlen(length) if (itm->buffer_length < (int)length) { status = TreeBUFFEROVF; break; } else retlen=length

static char *GetPath(PINO_DATABASE *dblist, NODE *node, int remove_tree_refs);
static const char *nonode = "<no-node>   ";
static int OpenNciR(TREE_INFO *info);

extern void *DBID;

char *TreeGetMinimumPath(int *def_nid_in, int nid_in) {return _TreeGetMinimumPath(DBID,def_nid_in,nid_in);}
int  TreeGetNci(int nid_in, struct nci_itm *nci_itm) {return _TreeGetNci(DBID,nid_in,nci_itm);}
char *TreeGetPath(int nid_in) {return _TreeGetPath(DBID,nid_in);}
int TreeIsOn(int nid) {return _TreeIsOn(DBID,nid);}

int _TreeGetNci(void *dbid, int nid_in, struct nci_itm *nci_itm)
{
  PINO_DATABASE *dblist = (PINO_DATABASE *)dbid;
  NID nid = *(NID *)&nid_in;
  int       status = TreeNORMAL;
  int       node_number;
  TREE_INFO *info;
  NCI_ITM  *itm;
  NCI       nci;
  NODE     *node;
  int       need_nci = 1;
  NID       out_nid;
  int       i;
  NODE     *cng_node;
  int       node_exists;
  int		depth;
  _int64    rfa_l;
  int       count = 0;
  NID      *out_nids;
  NID      *end_nids;
  NODE     *saved_node;
  if (!(IS_OPEN(dblist)))
    return TreeNOT_OPEN;
  if (dblist->remote)
	  return GetNciRemote(dbid,nid_in,nci_itm);
  nid_to_node(dblist, (&nid), saved_node);
  node_exists = saved_node && (saved_node->name[0] < 'a');
  for (itm = nci_itm; itm->code != NciEND_OF_LIST && status & 1; itm++)
  {
	  char *string = NULL;
	  int       retlen = 0;
	  node = saved_node;
	  switch (itm->code)
	  {
	  case NciDEPTH:
		  break_on_no_node;
		  set_retlen(sizeof(depth));
		  for (depth = 1; node->parent; node = parent_of(node))
			  depth++;
		  *(unsigned int *) itm->pointer = depth;
		  break;
	  case NciGET_FLAGS:
		  break_on_no_node;
		  read_nci;
		  set_retlen(sizeof(nci.flags));
		  *(unsigned int *) itm->pointer = nci.flags;
		  break;
	  case NciTIME_INSERTED:
		  break_on_no_node;
		  read_nci;
		  set_retlen(sizeof(nci.time_inserted));
                  memcpy(itm->pointer,&nci.time_inserted,sizeof(nci.time_inserted));
		  break;
	  case NciOWNER_ID:
		  break_on_no_node;
		  read_nci;
		  set_retlen(sizeof(nci.owner_identifier));
		  *(unsigned int *) itm->pointer = nci.owner_identifier;
		  break;
	  case NciCLASS:
		  break_on_no_node;
		  read_nci;
		  set_retlen(sizeof(nci.class));
		  *(unsigned char *) itm->pointer = nci.class;
		  break;
	  case NciDTYPE:
		  break_on_no_node;
		  read_nci;
		  set_retlen(sizeof(nci.dtype));
		  *(unsigned char *) itm->pointer = nci.dtype;
		  break;
	  case NciLENGTH:
		  break_on_no_node;
		  read_nci;
		  set_retlen(sizeof(nci.length));
		  *(unsigned int *) itm->pointer = nci.length;
		  break;
	  case NciRLENGTH:
		  break_on_no_node;
		  read_nci;
		  set_retlen(sizeof(nci.length));
		  *(unsigned int *) itm->pointer =
			  (nci.flags2 & NciM_DATA_IN_ATT_BLOCK) ? nci.length : 
                                 nci.DATA_INFO.DATA_LOCATION.record_length;
		  break;
	  case NciSTATUS:
		  break_on_no_node;
		  read_nci;
		  set_retlen(sizeof(nci.status));
		  *(unsigned int *) itm->pointer = nci.status;
		  break;
	  case NciDATA_IN_NCI:
		  break_on_no_node;
		  read_nci;
		  set_retlen(sizeof(char));
		  *(unsigned char *) itm->pointer = (nci.flags2 & NciM_DATA_IN_ATT_BLOCK) ? 1 : 0;
		  break;
	  case NciERROR_ON_PUT:
		  break_on_no_node;
		  read_nci;
		  set_retlen(sizeof(char));
		  *(unsigned char *) itm->pointer = (nci.flags2 & NciM_ERROR_ON_PUT) ? 1 : 0;
		  break;
	  case NciIO_STATUS:
		  break_on_no_node;
		  read_nci;
		  set_retlen(sizeof(nci.DATA_INFO.ERROR_INFO.error_status));
		  *(unsigned int *) itm->pointer = (nci.flags2 & NciM_ERROR_ON_PUT) ? 
                           nci.DATA_INFO.ERROR_INFO.error_status : 1;
		  break;
	  case NciIO_STV:
		  break_on_no_node;
		  read_nci;
		  set_retlen(sizeof(nci.DATA_INFO.ERROR_INFO.stv));
		  *(unsigned int *) itm->pointer = (nci.flags2 & NciM_ERROR_ON_PUT) ? 
                           nci.DATA_INFO.ERROR_INFO.stv : 1;
		  break;
	  case NciRFA:
		  break_on_no_node;
		  read_nci;
		  set_retlen(sizeof(rfa_l));
		  rfa_l = RfaToSeek(nci.DATA_INFO.DATA_LOCATION.rfa);
		  memcpy(itm->pointer, &rfa_l, min(sizeof(unsigned int), itm->buffer_length));
		  break;
	  case NciCONGLOMERATE_ELT:
		  break_on_no_node;
		  set_retlen(sizeof(node->conglomerate_elt));
		  if (swapshort((char *)&node->conglomerate_elt))
			  *(unsigned short *) itm->pointer = swapshort((char *)&node->conglomerate_elt);
		  else
			  *(unsigned short *) itm->pointer = 0;
		  break;
	  case NciPARENT:
		  break_on_no_node;
		  set_retlen(sizeof(NID));
		  if (node->parent)
		  {
			  node_to_nid(dblist, parent_of(node), (&out_nid))
				  *(NID *) itm->pointer = out_nid;
		  }
		  else
			  retlen = 0;
		  break;
	  case NciBROTHER:
		  break_on_no_node;
		  set_retlen(sizeof(NID));
		  if (node->brother)
		  {
			  node_to_nid(dblist, brother_of(node), (&out_nid));
			  *(NID *) itm->pointer = out_nid;
		  }
		  else
			  retlen = 0;
		  break;
	  case NciMEMBER:
		  break_on_no_node;
		  set_retlen(sizeof(NID));
		  if (node->member)
		  {
			  node_to_nid(dblist, member_of(node), (&out_nid));
			  *(NID *) itm->pointer = out_nid;
		  }
		  else
			  retlen = 0;
		  break;
	  case NciCHILD:
		  break_on_no_node;
		  set_retlen(sizeof(NID));
		  if (node->child)
		  {
			  node_to_nid(dblist, child_of(node), (&out_nid));
			  *(NID *) itm->pointer = out_nid;
		  }
		  else
			  retlen = 0;
		  break;
	  case NciPARENT_RELATIONSHIP:
		  break_on_no_node;
		  set_retlen(4);
		  *(unsigned int *) itm->pointer = (TreeIsChild(node) & 1) ? NciK_IS_CHILD : NciK_IS_MEMBER;
		  break;
	  case NciCONGLOMERATE_NIDS:
		  break_on_no_node;
		  if (swapshort((char *)&node->conglomerate_elt))
		  {
			  out_nid = nid;
			  out_nid.node -= (swapshort((char *)&node->conglomerate_elt) - 1);
			  cng_node = node - swapshort((char *)&node->conglomerate_elt) + 1;
			  for (i = 0; (i < itm->buffer_length / 4) && (swapshort((char *)&cng_node->conglomerate_elt) > i); i++)
			  {
				  set_retlen((sizeof(NID) * (i + 1)));
				  *((NID *) (itm->pointer) + i) = out_nid;
				  cng_node++;
				  out_nid.node++;
			  }
		  }
		  else
			  retlen = 0;
		  break;
	  case NciNUMBER_OF_CHILDREN:
		  break_on_no_node;
		  set_retlen(sizeof(count));
		  count = 0;
		  if (node->child)
			  for (node = child_of(node); node;
		  count++, node = node->brother ? brother_of(node) : 0);
		  *(int *) (itm->pointer) = count;
		  break;
	  case NciNUMBER_OF_MEMBERS:
		  break_on_no_node;
		  set_retlen(sizeof(count));
		  count = 0;
		  if (node->member)
			  for (node = member_of(node); node;
		  count++, node = node->brother ? brother_of(node) : 0);
		  *(int *) (itm->pointer) = count;
		  break;
	  case NciNUMBER_OF_ELTS:
		  break_on_no_node;
		  set_retlen(sizeof(count));
		  count = 0;
		  cng_node = node - swapshort((char *)&node->conglomerate_elt) + 1;
		  for (count = 0; swapshort((char *)&cng_node->conglomerate_elt) > count; count++, cng_node++);
		  *(int *) (itm->pointer) = count;
		  break;
	  case NciCHILDREN_NIDS:
		  {
			  break_on_no_node;
			  out_nids = (NID *) itm->pointer;
			  end_nids = (NID *) (((char *)itm->pointer) + itm->buffer_length);
			  if (node->child)
				  for (node = child_of(node); node && (out_nids + 1 <= end_nids);
			  node = node->brother ? brother_of(node) : 0, out_nids++)
				  node_to_nid(dblist, node, out_nids);
			  retlen = (char *) out_nids - (char *) itm->pointer;
		  }
		  break;
	  case NciMEMBER_NIDS:
		  break_on_no_node;
		  out_nids = (NID *) itm->pointer;
		  end_nids = (NID *) (((char *)itm->pointer) + itm->buffer_length);
		  if (node->member)
			  for (node = member_of(node); node && (out_nids + 1 <= end_nids);
		  node = node->brother ? brother_of(node) : 0, out_nids++)
			  node_to_nid(dblist, node, out_nids);
		  retlen = (char *) out_nids - (char *) itm->pointer;
		  break;
	  case NciUSAGE:
		  break_on_no_node;           
		  set_retlen(sizeof(node->usage));
		  *(unsigned char *) itm->pointer = node->usage;
		  break;
	  case NciNODE_NAME:
		  if (node_exists)
		  {
			  string = strncpy(malloc(sizeof(NODE_NAME)+1),node->name,sizeof(NODE_NAME));
			  string[sizeof(NODE_NAME)]='\0';
		  }
		  else
			  string = strcpy(malloc(sizeof(NODE_NAME)+1),nonode);
		  break;
	  case NciPATH:
		  if (node_exists)
			  string = GetPath(dblist, node, 0);
		  else
			  string = strcpy(malloc(sizeof(NODE_NAME)+1),nonode);
		  break;
	  case NciORIGINAL_PART_NAME:
		  break_on_no_node;
		  if (swapshort((char *)&node->conglomerate_elt))
		  {
                          struct descriptor string_d = {0, DTYPE_T, CLASS_D, 0};
			  DESCRIPTOR_NID(nid_dsc,0);
                          DESCRIPTOR(part_name,"PART_NAME");
			  nid_dsc.pointer = (char *) &nid;
			  status = _TreeDoMethod(dbid, &nid_dsc, (struct descriptor *)&part_name, &string_d MDS_END_ARG);
			  if (status == TreeNOMETHOD)
                          {
                            DESCRIPTOR(part_name,"ORIGINAL_PART_NAME");
			    status = _TreeDoMethod(dbid, &nid_dsc, (struct descriptor *)&part_name, &string_d MDS_END_ARG);
                          }
                          if (status & 1)
                          {
                            string = strncpy(malloc(string_d.length+1),string_d.pointer,string_d.length);
                            string[string_d.length] = 0;
                          }
                          StrFree1Dx(&string_d);
		  }
		  break;
	  case NciFULLPATH:
		  if (node_exists)
		  {
			  char *part = malloc(256*12);
			  char *temp;
			  string = malloc(256*12);
			  string[0] = 0;
			  part[0] = 0;
			  for (; node->parent; node = parent_of(node))
			  {
				  int i;
				  part[0] = TreeIsChild(node) ? '.' : ':';
				  for (i=0;i<sizeof(NODE_NAME) && node->name[i] != ' ';i++);
				  strncpy(&part[1],node->name,i);
				  part[i+1]='\0';
				  strcat(part,string);
				  temp=part;
				  part=string;
				  string=temp;
			  }
			  temp=part;
			  part=string;
			  string=temp;
			  string[0]='\\';
			  strcpy(&string[1],dblist->tree_info->treenam);
			  strcat(string,"::TOP");
			  strcat(string,part);
			  free(part);
		  }
		  else
			  string = strcpy(malloc(sizeof(NODE_NAME)+1),nonode);
		  break;
	  case NciMINPATH:
		  if (node_exists)
		  {
			  if (nid.tree == 0 && nid.node == 0)
			  {
				  string = malloc(7+strlen(dblist->tree_info->treenam));
				  string[0]='\\';
				  strcpy(&string[1],dblist->tree_info->treenam);
				  strcat(string,"::TOP");
			  }
			  else
			  {
				  char *part = malloc(256*12);
				  char *temp;
				  NODE     *default_node = dblist->default_node;
				  NODE     *ancestor = (NODE *) - 1;
				  char *path_string;
				  int       hyphens;
				  string = malloc(256*12);
				  string[0] = 0;
				  part[0] = 0;
				  for (hyphens = 0; default_node->parent; default_node = parent_of(default_node), hyphens++)
					  for (ancestor = parent_of(node); ancestor->parent; ancestor = parent_of(ancestor))
					  if (ancestor == default_node)
					  goto found_it;
      found_it:
				  for (ancestor = node; ancestor->parent && (default_node != ancestor); ancestor = parent_of(ancestor))
				  {
					  int i;
					  part[0] = TreeIsChild(ancestor) ? '.' : ':';
					  for (i=0;i<sizeof(NODE_NAME) && ancestor->name[i] != ' ';i++);
					  strncpy(&part[1],ancestor->name,i);
					  part[i+1]='\0';
					  strcat(part,string);
					  temp=part;
					  part=string;
					  string=temp;
				  }
				  if (hyphens)
				  {
					  temp=part;
					  part=string;
					  string=temp;
					  strcpy(string,".-");
					  for (i = 1; i < hyphens; i++)
						  strcat(string,".-");
					  strcat(string,part);
				  }
				  if (strlen(string) && string[0] == ':')
				  {
					  temp=part;
					  part=string;
					  string=temp;
					  strcpy(string,&part[1]);
				  }
				  path_string = GetPath(dblist, node, 1);
				  if (strlen(path_string) <= strlen(string))
				  {
					  temp = path_string;
					  path_string = string;
					  string=temp;
				  }
				  if (path_string) free(path_string);
				  free(part);
			  }
		  }
		  else
			  string = strcpy(malloc(sizeof(NODE_NAME)+1),nonode);
		  break;
	  case NciPARENT_TREE:
		  {
			  break_on_no_node;
			  set_retlen(sizeof(NID));
			  for (node = parent_of(node);
			  node && node->usage != TreeUSAGE_SUBTREE;
			  node = parent_of(node));
			  if (node)
			  {
				  node_to_nid(dblist, node, (&out_nid));
				  *(NID *) itm->pointer = out_nid;
			  }
			  else
				  *(int *) itm->pointer = 0;
			  break;
		  }
	  case NciDTYPE_STR:
	    {
	      char *lstr;
	      break_on_no_node;
	      read_nci;
	      lstr = MdsDtypeString(nci.dtype);
	      string = strcpy(malloc(strlen(lstr)+1),lstr);
	      break;
	    }

	  case NciCLASS_STR:
	    {
	      char *lstr;
	      break_on_no_node;
	      read_nci;
	      lstr = MdsClassString(nci.class);
	      string = strcpy(malloc(strlen(lstr)+1),lstr);
	      break;
	    }

	  case NciUSAGE_STR:
	    {
	      char *lstr;
	      break_on_no_node;
	      lstr = MdsUsageString(node->usage);
	      string = strcpy(malloc(strlen(lstr)+1),lstr);
	      break;
	    }

	  default:
		  status = TreeILLEGAL_ITEM;
    }
    if (string)
    {
		if (itm->buffer_length && itm->pointer)
		{
			retlen = min((int)strlen(string), itm->buffer_length);
			memcpy(itm->pointer,string,retlen);
			free(string);
		}
		else
		{
			retlen = (int)strlen(string);
			itm->pointer = (unsigned char *)string;
		}
    }
    if (itm->return_length_address)
		*itm->return_length_address = retlen;
  }
  return status;
}

static char *GetPath(PINO_DATABASE *dblist, NODE *node, int remove_tree_refs)
{
	char *string = malloc(256*12);
	char *part = malloc(256*12);
	char *temp;
	TREE_INFO *default_node_info = NULL;
	TREE_INFO *info = dblist->tree_info;
	int       tagged;
	string[0] = '\0';
	part[0] = '\0';
	if (remove_tree_refs)
	{
		NODE     *default_node = dblist->default_node;
		for (default_node_info = dblist->tree_info; default_node_info;
		default_node_info = default_node_info->next_info)
			if ((default_node >= default_node_info->node) &&
			(default_node <= default_node_info->node + default_node_info->header->nodes))
			break;
	}
	for (tagged = 0; node->parent && !tagged; node = parent_of(node))
	{
		char *tag;
		void *ctx = NULL;
		NID nid;
		unsigned int i;
		node_to_nid(dblist, node, (&nid));
		for (info=dblist->tree_info,i=0;info && i < nid.tree;i++,info=info->next_info);
		if ((tag = _TreeFindNodeTags((void *)dblist, *(int *)&nid, &ctx)) != NULL)
		{
			string[0] = '\\';
			string[1] = '\0';
			if (default_node_info != info)
			{
				strcat(string,info->treenam);
				strcat(string,"::");
			}
			strcat(string,tag);
			strcat(string,part);
			free(tag);
			tagged = 1;
		}
		else if (node == info->root)
		{
			string[0] = '\\';
			string[1] = '\0';
			if (default_node_info != info)
			{
				strcat(string,info->treenam);
				strcat(string,"::");
			}
			strcat(string,"TOP");
			strcat(string,part);
			tagged = 1;
		}
		else
		{
			int i;
			temp = part;
			part = string;
			string = temp;
			part[0] = TreeIsChild(node) ? '.' : ':';
			for (i=0;i<sizeof(NODE_NAME) && node->name[i] != ' ';i++);
			strncpy(&part[1],node->name,i);
			part[i+1]='\0';
			strcat(part,string);
		}
	}
	if (!strlen(string) || !tagged)
	{
		string[0] = '\\';
		string[1] = '\0';
		if (default_node_info != info)
		{
			strcat(string,info->treenam);
			strcat(string,"::");
		}
		strcat(string,"TOP");
		strcat(string,part);
	}
	free(part);
	return string;
}

int TreeIsChild(NODE *node)
{
	NODE *n = 0;
	if (node->parent)
		for(n=child_of(parent_of(node));n && n != node;n=brother_of(n));
	return n == node;
}

char *_TreeGetPath(void *dbid, int nid_in)
{
	PINO_DATABASE *dblist = (PINO_DATABASE *)dbid;
	NCI_ITM   itm_lst[] = {{0, NciPATH, 0, 0},{0, NciEND_OF_LIST, 0, 0}};
	if (!(IS_OPEN(dblist)))
		return NULL;
	_TreeGetNci(dbid, nid_in, itm_lst);
	return (char *)itm_lst[0].pointer;
}

char *_TreeGetMinimumPath(void *dbid, int *def_nid_in, int nid_in)
{
	PINO_DATABASE *dblist = (PINO_DATABASE *)dbid;
	NID *def_nid = (NID *)def_nid_in;
	NID *nid = (NID *)&nid_in;
	int status;
	NCI_ITM   itm_lst[] = {{0, NciMINPATH, 0, 0},
	{0, NciEND_OF_LIST, 0, 0}};
	if (!(IS_OPEN(dblist)))
		return NULL;
	if (def_nid)
	{
		int old_def;
		_TreeGetDefaultNid(dbid,&old_def);
		_TreeSetDefaultNid(dbid,*def_nid_in);
		status = _TreeGetNci(dbid, nid_in, itm_lst);
		_TreeSetDefaultNid(dbid,old_def);
	}
	else
		status = _TreeGetNci(dbid, nid_in, itm_lst);
	return (status & 1) ? (char *)itm_lst[0].pointer : NULL;
}

int _TreeIsOn(void *dbid, int nid)
{
	PINO_DATABASE *dblist = (PINO_DATABASE *)dbid;
	int nci_flags;
	int retlen;
	NCI_ITM   nci_list[2] = {{4, NciGET_FLAGS, 0, 0},
	{0, NciEND_OF_LIST, 0, 0}};
	int       status;
	if (!(IS_OPEN(dblist)))
		return TreeNOT_OPEN;
	nci_list[0].pointer = (unsigned char *)&nci_flags;
	nci_list[0].return_length_address = &retlen;
	status = _TreeGetNci(dbid, nid, nci_list);
	if (status & 1)
	{
		if (nci_flags & NciM_STATE)
			if (nci_flags & NciM_PARENT_STATE)
			status = TreeBOTH_OFF;
		else
			status = TreeOFF;
		else if (nci_flags & NciM_PARENT_STATE)
			status = TreePARENT_OFF;
		else
			status = TreeON;
	}
	return status;
}



int TreeGetNciW(TREE_INFO *info, int node_num, NCI *nci)
{
	int       status = TreeNORMAL;

	/******************************************
	If the tree is not open for edit then
    if the characteristics file is not open
	open the characteristics file for readonly
	access.
    if OK so far then
	fill in the rab and read the record
	******************************************/

	if ((info->edit == 0) || (node_num < info->edit->first_in_mem))
	{
		if (info->nci_file == NULL)
			status = OpenNciR(info);
		if (status & 1)
		{
		  status = TreeLockNci(info,1,node_num);
		  if (status & 1)
		  {
        char nci_bytes[42];
		    MDS_IO_LSEEK(info->nci_file->get, node_num * sizeof(nci_bytes), SEEK_SET);
		    status = MDS_IO_READ(info->nci_file->get,(void *)nci_bytes, sizeof(nci_bytes)) == sizeof(nci_bytes) ?
                      TreeSUCCESS : TreeFAILURE;
                    if (status == TreeSUCCESS)
                      TreeSerializeNciIn(nci_bytes,nci);
                    TreeUnLockNci(info,1,node_num);
		  }
		}
	}
	else
	{
	/********************************************
    Otherwise the tree is open for edit so
    the attributes are just a memory reference
    away.
		*********************************************/

		memcpy(nci,info->edit->nci +  node_num - info->edit->first_in_mem, sizeof(NCI));
	}

	return status;
}

static int OpenNciR(TREE_INFO *info)
{
	int       status;
	/****************************************************
    Allocate an nci_file structure
    (if there is any problem ...
	Free the mem allocated and return
	*****************************************************/

	if (info->nci_file == NULL) {
		info->nci_file = malloc(sizeof(NCI_FILE));
		if (info->nci_file != NULL)
			memset(info->nci_file,0,sizeof(NCI_FILE));
	}
	if (info->nci_file != NULL)
	{

		size_t len = strlen(info->filespec)-4;
		char *filename = strncpy(malloc(len+16),info->filespec,len);
		filename[len]='\0';
		strcat(filename,"characteristics");
		info->nci_file->get = MDS_IO_OPEN(filename,O_RDONLY | O_BINARY | O_RANDOM, 0);
                free(filename);
                status = (info->nci_file->get == -1) ? TreeFAILURE : TreeNORMAL;
		if (!(status & 1))
		{
			free(info->nci_file);
			info->nci_file = NULL;
		}
	}
	return status;
}

void TreeFree(void *ptr)
{
  free(ptr);
}

#ifdef HAVE_VXWORKS_H
_int64 RfaToSeek(unsigned char *rfa)
{
  _int64 ans = (((_int64)rfa[0] << 9) |
            ((_int64)rfa[1] << 17) |
            ((_int64)rfa[2] << 25) |
            ((_int64)rfa[4]) |
            (((_int64)rfa[5] & 1) << 8));
    ans =- 512;
    ans |= ((_int64)rfa[3] << 33);
  return ans;
}
#else
_int64 RfaToSeek(unsigned char *rfa)
{
  _int64 ans = (((_int64)rfa[0] << 9) |
            ((_int64)rfa[1] << 17) |
            ((_int64)rfa[2] << 25) |
            ((_int64)rfa[4]) |
            (((_int64)rfa[5] & 1) << 8)) - 512;
    ans |= ((_int64)rfa[3] << 33);
  return ans;
}
#endif
void SeekToRfa(_int64 seek, unsigned char *rfa)
{
  _int64 tmp = seek + 512;
  rfa[0] = (unsigned char)((tmp >> 9) & 0xff);
  rfa[1] = (unsigned char)((tmp >> 17) & 0xff);
  rfa[2] = (unsigned char)((tmp >> 25) & 0xff);
  rfa[3] = (unsigned char)((tmp >> 33) & 0xff);
  rfa[4] = (unsigned char)(tmp & 0xff);
  rfa[5] = (unsigned char)(tmp >> 8 & 0x1);
}
