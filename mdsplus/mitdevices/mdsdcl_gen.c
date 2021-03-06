#include <mitdevices_msg.h>
#include <mds_gendevice.h>
#include "mdsdcl_gen.h"
int mdsdcl__add(struct descriptor *name_d_ptr, struct descriptor *dummy_d_ptr, int *nid_ptr)
{
	static DESCRIPTOR(library_d, "MIT$DEVICES");
	static DESCRIPTOR(model_d, "MDSDCL");
	static DESCRIPTOR_CONGLOM(conglom_d, &library_d, &model_d, 0, 0);
	int usage = TreeUSAGE_DEVICE;
	int curr_nid, old_nid, head_nid, status;
	long int flags = NciM_WRITE_ONCE;
	NCI_ITM flag_itm[] = {{2, NciSET_FLAGS, 0, 0},{0, 0, 0, 0}};
	char *name_ptr = strncpy(malloc(name_d_ptr->length+1),name_d_ptr->pointer,name_d_ptr->length);
	flag_itm[0].pointer = (unsigned char *)&flags;
	name_ptr[name_d_ptr->length]=0;
	status = TreeStartConglomerate(MDSDCL_K_CONG_NODES);
	if (!(status & 1)) return status;
	status = TreeAddNode(name_ptr, &head_nid, usage);
	if (!(status & 1)) return status;
	*nid_ptr = head_nid;
	 status = TreeSetNci(head_nid, flag_itm);
	status = TreePutRecord(head_nid, (struct descriptor *)&conglom_d,0);
	if (!(status & 1)) return status;
	status = TreeGetDefaultNid(&old_nid);
	if (!(status & 1)) return status;
	status = TreeSetDefaultNid(head_nid);
	if (!(status & 1)) return status;
	ADD_NODE(:COMMENT, TreeUSAGE_TEXT)
	ADD_NODE(:VERBS, TreeUSAGE_TEXT)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE(:COMMANDS, TreeUSAGE_TEXT)
	flags |= NciM_NO_WRITE_SHOT;
	 status = TreeSetNci(curr_nid, flag_itm);
	ADD_NODE_ACTION(:EXEC_ACTION, EXECUTE, ANALYSIS, 50, 0, 0, MDSDCL_SERVER, 0)
	status = TreeEndConglomerate();
		if (!(status & 1)) return status;
	return(TreeSetDefaultNid(old_nid));
}

int mdsdcl__part_name(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr, struct descriptor *out_d)
{
	int element = 0, status;
 	NCI_ITM nci_list[] = {{4, NciCONGLOMERATE_ELT, 0, 0},{0,0,0,0}};
	nci_list[0].pointer = (unsigned char *)&element;
	status = TreeGetNci(*(int *)nid_d_ptr->pointer, nci_list);
		if (!(status & 1)) return status;
	switch(element)  {
		case(MDSDCL_N_HEAD + 1) : StrFree1Dx(out_d); break;
		case(MDSDCL_N_COMMENT + 1) : COPY_PART_NAME(:COMMENT) break;
		case(MDSDCL_N_VERBS + 1) : COPY_PART_NAME(:VERBS) break;
		case(MDSDCL_N_COMMANDS + 1) : COPY_PART_NAME(:COMMANDS) break;
		case(MDSDCL_N_EXEC_ACTION + 1) : COPY_PART_NAME(:EXEC_ACTION) break;
		default : status = TreeILLEGAL_ITEM;
	}
	return status;
}

	extern int mdsdcl___execute();
#define free_xd_array { int i; for(i=0; i<2;i++) if(work_xd[i].l_length) MdsFree1Dx(&work_xd[i],0);}
#define error(nid,code,code1) {free_xd_array return GenDeviceSignal(nid,code,code1);}

int mdsdcl__execute(struct descriptor *nid_d_ptr, struct descriptor *method_d_ptr)
{
	declare_variables(InExecuteStruct)
	struct descriptor_xd work_xd[2];
	int xd_count = 0;
	memset((char *)work_xd, '\0', sizeof(struct descriptor_xd) * 2);
	initialize_variables(InExecuteStruct)

	read_string_error(MDSDCL_N_COMMANDS,commands, DEV$_BAD_COMMANDS);
	read_string_error(MDSDCL_N_VERBS,verbs, DEV$_BAD_VERBS);
	status = mdsdcl___execute(nid_d_ptr, &in_struct);
	free_xd_array
	return status;
}
#undef free_xd_array
