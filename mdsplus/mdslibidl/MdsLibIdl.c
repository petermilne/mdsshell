#include <mdsdescrip.h>
#include <treeshr.h>
#include <mdsshr.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <mds_stdarg.h>
#include <libroutines.h>

extern int TdiExecute();
extern int TdiData();
extern int TdiCvt();
extern int TdiCompile();
extern int TdiDebug();

static char *cvsrev = "@(#)$RCSfile: MdsLibIdl.c,v $ $Revision: 1.29 $ $Date: 2004/05/21 15:42:35 $";

#ifdef _WINDOWS
#define BlockSig(a)
#define UnBlockSig(a)
#else
static int BlockSig(int sig_number)
{
  sigset_t newsigset;
  sigemptyset(&newsigset);
  sigaddset(&newsigset,sig_number);
  return sigprocmask(SIG_BLOCK, &newsigset, NULL);
}

static int UnBlockSig(int sig_number)
{
  sigset_t newsigset;
  sigemptyset(&newsigset);
  sigaddset(&newsigset,sig_number);
  return sigprocmask(SIG_UNBLOCK, &newsigset, NULL);
}

#endif

#define min(a,b) (((a) < (b)) ? (a) : (b))

int IdlMdsClose(int argc, void **argv)
{
  int status;
  BlockSig(SIGALRM);
  if (argc > 1)
    status = TreeClose((char *)argv[0],((char *)argv[1] - (char *)0));
  else
  {
    status = TreeClose(0,0);
    while(TreeClose(0,0)&1);
  }
  UnBlockSig(SIGALRM);
  return status;
}

int IdlMdsOpen(int argc, void **argv)
{
  int status;
  BlockSig(SIGALRM);
  status = TreeOpen((char *)argv[0],((char *)argv[1] - (char *)0),0);
  UnBlockSig(SIGALRM);
  return status;
}

int IdlMdsSetDefault(int argc, void **argv)
{
  int status;
  int nid;
  BlockSig(SIGALRM);
  status = TreeSetDefault((char *)argv[0],&nid);
  UnBlockSig(SIGALRM);
  return status;
}

static EMPTYXD(mdsValueAnswer);
static array_coeff arrayArgs[16];
static struct descriptor scalarArgs[16];

typedef struct {
  unsigned short slen;   /* Length of string */
  short stype;  /* type of string:  (0) static, (!0)   dynamic */
  char *s;      /*  Addr of string, invalid if slen == 0.  */
} IDL_STRING;

typedef struct {
  unsigned int slen;   /* Length of string */
  short stype;  /* type of string:  (0) static, (!0)   dynamic */
  char *s;      /*  Addr of string, invalid if slen == 0.  */
} IDL_STRING_L;

static int ShortStrings(char *b)
{
#ifdef WORDS_BIGENDIAN
  int t1_idx=5;
  int t2_idx=4;
#else
  int t1_idx=4;
  int t2_idx=5;
#endif
  return ((b[t1_idx] != 0) && (b[t1_idx] != 1)) || (b[t2_idx] != 0);
}

static void *MakeDescr(int idx, int *argsize, void *bytes)
{
  if (argsize[0] == 0)
  {
    scalarArgs[idx].class = CLASS_S;
    scalarArgs[idx].pointer = (char *)bytes;
    switch (argsize[1])
    {
    case 1: scalarArgs[idx].length = 1; scalarArgs[idx].dtype = DTYPE_BU; break;
    case 2: scalarArgs[idx].length = 2; scalarArgs[idx].dtype = DTYPE_W; break;
    case 3: scalarArgs[idx].length = 4; scalarArgs[idx].dtype = DTYPE_L; break;
    case 4: scalarArgs[idx].length = 4; scalarArgs[idx].dtype = DTYPE_FS; break;
    case 5: scalarArgs[idx].length = 8; scalarArgs[idx].dtype = DTYPE_FT; break;
    case 6: scalarArgs[idx].length = 8; scalarArgs[idx].dtype = DTYPE_FSC; break;
    case 7: scalarArgs[idx].length = ShortStrings(bytes) ? 
                         ((IDL_STRING *)bytes)->slen : ((IDL_STRING_L *)bytes)->slen;
            scalarArgs[idx].dtype = DTYPE_T; 
            scalarArgs[idx].pointer = ShortStrings(bytes) ? 
                         ((IDL_STRING *)bytes)->s : ((IDL_STRING_L *)bytes)->s; break;
    case 9: scalarArgs[idx].length = 16; scalarArgs[idx].dtype = DTYPE_FTC; break;
    case 12: scalarArgs[idx].length = 2; scalarArgs[idx].dtype = DTYPE_WU; break;
    case 13: scalarArgs[idx].length = 4; scalarArgs[idx].dtype = DTYPE_LU; break;
    case 14: scalarArgs[idx].length = 8; scalarArgs[idx].dtype = DTYPE_Q; break;
    case 15: scalarArgs[idx].length = 8; scalarArgs[idx].dtype = DTYPE_QU; break;
    default: return 0;
    }
    return (void *)&scalarArgs[idx];
  }
  else
  {
    int i;
    arrayArgs[idx].class = CLASS_A;
    arrayArgs[idx].pointer = arrayArgs[idx].a0 = (char *)bytes;
    arrayArgs[idx].scale = 0;
    arrayArgs[idx].digits = 0;
    arrayArgs[idx].dimct = argsize[0];
    arrayArgs[idx].aflags.coeff = 1;
    for (i=0 ; i < argsize[0]; i++)
      arrayArgs[idx].m[i]=argsize[i+1];
    switch (argsize[argsize[0]+1])
    {
    case 1: arrayArgs[idx].length = 1; arrayArgs[idx].dtype = DTYPE_BU;  arrayArgs[idx].arsize = argsize[argsize[0]+2];     break;
    case 2: arrayArgs[idx].length = 2; arrayArgs[idx].dtype = DTYPE_W;   arrayArgs[idx].arsize = argsize[argsize[0]+2] * 2; break;
    case 3: arrayArgs[idx].length = 4; arrayArgs[idx].dtype = DTYPE_L;   arrayArgs[idx].arsize = argsize[argsize[0]+2] * 4; break;
    case 4: arrayArgs[idx].length = 4; arrayArgs[idx].dtype = DTYPE_FS;  arrayArgs[idx].arsize = argsize[argsize[0]+2] * 4; break;
    case 5: arrayArgs[idx].length = 8; arrayArgs[idx].dtype = DTYPE_FT;  arrayArgs[idx].arsize = argsize[argsize[0]+2] * 8; break;
    case 6: arrayArgs[idx].length = 8; arrayArgs[idx].dtype = DTYPE_FSC; arrayArgs[idx].arsize = argsize[argsize[0]+2] * 8; break;
    case 7: 
      {
        if (ShortStrings(bytes))
	{
  	IDL_STRING *str;
        int num = 1;
        unsigned short maxlen;
	arrayArgs[idx].dtype = DTYPE_T;
        for (i=0;i<arrayArgs[idx].dimct;i++)
          num = num * arrayArgs[idx].m[i];
        for (i=0,str = (IDL_STRING *)bytes,maxlen=0;i<num;i++,str++)
          if (str->slen > maxlen) maxlen = str->slen;
        arrayArgs[idx].length = maxlen;
        arrayArgs[idx].arsize = maxlen * num;
        if (arrayArgs[idx].arsize > 0)
	{
          char *ptr;
          char *blanks;
          blanks = malloc(maxlen + 1);
          ptr = arrayArgs[idx].pointer = arrayArgs[idx].a0 = malloc(arrayArgs[idx].arsize + 1);
          memset(blanks,32,maxlen);
          for (i=0,str = (IDL_STRING *)bytes;i<num;i++,str++,ptr += maxlen)
	  {
            if (str->s) strcpy(ptr,str->s);
            if (str->slen < maxlen) strncat(ptr,blanks,maxlen - str->slen);
          }
          free(blanks);
	}
        else
          arrayArgs[idx].pointer = 0;
        }
        else
	{
  	IDL_STRING_L *str;
        int num = 1;
        unsigned short maxlen;
	arrayArgs[idx].dtype = DTYPE_T;
        for (i=0;i<arrayArgs[idx].dimct;i++)
          num = num * arrayArgs[idx].m[i];
        for (i=0,str = (IDL_STRING_L *)bytes,maxlen=0;i<num;i++,str++)
          if (str->slen > maxlen) maxlen = str->slen;
        arrayArgs[idx].length = maxlen;
        arrayArgs[idx].arsize = maxlen * num;
        if (arrayArgs[idx].arsize > 0)
	{
          char *ptr;
          char *blanks;
          blanks = malloc(maxlen + 1);
          ptr = arrayArgs[idx].pointer = arrayArgs[idx].a0 = malloc(arrayArgs[idx].arsize + 1);
          memset(blanks,32,maxlen);
          for (i=0,str = (IDL_STRING_L *)bytes;i<num;i++,str++,ptr += maxlen)
	  {
            if (str->s) strcpy(ptr,str->s);
            if (str->slen < maxlen) strncat(ptr,blanks,maxlen - str->slen);
          }
          free(blanks);
	}
        else
          arrayArgs[idx].pointer = 0;
        }
      }
      break;
    case 9: arrayArgs[idx].length = 16; arrayArgs[idx].dtype = DTYPE_FTC; arrayArgs[idx].arsize = argsize[argsize[0]+2] * 16; break;
    case 12: arrayArgs[idx].length = 2; arrayArgs[idx].dtype = DTYPE_WU;  arrayArgs[idx].arsize = argsize[argsize[0]+2] * 2; break;
    case 13: arrayArgs[idx].length = 4; arrayArgs[idx].dtype = DTYPE_LU;  arrayArgs[idx].arsize = argsize[argsize[0]+2] * 4; break;
    case 14: arrayArgs[idx].length = 8; arrayArgs[idx].dtype = DTYPE_Q;   arrayArgs[idx].arsize = argsize[argsize[0]+2] * 8; break;
    case 15: arrayArgs[idx].length = 8; arrayArgs[idx].dtype = DTYPE_QU;  arrayArgs[idx].arsize = argsize[argsize[0]+2] * 8; break;
    default: return 0;
    }
    return (void *)&arrayArgs[idx];
  }
}
    
int IdlMdsValue(int argc, void **argv)
{
  int status;
  void *arglist[18];
  struct descriptor expression = {0,DTYPE_T,CLASS_S,0};
  EMPTYXD(tmp);
  int argidx = 1;
  int i;
  static int clear=4;
  static DESCRIPTOR_LONG(clear_d,&clear);
  BlockSig(SIGALRM);
  expression.length = strlen((char *)argv[0]);
  expression.pointer = (char *)argv[0];
  arglist[argidx++] = (void *)&expression;
  for (i=3;i < argc; i += 2)
  {
    arglist[argidx++] = (void *)MakeDescr(argidx - 2,(int *)argv[i],argv[i+1]);
  }
  arglist[argidx++] = (void *)&tmp;
  arglist[argidx++] = MdsEND_ARG;
  *(long *)&arglist[0] = argidx; 
  status = LibCallg(arglist,TdiExecute);
  if (status & 1)
  {
    status = TdiData(tmp.pointer,&mdsValueAnswer MDS_END_ARG);
    MdsFree1Dx(&tmp, NULL);
    if (status & 1)
    {
      if (mdsValueAnswer.pointer->dtype == DTYPE_F ||
          mdsValueAnswer.pointer->dtype == DTYPE_FS)
      {
        float float_v = (float)0.0;
        DESCRIPTOR_FLOAT(float_d,0);
        float_d.pointer = (char *)&float_v;
		if (float_d.dtype != mdsValueAnswer.pointer->dtype)
          TdiCvt(&mdsValueAnswer,&float_d,&mdsValueAnswer MDS_END_ARG);
      }
      else if (mdsValueAnswer.pointer->dtype == DTYPE_D ||
               mdsValueAnswer.pointer->dtype == DTYPE_G ||
               mdsValueAnswer.pointer->dtype == DTYPE_FT ||
               mdsValueAnswer.pointer->dtype == DTYPE_O ||
               mdsValueAnswer.pointer->dtype == DTYPE_OU)
      {
        double double_v = 0.0;
        struct descriptor double_d = {sizeof(double), DTYPE_NATIVE_DOUBLE, CLASS_S, 0};
        double_d.pointer = (char *)&double_v;
		double_d.dtype = DTYPE_NATIVE_DOUBLE;
		if (double_d.dtype != mdsValueAnswer.pointer->dtype)
          TdiCvt(&mdsValueAnswer,&double_d,&mdsValueAnswer MDS_END_ARG);
      }
      else if (mdsValueAnswer.pointer->dtype == DTYPE_FC ||
               mdsValueAnswer.pointer->dtype == DTYPE_FSC)
      {
        float float_v[2] = {(float)0.0,(float)0.0};
        struct descriptor complex_d = {sizeof(float_v), DTYPE_FLOAT_COMPLEX, CLASS_S, 0};
        complex_d.pointer = (char *)float_v;
		if (complex_d.dtype != mdsValueAnswer.pointer->dtype)
          TdiCvt(&mdsValueAnswer,&complex_d,&mdsValueAnswer MDS_END_ARG);
      }
      else if (mdsValueAnswer.pointer->dtype == DTYPE_DC ||
               mdsValueAnswer.pointer->dtype == DTYPE_GC ||
               mdsValueAnswer.pointer->dtype == DTYPE_FTC)
      {
        double double_v[2] = {0.0,0.0};
        struct descriptor dcomplex_d = {sizeof(double_v), DTYPE_DOUBLE_COMPLEX, CLASS_S, 0};
        dcomplex_d.pointer = (char *)double_v;
		if (dcomplex_d.dtype != mdsValueAnswer.pointer->dtype)
          TdiCvt(&mdsValueAnswer,&dcomplex_d,&mdsValueAnswer MDS_END_ARG);
      }
      ((char *)argv[2])[0] = 0;
      if (mdsValueAnswer.pointer->class == CLASS_S)
      {
        switch (mdsValueAnswer.pointer->dtype)
		{
	case DTYPE_B:  strcpy((char *)argv[2],"if answer gt 127 then answer = fix(answer)-256");
        case DTYPE_BU: strcpy((char *)argv[1],"answer = 0b"); break; 
        case DTYPE_W: strcpy((char *)argv[1],"answer = 0"); break;
        case DTYPE_WU: strcpy((char *)argv[1],"answer = uint(0)"); break;
        case DTYPE_L:  strcpy((char *)argv[1],"answer = 0L"); break;
        case DTYPE_LU: strcpy((char *)argv[1],"answer = ulong(0)"); break;
        case DTYPE_Q:  strcpy((char *)argv[1],"answer = long64(0)"); break;
        case DTYPE_QU: strcpy((char *)argv[1],"answer = ulong64(0)"); break;
        case DTYPE_NATIVE_FLOAT: strcpy((char *)argv[1],"answer = 0.0"); break;
        case DTYPE_NATIVE_DOUBLE: strcpy((char *)argv[1],"answer = double(0.0)"); break;
        case DTYPE_FLOAT_COMPLEX: strcpy((char *)argv[1],"answer = complex(0.0)"); break;
        case DTYPE_DOUBLE_COMPLEX: strcpy((char *)argv[1],"answer = dcomplex(0.0)"); break;
        case DTYPE_T: 
              if (mdsValueAnswer.pointer->length > 0)
			  {
                 sprintf((char *)argv[1],"answer = bytarr(%d)",mdsValueAnswer.pointer->length);
	             strcpy((char *)argv[2],"answer = string(answer)");
			  }
              else
			  {
		         strcpy((char *)argv[1],"answer = ''");
              }
	      break;
        }
      }
      else if (mdsValueAnswer.pointer->class == CLASS_A)
      {
        array_coeff *ptr = (array_coeff *)mdsValueAnswer.pointer;
        char dims[512] = "(";
        int i;
        if (ptr->aflags.coeff)
          for (i=0;i<ptr->dimct;i++) sprintf(dims,"%s%d,",dims,ptr->m[i] > 0 ? ptr->m[i] : 1);
        else
          sprintf(dims,"%s%d,",dims,((ptr->arsize/ptr->length) > 0) ? (ptr->arsize/ptr->length) : 1);
        dims[strlen(dims)-1]=')';
        switch (mdsValueAnswer.pointer->dtype)
	{
	case DTYPE_B: strcpy((char *)argv[2],"if max(answer) gt 127 then answer = fix(answer)-256");
        case DTYPE_BU: strcpy((char *)argv[1],"answer = bytarr"); strcat((char *)argv[1],dims); break; 
        case DTYPE_W: strcpy((char *)argv[1],"answer = intarr"); strcat((char *)argv[1],dims); break;
        case DTYPE_WU: strcpy((char *)argv[1],"answer = uintarr"); strcat((char *)argv[1],dims); break;
        case DTYPE_L: strcpy((char *)argv[1],"answer = lonarr"); strcat((char *)argv[1],dims); break;
        case DTYPE_LU: strcpy((char *)argv[1],"answer = ulonarr"); strcat((char *)argv[1],dims); break;
        case DTYPE_Q: strcpy((char *)argv[1],"answer = lon64arr"); strcat((char *)argv[1],dims); break;
        case DTYPE_QU: strcpy((char *)argv[1],"answer = ulon64arr"); strcat((char *)argv[1],dims); break;
        case DTYPE_NATIVE_FLOAT: strcpy((char *)argv[1],"answer = fltarr"); strcat((char *)argv[1],dims); break;
        case DTYPE_NATIVE_DOUBLE: strcpy((char *)argv[1],"answer = dblarr"); strcat((char *)argv[1],dims); break;
        case DTYPE_FLOAT_COMPLEX: strcpy((char *)argv[1],"answer = complexarr"); strcat((char *)argv[1],dims); break;
        case DTYPE_DOUBLE_COMPLEX: strcpy((char *)argv[1],"answer = dcomplexarr"); strcat((char *)argv[1],dims); break;
        case DTYPE_T: 
	      if (mdsValueAnswer.pointer->length > 0)
		  {
            sprintf((char *)argv[1],"answer = bytarr(%d,",mdsValueAnswer.pointer->length); strcat((char *)argv[1],&dims[1]);
	        strcpy((char *)argv[2],"answer = strtrim(string(answer))");
          }
          else
		  {
	        strcpy((char *)argv[1],"answer = strarr"); strcat((char *)argv[1],dims);
          }
          break;
        }
      }
    }
  }
  for (i=0;i<16;i++)
  {
    if (arrayArgs[i].dtype == DTYPE_T && arrayArgs[i].pointer != NULL)
    {
      free(arrayArgs[i].pointer);
      arrayArgs[i].pointer = 0;
    }
  }
  TdiDebug(&clear_d,&tmp MDS_END_ARG);
  MdsFree1Dx(&tmp,0);
  UnBlockSig(SIGALRM);
  return status;
}

int IdlMdsPut(int argc, void **argv)
{
  int status;
  void *arglist[18];
  struct descriptor expression = {0,DTYPE_T,CLASS_S,0};
  EMPTYXD(tmp);
  int argidx = 1;
  int i;
  int nid;
  BlockSig(SIGALRM);
  status = TreeFindNode((char *)argv[0],&nid);
  if (status & 1)
  {
    expression.length = strlen((char *)argv[1]);
    expression.pointer = (char *)argv[1];
    arglist[argidx++] = (void *)&expression;
    for (i=2;i < argc; i += 2)
    {
      arglist[argidx++] = (void *)MakeDescr(argidx - 2,(int *)argv[i],argv[i+1]);
    }
    arglist[argidx++] = (void *)&tmp;
    arglist[argidx++] = MdsEND_ARG;
    *(int *)&arglist[0] = argidx; 
    status = LibCallg(arglist,TdiCompile);
    if (status & 1)
    {
      status = TreePutRecord(nid,(struct descriptor *)&tmp,0);
      MdsFree1Dx(&tmp, NULL);
    }
    for (i=0;i<16;i++)
    {
      if (arrayArgs[i].dtype == DTYPE_T && arrayArgs[i].pointer != NULL)
      {
        free(arrayArgs[i].pointer);
        arrayArgs[i].pointer = 0;
      }
    }
  }
  UnBlockSig(SIGALRM);
  return status;
}

int IdlGetAns(int argc, void **argv)
{
  if (mdsValueAnswer.pointer->class == CLASS_S)
  {
    memcpy(argv[0], mdsValueAnswer.pointer->pointer, mdsValueAnswer.pointer->length);
  }
  else if (mdsValueAnswer.pointer->class = CLASS_A)
  {
    memcpy(argv[0], mdsValueAnswer.pointer->pointer, ((struct descriptor_a *)mdsValueAnswer.pointer)->arsize);
  }
  MdsFree1Dx(&mdsValueAnswer,NULL);
  return 1;
} 
