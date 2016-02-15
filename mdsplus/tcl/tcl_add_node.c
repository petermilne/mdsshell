#include        "tclsysdef.h"
#include        <ncidef.h>
#include        <usagedef.h>

/**********************************************************************
* TCL_ADD_NODE.C --
*
* TclAddNode:  Add a node
*
* History:
*  02-Feb-1998  TRG  Create.  Ported from original mds code.
*
************************************************************************/


#define DSC(A)        (struct descriptor *)(A)



	/*****************************************************************
	 * TclAddNode:
	 * Add a node
	 *****************************************************************/
int   TclAddNode()		/* Return: status			*/
   {
    int   k;
    int   nid;
    int   sts;
    int   stsQual;
    char  usage;
    DYNAMIC_DESCRIPTOR(dsc_nodnam);
    DYNAMIC_DESCRIPTOR(dsc_modelType);
    DYNAMIC_DESCRIPTOR(dsc_qualifiers);
    DYNAMIC_DESCRIPTOR(dsc_usageStr);

    sts = cli_get_value("NODENAME",&dsc_nodnam);
    l2u(dsc_nodnam.dscA_pointer,0);

    if (cli_get_value("MODEL",&dsc_modelType) & 1)
       {
        stsQual = cli_get_value("QUALIFIERS",&dsc_qualifiers);
        if (stsQual & 1)
            fprintf(stderr,"--> QUALIFIERS option is obsolete (ignored)\n");
        sts = TreeAddConglom(dsc_nodnam.dscA_pointer,
                            dsc_modelType.dscA_pointer,&nid);
       }
    else
       {
        cli_get_value("USAGE",&dsc_usageStr);
        str_prefix(&dsc_usageStr,"USAGE.");
        cli_get_value(&dsc_usageStr,&dsc_usageStr);
        sscanf(dsc_usageStr.dscA_pointer,"%d",&k);
        usage = (char)k;
        sts = TreeAddNode(dsc_nodnam.dscA_pointer,&nid,usage);
       }

    if (sts & 1)
        TclNodeTouched(nid,new);
    else
       {
        MdsMsg(sts,"Error adding node %s",dsc_nodnam.dscA_pointer);
#ifdef vms
        lib$signal(sts,0);
#endif
       }
    MdsFree1Dx((struct descriptor_xd *)&dsc_nodnam, NULL);
    MdsFree1Dx((struct descriptor_xd *)&dsc_modelType, NULL);
    MdsFree1Dx((struct descriptor_xd *)&dsc_qualifiers, NULL);
    MdsFree1Dx((struct descriptor_xd *)&dsc_usageStr, NULL);
    return sts;
   }
